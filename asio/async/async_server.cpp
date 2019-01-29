#if 1
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>

using namespace boost::asio;
using boost::asio::ip::tcp;
#define MEN_FN(x)		boost::bind(&self_type::x, shared_from_this())
#define MEN_FN1(x, y)	boost::bind(&self_type::x, shared_from_this(), y)
#define MEN_FN2(x, y, z)boost::bind(&self_type::x, shared_from_this(), y, z)
io_context service;
class talk_to_client;
typedef std::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
typedef boost::system::error_code error_code;
array clients;
class talk_to_client :public std::enable_shared_from_this<talk_to_client>, boost::noncopyable {
public:
	typedef client_ptr ptr;
	typedef talk_to_client self_type;
	talk_to_client() :
		sock_(service), started_(true), username_(), timer_(service), last_ping(), clients_changed_(false)
	{}
	void start() {
		started_ = true;
		clients.push_back(shared_from_this());
		last_ping = boost::posix_time::microsec_clock::local_time();
		do_read();
	}

	tcp::socket &sock() { return sock_; }
	const std::string &username() { return username_; }
	void set_clients_changed() {
		clients_changed_ = true;
	}
	static ptr new_() {
		ptr new_(new talk_to_client);
		return new_;
	}

	void stop() {
		if (!started_)return;
		started_ = false;
		sock_.close();
		ptr self = shared_from_this();
		array::iterator it = std::find(clients.begin(), clients.end(), self);
		clients.erase(it);
		set_clients_changed();
	}
	void on_read(const error_code& err, size_t bytes) {
		if (err)stop();
		if (!started())return;
		std::string msg(read_buffer_, bytes);
		if (msg.find("login ") == 0)on_login(msg);
		else if (msg.find("ping") == 0)on_ping();
		else if (msg.find("ask_clients") == 0)on_clients();
	}

	void on_login(const std::string& msg) {
		std::istringstream in(msg);
		in >> username_ >> username_;
		do_write("login ok\n");
		set_clients_changed();
	}

	void on_ping() {
		do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}

	void on_clients() {
		std::string msg;
		for (array::const_iterator b = clients.begin(); b != clients.end(); ++b) {
			msg += (*b)->username() + " ";
		}
		do_write("clients " + msg + "\n");
	}
	bool started()const { return started_; }
	void do_read() {
		async_read(sock_, buffer(read_buffer_), MEN_FN2(read_complete, _1, _2), MEN_FN2(on_read, _1, _2));
		post_check_ping();
	}
	void do_ping() {
		do_write("ping\n");
	}

	void do_ask_clients() {
		do_write("ask_clients\n");
	}

	void on_write(const error_code& err, size_t bytes) {
		do_read();
	}
	void on_check_ping() {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		if ((now - last_ping).total_milliseconds() > 5000)
			stop();
		last_ping = boost::posix_time::microsec_clock::local_time();
	}
	void post_check_ping() {
		timer_.expires_from_now(boost::posix_time::millisec(5000));
		timer_.async_wait(MEN_FN(on_check_ping));
	}
	void do_write(const std::string& msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()), MEN_FN2(on_write, _1, _2));
	}
	size_t read_complete(const error_code& err, size_t bytes) {
		if (err)return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}
private:
	tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string username_;
	deadline_timer timer_;
	boost::posix_time::ptime last_ping;
	bool clients_changed_;
};
tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 4396));
void handle_accept(talk_to_client::ptr client, const error_code& err) {
	client->start();
	talk_to_client::ptr new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(&handle_accept, new_client, _1));
}

int main(int argc, char* argv[]) {
	talk_to_client::ptr client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(&handle_accept, client, _1));
	service.run();
}
#endif
