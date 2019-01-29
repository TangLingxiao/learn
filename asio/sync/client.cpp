#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>

using namespace boost::asio::ip;
using boost::asio::ip::tcp;
#if 1
class talk_to_srv {
public:
	talk_to_srv(boost::asio::io_context& io_context_, const std::string& s)
		:socket_(io_context_), already_read_(0), started_(true), user_name_(s) {

	}
	void connect(const tcp::endpoint& ep) {
		socket_.connect(ep);
	}
	void loop() {
		write("login " + user_name_ + "\n");
		read_answer();
		while (started_) {
			write_request();
			read_answer();
			boost::this_thread::sleep(boost::posix_time::millisec(rand() % 7000));
		}
	}
	const std::string & username()const { return user_name_; }
	void write(const std::string& str) {
		socket_.write_some(boost::asio::buffer(str));
	}
	void read_answer() {
		already_read_ = 0;
		boost::asio::read(socket_, boost::asio::buffer(buff_), boost::bind(&talk_to_srv::read_complete, this, _1, _2));
		process_msg();
	}
	void process_msg() {
		std::string msg(buff_, already_read_);
		if (msg.find("login ") == 0) {
			on_login();
		}
		else if (msg.find("ping") == 0) {
			on_ping(msg);
		}
		else if (msg.find("clients ") == 0) {
			on_clients(msg);
		}
		else {
			std::cerr << "invalid msg " << msg << std::endl;
		}
	}
	void do_ask_clients() {
		write("ask_clients\n");
		read_answer();
	}
	void on_login() {
		do_ask_clients();
	}
	void on_ping(const std::string& msg) {
		std::istringstream in(msg);
		std::string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed") {
			do_ask_clients();
		}
	}
	void on_clients(const std::string& msg) {
		std::string clients = msg.substr(8);
		std::cout << user_name_ << ", new client list:" << clients;
	}
	void write_request() {
		write("ping\n");
	}
	size_t read_complete(const boost::system::error_code& err, size_t n) {
		if (err) {
			return 0;
		}
		already_read_ = n;
		bool found = std::find(buff_, buff_ + n, '\n') < buff_ + n;
		return found ? 0 : 1;
	}
private:
	tcp::socket socket_;
	enum { max_msg = 1024 };
	int already_read_;
	char buff_[max_msg];
	bool started_;
	std::string user_name_;
};


void run_client(const std::string& client_name,const tcp::endpoint& ep)
{
	boost::asio::io_context io_context_;
	talk_to_srv c(io_context_, client_name);
	try {
		c.connect(ep);
		c.loop();
	}
	catch (boost::system::error_code& err) {
		std::cout << "client terminated " << std::endl;
	}
}

int main(int argc, char* argv[])
{
	
	tcp::endpoint ep(make_address_v4("127.0.0.1"), 4396);
	const char* users[] = { "Johon" ,"James","Lucy","Tracy","Frank","Abby", NULL};
	boost::thread_group threads;
	for (const char** p = users; *p; ++p) {
		threads.create_thread(boost::bind(&run_client, *p, ep));
	}
	threads.join_all();
	return 0;
}
#endif
