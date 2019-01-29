#if 1
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;
size_t read_complete(char* buf, const boost::system::error_code& err, size_t bytes)
{
	if (err) {
		return 0;
	}
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

void sync_echo()
{
	tcp::endpoint ep(tcp::v4(), 4396);
	boost::asio::io_context io_context_;
	tcp::socket socket_(io_context_);
	tcp::acceptor acceptor_(io_context_, ep);
	char buf[1024];
	while (1) {
		acceptor_.accept(socket_);
		size_t bytes = boost::asio::read(socket_, boost::asio::buffer(buf), boost::bind(&read_complete, buf, _1, _2));
		std::string msg(buf, bytes);
		boost::asio::write(socket_, boost::asio::buffer(msg));
		socket_.close();
	}
}

class session :public boost::noncopyable, public boost::enable_shared_from_this<session>
{
public:
	session(boost::asio::io_context& io_context_) :io_context_(io_context_), socket_(io_context_) {
		memset(buf, 0, sizeof(buf));
	}


	tcp::socket& socket() {
		return socket_;
	}
	void start_accept() {
		do_read();
	}

	void do_read() {
		boost::asio::async_read(socket_, boost::asio::buffer(buf), boost::bind(&read_complete, buf, _1, _2),
			boost::bind(&session::on_read, shared_from_this(), _1, _2));
	}

	void on_read(const boost::system::error_code& err, std::size_t bytes) {
		msg.assign(buf, bytes);
		boost::asio::async_write(socket_, boost::asio::buffer(msg), boost::bind(&session::on_write, shared_from_this(), _1, _2));
	}

	void on_write(const boost::system::error_code& err, std::size_t bytes) {		
		//socket_.close();
		do_read();
	}

	void close() {
		socket_.close();
	}
private:
	boost::asio::io_context& io_context_;
	tcp::socket socket_;
	char buf[1024];
	std::string msg;
};

class acceptor :public boost::noncopyable, public boost::enable_shared_from_this<acceptor>
{
public:
	acceptor(boost::asio::io_context& io_context_) :io_context_(io_context_), acceptor_(io_context_, tcp::endpoint(tcp::v4(), 4396)) {

	}

	void start_acceptor() {
		boost::shared_ptr<session> session(new session(io_context_));
		session->start_accept();
		acceptor_.async_accept(session->socket(), [this](const boost::system::error_code& err)
		{
			shared_from_this()->start_acceptor();
		});
	}
private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

int main()
{
	//sync_echo();
	boost::asio::io_context io_context_;
	boost::shared_ptr<acceptor> acceptor_(new acceptor(io_context_));
	acceptor_->start_acceptor();
	io_context_.run();
	system("pause");
	return 0;
}
#endif
