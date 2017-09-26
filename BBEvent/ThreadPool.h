#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Event.h"
//#include "Receiver.h"
#include "Signal.h"
#include "SignalFunctor.h"

#include <mutex>
#include <thread>
#include <functional>
#include <deque>
#include <vector>

namespace BB
{

	//! TODO:	Need to solve eventbuildup
	//!			And buildup of events that are never solved (when their eSolveFlag is never called)
	//!			Maybe when Event isnt solved in some amount of time, its flag is set to any ?
	//!				This solves not-calling-eSolveFlag problem
	//!				This creates other problems
	
	//! Purpose of this class is to manage jobs between threads
	class CThreadPool
	{
	private:
		std::mutex m_WorkingMutex;							//	Mutex for m_Working
		bool m_Working = false;

		std::deque < t_SignaledEventFunctor > m_JobQueue;	// Job queue;	CEvent must be deleted after use
		mutable std::mutex m_QueueMutex;					//	Mutex for accessing queue 

		std::condition_variable m_QueueConditionVariable;	//	Is incremented for all threads on new job

		std::vector<std::thread> m_Threads;					
		int64_t m_NumThreads;

	
	private:
		//this should be optimized
		void run();

		//this should be optimized
		//! \param reference to signaled event functor - Reference due to automatic deletion of functors internal data on call
		void solve(t_SignaledEventFunctor SEFun, eSolveFlags flag);

		//! Blocks and Returns signaled event functor when job queue is filled
		t_SignaledEventFunctor pop(eSolveFlags flag, bool wait = true);

		std::vector<t_SignaledEventFunctor> popRemaining();
		template<typename Te>
		std::vector<t_SignaledEventFunctor> popRemaining();
		std::vector<t_SignaledEventFunctor> popRemaining(CEventSequencer* receiver);

		//debug - for checking job buildup
		uint64_t getJobCount(eSolveFlags flag);
	public:

		CThreadPool(int64_t numThreads = 0);
		~CThreadPool();

		int getJobCount();


		void start();
		void push(t_Signal signal, std::shared_ptr<CEvent> event, bool highPriority = false);
		void stopAll();


		void handleAllEvents();
		void handleAllEvents(BB::eSolveFlags flags);
		void handleAllEvents(CEventSequencer* receiver, BB::eSolveFlags flags);

		int64_t threadCount();

		//TODO: Add multiple event handling using variadic templates
		template<typename Te>
		void handleAllEvents(eSolveFlags flags);
	};

	template<typename Te>
	std::vector<t_SignaledEventFunctor> CThreadPool::popRemaining()
	{
		std::vector<t_SignaledEventFunctor> result;

		auto it = m_JobQueue.begin();
		while (it != m_JobQueue.end())
		{
			if (Te* newE = dynamic_cast<Te*>(it->event))
			{
				result.push_back((*it));
				it = m_JobQueue.erase(it);
			}
			else
				++it;
		}

		return result;
	}


	template<typename Te>
	void CThreadPool::handleAllEvents(eSolveFlags flags)
	{
		std::vector<t_SignaledEventFunctor> allEvents;
		if (m_QueueMutex.try_lock())
		{
			allEvents = popRemaining<Te>();
			
			m_QueueMutex.unlock();
		}
		
		for (auto j : allEvents)
		{
			solve(j, flags);
		}
	}

}

#endif

/*
replace std pair with event functor ?
class CEventFunctor
{
public:
CEventFunctor(std::function<void(const void*)> f, const void* event): m_F(f), m_Event(event)
{}
template<typename Te>
void operator() (const Te& e)
{
std::function<void(const Te&)> f_cvoidp = *(reinterpret_cast<std::function<void(const Te&)>*>(&m_F));//casted
f_cvoidp(e);
}

std::function<void(const void*)> m_F;
const void* m_Event;
};

*/