#if 1
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::asio::ip::tcp;
boost::recursive_mutex cs;
class talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> client_array;
client_array clients;
boost::asio::io_context io_context_;
class talk_to_client : public boost::enable_shared_from_this<talk_to_client> {
public:
	talk_to_client(boost::asio::io_context& io_context_)
		:socket_(io_context_), already_read_(0), started_(true), user_name_(),
		clients_changed_(false), last_ping(boost::posix_time::max_date_time)
	{}
	const std::string& username()const { return user_name_; }
	void answer_to_client() {
		try
		{
			read_request();
			process_request();
		}
		catch (boost::system::system_error&)
		{
			stop();
		}
		if (timed_out()) {
			stop();
		}
	}
	void read_request() {
		if (socket_.available()) {
			already_read_ += socket_.read_some(boost::asio::buffer(buff_ + already_read_, max_msg - already_read_));
		}
	}
	void process_request() {
		bool found_enter = std::find(buff_, buff_ + already_read_, '\n') < buff_ + already_read_;
		if (!found_enter) {
			return;
		}
		last_ping = boost::posix_time::microsec_clock::local_time();
		size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
		std::string msg(buff_, pos);//完整的消息
		std::copy(buff_ + pos + 1, buff_ + max_msg, buff_);
		already_read_ -= pos + 1;
		if (msg.find("login ") == 0) { on_login(msg); }
		else if (msg.find("ping") == 0) { on_ping(); }
		else if (msg.find("ask_clients") == 0) { on_clients(); }
		else {
			std::cerr << "invalid msg " << msg << std::endl;
		}
	}
	
	void on_login(const std::string& msg) {
		std::istringstream in(msg);
		in >> user_name_ >> user_name_;
		write("login ok\n");
		update_clients_changed();
	}
	void on_ping() {
		write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}
	void on_clients() {
		std::string msg;
		{
			boost::recursive_mutex::scoped_lock lk(cs);
			for (client_array::const_iterator b = clients.begin(), e = clients.end(); b != e; ++b) {
				msg += (*b)->username() + " ";
			}
			write("clients " + msg + "\n");
		}

	}
	void update_clients_changed() {
		set_client_changed();
	}
	void write(const std::string& msg) {
		socket_.write_some(boost::asio::buffer(msg));
	}
	void stop() {
		boost::system::error_code err;
		socket_.close(err);
	}
	void set_client_changed() {
		clients_changed_ = true;
	}
	tcp::socket& sock() { return socket_; }
	bool timed_out()const {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		long long ms = (now - last_ping).total_milliseconds();
		return ms > 5000;
	}
private:
	tcp::socket socket_;
	enum { max_msg = 1024 };
	int already_read_;
	char buff_[max_msg];
	bool started_;
	std::string user_name_;
	bool clients_changed_;
	boost::posix_time::ptime last_ping;
};


void accept_thread() {
	tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), 4396));
	while (true) {
		client_ptr new_(new talk_to_client(io_context_));
		acceptor.accept(new_->sock());
		boost::recursive_mutex::scoped_lock lk(cs);
		clients.push_back(new_);
	}
}

void handler_clients_thread() {
	while (true) {
		boost::this_thread::sleep(boost::posix_time::millisec(1));
		boost::recursive_mutex::scoped_lock lk(cs);
		for (client_array::iterator b = clients.begin(), e = clients.end(); b != e; ++b) {
			(*b)->answer_to_client();
		}
		clients.erase(std::remove_if(clients.begin(), clients.end(), boost::bind(&talk_to_client::timed_out, _1)), clients.end());
	}
}

int main(int argc, char* argv[]) {
	boost::recursive_mutex cs;
	boost::thread_group threads;
	threads.create_thread(handler_clients_thread);
	threads.create_thread(accept_thread);
	threads.join_all();
}
#endif
