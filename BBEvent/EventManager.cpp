#include "EventManager.h"

namespace BB
{
	CEventManager::CEventManager(int64_t threadCount)
		:	m_ThreadPool(threadCount)
	{
		if (!this->start())
		{
			throw std::runtime_error("Failed to start eventManager");
		}
	}

	CEventManager::~CEventManager()
	{
		this->waitAllEvents();
		stop();

	}

	void CEventManager::allowDistribution(bool value)
	{
		m_DistributionAllowed = value;
	}
	void CEventManager::waitAllEvents()
	{
		while (m_ThreadPool.getJobCount() > 0 && m_ThreadPool.threadCount() > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	bool CEventManager::start()
	{
		if (m_ThreadPool.threadCount() > -1)
		{
			m_ThreadPool.start();
			return true;
		}
		return false;
	}

	void CEventManager::stop()
	{
		m_ThreadPool.stopAll();
		//handleAllEvents(eSolveFlags::ANY);
		return;
	}

	void CEventManager::handleAllEvents(eSolveFlags flags)
	{
		m_ThreadPool.handleAllEvents(flags);
	}
	void CEventManager::handleAllEvents(CEventSequencer* receiver, eSolveFlags flags)
	{
		m_ThreadPool.handleAllEvents(receiver, flags);
	}

	int64_t CEventManager::getThreadCount()
	{
		return m_ThreadPool.threadCount();
	}

	std::vector<std::string> CEventManager::getRegistryStr()
	{
		std::vector<std::string> result;

		for (auto r : m_ReceiverFuncs)
		{
			std::string eventName = (*r.first).name();
			eventName = eventName.substr(11, eventName.size() - 11);

			for (auto s : r.second)
			{
				std::string receiverClass = typeid(s.receiver).name();

				t_ReceiverConnectionNameRegistry::iterator connNameIT = m_ReceiverConnectionNameRegistry.find(s.receiver);
				if (connNameIT != m_ReceiverConnectionNameRegistry.end())
				{
					result.push_back((*connNameIT).second + "\t <-- \t" + eventName);
				}
			}
		}
		return result;
	}
}