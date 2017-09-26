#include "ThreadPool.h"
#include <iostream>
namespace BB
{
	CThreadPool::CThreadPool(int64_t numThreads) : m_NumThreads(numThreads)
	{}

	CThreadPool::~CThreadPool()
	{
		stopAll();
	}

	int CThreadPool::getJobCount()
	{
		m_QueueMutex.lock();
		int size = m_JobQueue.size();
		m_QueueMutex.unlock();
		return size;
	}
	uint64_t CThreadPool::getJobCount(eSolveFlags flag)
	{
		uint64_t getOrderNumber = 0;
		m_QueueMutex.lock();
		for (auto j : m_JobQueue)
		{
			eSolveFlags jobFlag = j.event->getSolveFlags();
			if ((jobFlag & flag) != eSolveFlags::NONE)
			{
				getOrderNumber++;
			}
		}
		m_QueueMutex.unlock();
		return getOrderNumber;
	}

	void CThreadPool::start()
	{
		if (m_Working == true)
			return;
		m_Working = true;
		for (int i = 0; i < m_NumThreads; i++)
		{
			m_Threads.push_back(std::thread(&CThreadPool::run, this));
		}
	}
	void CThreadPool::push(t_Signal signalReceiver, std::shared_ptr<CEvent> event, bool highPriority)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
				
		t_SignaledEventFunctor eventFunctor(signalReceiver, event);
		if(highPriority)
			m_JobQueue.push_front(eventFunctor);
		else
			m_JobQueue.push_back(eventFunctor);
			
		lock.unlock();

		m_QueueConditionVariable.notify_all();
	}
	void CThreadPool::stopAll()
	{
		bool doClose = false;
		{
			std::lock_guard<std::mutex> lock(m_WorkingMutex);
			if (m_Working == true)
			{
				doClose = true;
				m_Working = false;
			}
		}

		if (doClose)
		{
			//std::cout << "Notify All CV" << std::endl;
			m_QueueConditionVariable.notify_all();

			for (auto it = m_Threads.begin(); it != m_Threads.end(); ++it)
			{
				(*it).join();
			}
			m_Threads.clear();
			//std::cout << m_Threads.size() <<" Threads stopped" << std::endl;
		}
	}

	void CThreadPool::handleAllEvents()
	{
		handleAllEvents(eSolveFlags::ANY);
	}

	void CThreadPool::handleAllEvents(BB::eSolveFlags flags)
	{
		int i = 0;
		while (getJobCount() > 0)
		{
			
			auto job = pop(flags, false);
			if (job.isValid())
			{
				solve(job, flags);
			}

		}
	}

	void CThreadPool::handleAllEvents(CEventSequencer* receiver, BB::eSolveFlags flags)
	{
		for (auto& j : popRemaining(receiver))
		{
			solve(j, flags);
		}
	}

	int64_t CThreadPool::threadCount()
	{
		return m_NumThreads;
	}


	std::vector<t_SignaledEventFunctor> CThreadPool::popRemaining()
	{
		bool empty = false;
		std::vector<t_SignaledEventFunctor> result;

		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			result = std::vector<t_SignaledEventFunctor>(m_JobQueue.begin(), m_JobQueue.end());
			m_JobQueue.clear();
		}
		return result;
	}
	std::vector<t_SignaledEventFunctor> CThreadPool::popRemaining(CEventSequencer* receiver)
	{
		std::vector<t_SignaledEventFunctor> result;
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			auto jIT = m_JobQueue.begin();
			while (jIT != m_JobQueue.end())
			{
				if (jIT->signal.receiver == receiver)
				{
					result.push_back((*jIT));
					jIT = m_JobQueue.erase(jIT);
				}
				else
				{
					++jIT;
				}
			}
		}
		return result;
	}

	//private methods
	t_SignaledEventFunctor CThreadPool::pop(eSolveFlags flag, bool wait)
	{
		bool running = false;
		bool empty = false;
		t_SignaledEventFunctor job;


		std::unique_lock<std::mutex> lock(m_QueueMutex);
		do
		{
			
			{m_WorkingMutex.lock();
				running = m_Working;
			}m_WorkingMutex.unlock();

			empty = m_JobQueue.empty();

			if (empty && running && wait)
			{
				m_QueueConditionVariable.wait(lock);
			}
		} while (empty && running && wait);
		
		if (empty)
		{
			return job;
		}

		//std::deque < t_SignaledEventFunctor >::iterator it;
		bool atEnd = false;
		bool correctFlag = false;
		bool result = false;
		for (auto it = m_JobQueue.begin(); it < m_JobQueue.end(); it++)
		{
			correctFlag = (((*it).event->getSolveFlags() & flag) != eSolveFlags::NONE);
			if(correctFlag)
			{
				job = (*it);
				m_JobQueue.erase(it);
				break;
			}			
		}
		//OLD implementations, just queuedfirst, and requeued to front when didnt match flag
		//job = m_JobQueue.front();
		//m_JobQueue.pop_front();

		return job;
	}

	
	void CThreadPool::run()
	{
		bool working = true;
		do
		{
			
			if(m_WorkingMutex.try_lock() == true)
			{
				working = m_Working;
				m_WorkingMutex.unlock();
			}
			if (working)
			{
				auto f = pop(eSolveFlags::THREADED);
				solve(f, eSolveFlags::THREADED);
			}
			
		} while (working);
	}

	void CThreadPool::solve(t_SignaledEventFunctor SEFun, eSolveFlags flag)
	{
		
		if (SEFun.isValid() == false)
			return;
		if ((SEFun.event->getSolveFlags() & flag) != eSolveFlags::NONE)
		{
			SEFun.event->setSolveOrigin(flag);//set origin flag call to event for later check
			SEFun();//Event ptr is deleted after 
		}
		else
			SEFun.solved = false;

		//when not solved, push back of queue
		if (SEFun.solved == false)
			push(SEFun.signal, SEFun.event, false);

	}

		/*

		eSolveFlags functorFlags = SEFun.event->getSolveFlags();
		if (functorFlags == eSolveFlags::SPECIFIC)
			int i = 0;
		if ( (functorFlags & flag ) != eSolveFlags::NONE)
		{
			SEFun.event->setSolveOrigin(flag);//set origin flag call to event for later check
			SEFun();//Event ptr is deleted after 
		}
		else
		{

			//
			push(SEFun.signal, SEFun.event, true);//push back to front of the queue
		}

	}
	*/

}
