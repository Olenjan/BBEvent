#include <iostream>
#include <memory>

#include <random>

#include <EventManager.h>
#include <EventSequencer.h>
#include <Receiver.h>
#include <MethodReceiver.h>
#include <Event.h>

inline static int64_t randomInt64_t(int from, int to)
{
	if (from > to)
	{
		int tmpFrom = from;
		from = to;
		to = tmpFrom;
	}

	std::random_device randomDevice;

	std::default_random_engine engine(randomDevice());
	std::uniform_int_distribution<int64_t> uniform_distribution(from, to);
	int64_t mean = uniform_distribution(engine);
	return mean;
}

class CExampleEvent : public BB::CEvent
{
public:
	int m_I = 0;

	CExampleEvent(int i)
		:	CEvent(BB::eSolveFlags::ANY),//solvable on any thread
			m_I(i)
	{
	}

};

class CExampleSystem
	:	public BB::CEventSequencer,
		public BB::CReceiver<CExampleEvent>::CMethod//using method receiver, other receivers available
{
private:
	int64_t m_InternalCTR= 0;
public:
	CExampleSystem()
		:	CEventSequencer()
	{
		CEventSequencer::setSequentiality(false);//false enables solving of events simultaneously
		//when true, events are handled in row as they appear in queue - default value is true
	}

	virtual bool Event(std::shared_ptr<CExampleEvent> event)
	{
		//random calculations to delay each event 
		for (int i = 0; i < randomInt64_t(1000, 10000); i++)
		{
			int64_t r1 = randomInt64_t(10000, 100000);
			int64_t r2 = randomInt64_t(10000, 100000);
			int64_t result = (i * randomInt64_t(r1, r2)) % randomInt64_t(10000, 100000);
		}

		//since sequentiality is set false, access internal counter by locking the whole system down
		int threadSafeInternalCtr = 0;
		CEventSequencer::lockEvents();
		{
			threadSafeInternalCtr = m_InternalCTR;
			m_InternalCTR++;
		}
		CEventSequencer::unlockEvents();

		std::cout << "[" << std::this_thread::get_id() << "] " << "Example event i = '" << event->m_I << "' | Internal counter: '" << threadSafeInternalCtr <<"'"<< std::endl;

		return true;
	}
};


int main()
{
	//use 8 threads
	std::shared_ptr<BB::CEventManager> eventManager = std::make_shared<BB::CEventManager>(8);
	CExampleSystem newSystem;

	eventManager->registerReceiver<CExampleEvent>(&newSystem, "Example event connection");
	for (int i = 0; i < 10; i++)
	{
		eventManager->distributeEvent(CExampleEvent(i));
	}

	//wait enter to exit
	std::cin.get();
}