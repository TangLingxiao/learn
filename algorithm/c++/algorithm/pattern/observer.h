#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <mutex>
#include <vector>
#include <memory>

class Observer;



class Observerable :public std::enable_shared_from_this<Observerable>
{
public:
	void register_(std::weak_ptr<Observer>& observer);


	void notify();
	
private:
	std::vector<std::weak_ptr<Observer>> m_vObservers;
	std::mutex m_mtx;
};

class Observer :public std::enable_shared_from_this<Observer>
{
public:
	Observer(const std::shared_ptr<Observerable>& pOwner) :m_pOwner(pOwner)
	{

	}
	virtual ~Observer() {};
	void init()
	{
		m_pOwner->register_(std::weak_ptr<Observer>(shared_from_this()));
	}
	void update();
	
private:
	std::shared_ptr<Observerable> m_pOwner;
};
#endif
