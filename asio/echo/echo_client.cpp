#if 1
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

#define USE_ASYNC 1
size_t read_complete(char* buf, const boost::system::error_code& err, size_t bytes)
{
	if (err) {
		return 0;
	}
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

#if !USE_ASYNC
void sync_echo(std::string msg, const tcp::resolver::results_type& ep)
{
	msg += '\n';
	boost::asio::io_context io_context_;
	tcp::socket socket_(io_context_);
	//socket_.connect(ep);
	boost::asio::connect(socket_, ep);
	boost::asio::write(socket_, boost::asio::buffer(msg));
	char buf[1024];
	int bytes = boost::asio::read(socket_, boost::asio::buffer(buf, 1024), boost::bind(&read_complete, buf, _1, _2));
	std::string copy(buf, bytes - 1);
	msg = msg.substr(0, msg.size() - 1);
	std::cout << "server echoed out " << msg << ": " << (copy == msg ? "OK" : "FAIL") << std::endl;
	socket_.close();
}
#else
class connection :public boost::noncopyable, public std::enable_shared_from_this<connection>
{
public:
	connection(boost::asio::io_context& io_context_, std::string msg, const tcp::resolver::results_type& ep) :
		io_context_(io_context_), socket_(io_context_), msg_(msg), ep_(ep) {
		memset(buf, 0, sizeof(buf));
		msg_ += '\n';
	}
	~connection() {}
	void start() {
		boost::asio::async_connect(socket_, ep_, boost::bind(&connection::write, shared_from_this(), _1));
	}

	void write(const boost::system::error_code& err) {
		if (err) {
			socket_.close();
			return;
		}
		socket_.async_write_some(boost::asio::buffer(msg_), boost::bind(&connection::read, shared_from_this(), _1, _2));
	}

	void read(const boost::system::error_code& err, std::size_t bytes) {
		if (err) {
			socket_.close();
			return;
		}
		boost::asio::async_read(socket_, boost::asio::buffer(buf, 1024), boost::bind(&read_complete, buf, _1, _2), boost::bind(&connection::stop, shared_from_this(), _1, _2));
	}

	void stop(const boost::system::error_code& err, std::size_t bytes) {
		if (err) {
			socket_.close();
			return;
		}
		std::string copy(buf, bytes - 1);
		msg_ = msg_.substr(0, msg_.size() - 1);
		std::cout << "server echoed out " << msg_ << ": " << (copy == msg_ ? "OK" : "FAIL") << std::endl;
		socket_.close();
	}
private:
	boost::asio::io_context& io_context_;
	tcp::socket socket_;
	std::string msg_;
	const tcp::resolver::results_type& ep_;
	char buf[1024];
};
#endif


int main(int argc, char** argv)
{
	char* msgs[] = { "test001","test002","test003","test004",0 };
	boost::asio::io_context io_context_;
	tcp::resolver resolver_(io_context_);
	tcp::resolver::results_type ep = resolver_.resolve(argv[1], argv[2]);
	//tcp::endpoint ep(boost::asio::ip::address::from_string(argv[1]), atoi(argv[2]));
	boost::thread_group ghreads;
	for (char** pmsg = msgs; *pmsg; ++pmsg) {
#if !USE_ASYNC
		//sync_echo
		ghreads.create_thread(boost::bind(&sync_echo, *pmsg, ep));
#else
		//async_echo
		std::shared_ptr<connection> pconn(new connection(io_context_, *pmsg, ep));
		pconn->start();
#endif
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

#if !USE_ASYNC
	ghreads.join_all();
#else
	io_context_.run();
#endif

	system("pause");
	return 0;
}
#endif
