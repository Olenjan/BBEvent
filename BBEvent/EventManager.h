#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <map>
#include <utility>
#include <typeinfo>
#include <assert.h>
#include <vector>

#include "Signal.h"
#include "ThreadPool.h"
#include "Event.h"
#include "Receiver.h"


namespace BB
{
	//! TODO: Receiver to shared pointer
	//!			To solve problem where object is deleted, but pointer still used in eventMGR
	class CEventManager
	{
	private:
		//! <Event Type, pair of <function, pointer to receiver>>
		typedef std::map<const std::type_info*, std::vector<t_Signal>>					t_EventSignalRegistry;				//	<Event type_info			|	vector<t_Signal>>
		typedef std::map<const std::type_info*, std::vector<const std::type_info*>>		t_ReceiverEvents;					//	<receiver type info			|	event type info>
		typedef std::map<CEventSequencer*, std::string>									t_ReceiverConnectionNameRegistry;	//	<CEventSequencer*		|	Name in string>


		t_EventSignalRegistry				m_ReceiverFuncs;
		t_ReceiverConnectionNameRegistry	m_ReceiverConnectionNameRegistry;
		t_ReceiverEvents					m_ReceiverEvents;
		CThreadPool							m_ThreadPool;
		bool m_DistributionAllowed = true; 
		bool								m_Synced;
	public:
		//! \param threadCount, number of offsite threads to run.
		//! \param threadCount on <1 threadcount, no events area handled off thread. handleAllEvents should be used.
		CEventManager(int64_t threadCount = 0);

		~CEventManager();

		void allowDistribution(bool value);

		void waitAllEvents();

		//Starts the threads when not running.
		//! \returns true on successful start
		//! \returns false when already running or threads <0
		//! on 0 threads, no offsite threads are run, handleAllEvents should be called
		bool start();

		//! Stops managing events in thread pool
		//! returns when all threads in pool have stopped
		//! If any event left to handle, handles them in this thread
		//! Before stop, handleAllEvents should be called
		void stop();

		//! Handles jobs. Should only be called out on main thread
		void handleAllEvents(eSolveFlags flags);

		void handleAllEvents(CEventSequencer* receiver, eSolveFlags flags);

		template<typename Te>
		void handleAllEvents(eSolveFlags flags);


		//! Returns thread count used in threadpool
		int64_t getThreadCount();

		//! Registers receiver as event consumer.
		//! When an event is pushed, this one event is distrubuted between all receivers that have registered to that event.
		template<typename Te, typename Tr>
		void registerReceiver(Tr* receiver, std::string connectionName);

		//! Distributed copy of same event to all registered receivers
		//! \returns number of events distributed		
		template<typename Te>
		int64_t distributeEvent(const Te& newEvent);


		//! \returns vector<string> of registered parents and events
		std::vector<std::string> getRegistryStr();

	};

	template<typename Te, typename Tr>
	void CEventManager::registerReceiver(Tr* receiver, std::string connectionName)
	{
		static_assert(std::is_base_of<BB::CEvent, Te>(), "Te is not base of CEvent.");
		static_assert(std::is_base_of<BB::CReceiver<Te>::CInterface, Tr>(), "Tr is not base BB::CReceiver<Te>::CInterface");
		static_assert(std::is_base_of<BB::CEventSequencer, Tr>(), "Tr has not inherited a receiver base");
		BB::CReceiver<Te>::CInterface* r = dynamic_cast<BB::CReceiver<Te>::CInterface*>(receiver);

		std::function<bool(const const std::shared_ptr<Te>)> f = std::bind(&BB::CReceiver<Te>::CInterface::Event, (BB::CReceiver<Te>::CInterface*)receiver, std::placeholders::_1);//original
		std::function<bool(const std::shared_ptr<CEvent>)> f_cvoidp = *(reinterpret_cast<std::function<bool(const std::shared_ptr<CEvent>)>*>(&f));//casted

		CEventSequencer* baseReceiver = (CEventSequencer*)receiver;

		t_EventSignalRegistry::iterator it = m_ReceiverFuncs.find(&typeid(Te));
		if (it != m_ReceiverFuncs.end())
		{
			for (auto sit : (*it).second)
			{
				if (sit.receiver == baseReceiver)
					return;
			}
			(*it).second.push_back(t_Signal(f_cvoidp, baseReceiver));
		}
		else
			m_ReceiverFuncs[&typeid(Te)].push_back(t_Signal(f_cvoidp, baseReceiver));

		m_ReceiverConnectionNameRegistry[baseReceiver] = connectionName;

		m_ReceiverEvents[&typeid(receiver)].push_back(&typeid(Te));
		const std::type_info* p = &typeid((void*)receiver);

		return;
	}

	template<typename Te>
	int64_t CEventManager::distributeEvent(const Te& newEvent)
	{
		if (m_DistributionAllowed == false)
			return 0;
		//TODO: must also make sure event is correct type
		//for all receivers in registry by event type
		//bind function pointer
		if (m_ReceiverFuncs[&typeid(Te)].size() > 0)
		{
			for (auto funcFromPair: m_ReceiverFuncs[&typeid(Te)])
			{
				std::shared_ptr<CEvent> event = std::make_shared<Te>(newEvent);
				m_ThreadPool.push(funcFromPair, event);
			}
			return  m_ReceiverFuncs[&typeid(Te)].size();
		}
		return 0;
	}

	template<typename Te>
	void CEventManager::handleAllEvents(eSolveFlags flags)
	{
		m_ThreadPool.handleAllEvents<Te>();
	}
	
}

#endif