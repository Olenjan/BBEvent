#ifndef RECEIVER_H
#define RECEIVER_H

#include <mutex>
#include <iostream>
#include <type_traits>
#include <vector>

#include "Event.h"

namespace BB
{
	class CEventManager;
	template<class E>
	class CReceiver
	{
		friend CEventManager;//for access to CInterface from EventManager only
	protected:
		class CInterface
		{
			friend CEventManager;//For access to solve from EventManager only
		public:
			CInterface() = default;
			virtual ~CInterface() {}

		private:
			virtual bool Event(const std::shared_ptr<E>) = 0;
		};

	public:
		class CRecent;
		class CUnorderedQueue;
		class CMethod;
		class CUnorderedParallel;
	};
}
#include "EventManager.h"
#endif
		/*
	public:

		//RECENT RECEIVER
		class CRecent : public CReceiverInterface<E>
		{
		private:
			E eventData;
			bool m_Updated = false;
			std::mutex m_Mutex;
		public:
			CRecent() : CReceiverInterface<E>() { static_assert(std::is_base_of<CEvent, E>(), "E is not an event"); }

			//! \Returns true when event has been updated
			bool updated()
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				return m_Updated;
			}

			E consume()
			{
				E result;
				m_Mutex.lock();
				if (eventData.valid)
				{
					result = eventData;
					eventData.valid = false;
				}
				m_Updated = false;
				m_Mutex.unlock();
				return result;
			}
			virtual void Handle(const E* event) final;
		};

		void CRecentReceiver<E>::Handle(const E* event)
		{
			m_Mutex.lock();
			((CEvent*)event)->valid = true;
			eventData = *event;
			m_Updated = true;
			m_Mutex.unlock();
		}

		//! QUEUED RECEIVER
		class CQueued : public CInterface<E>
		{
			std::mutex m_Mutex;
			std::vector<E> m_EventQueue;
		public:
			std::vector<E> consume() { m_Mutex.lock(); std::vector<E> result = m_EventQueue; m_EventQueue.clear(); m_Mutex.unlock(); return result; }

			CQueuedReceiver() {}
			virtual void Handle(const E* event) final;
		};
		template<class E>
		void CQueuedReceiver<E>::Handle(const E* event)
		{
			m_Mutex.lock();
			m_EventQueue.push_back(*event);
			m_Mutex.unlock();
		}

		//functionalReceiver
		class CFunctionalEvent : public CReceiverInterface<E>
		{
		private:
			std::mutex m_Mutex;
		public:

			CFunctionalEvent() {};
			//should only be called out by other threads
			virtual void Handle(const E* event) final
			{
				Event(event);
			}

			//! Overload this function for classes where threaded functionality is needed
			virtual void Event(const E* event) = 0;

		};
	};
}
*/
	/*

	//! Receiver, overload of Handle mandatory
	template<class E>
	class CReceiverInterface
	{
	public:
		virtual ~CReceiverInterface() {}
		//! Lock is automatically handled by signal functor in signal.h
		virtual void Handle(const E* event) = 0;	
	};
	
	//! Handle-less receiver
	//! Sets event as internal data that is ready to be consumed
	//! For each received, a copy of same data is received
	//! Does not ensure event order, most recent event is used
	template<class E = CEvent>
	class CRecentReceiver : public CReceiverInterface<E>
	{
	private:
		E eventData;
		bool m_Updated = false;
		std::mutex m_Mutex;
	public:
		CRecentReceiver() : CReceiverInterface<E>() { static_assert(std::is_base_of<CEvent, E>(), "E is not an event"); }

		//! \Returns true when event has been updated
		bool updated()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Updated;
		}

		E consume()
		{
			E result;
			m_Mutex.lock();
			if (eventData.valid)
			{
				result = eventData;
				eventData.valid = false;
			}
			m_Updated = false;
			m_Mutex.unlock();
			return result;
		}
		virtual void Handle(const E* event) final;
	};

	template<class E>
	void CRecentReceiver<E>::Handle(const E* event)
	{
		m_Mutex.lock();
		((CEvent*)event)->valid = true;
		eventData = *event;
		m_Updated = true;
		m_Mutex.unlock();
	}

	//! Handle-less receiver
	//! Sets event to queue to internal data that is ready to be consumed
	//! On consumption, all events are cleared for given received
	//! For each receiver, a copy of same data is received
	//! Does not ensure event order
	template<class E>
	class CQueuedReceiver : public CReceiverInterface<E>
	{
		std::mutex m_Mutex;
		std::vector<E> m_EventQueue;
	public:
		std::vector<E> consume() { m_Mutex.lock(); std::vector<E> result = m_EventQueue; m_EventQueue.clear(); m_Mutex.unlock(); return result; }

		CQueuedReceiver() {}
		virtual void Handle(const E* event) final;
	};
	

	template<class E>
	void CQueuedReceiver<E>::Handle(const E* event)
	{
		m_Mutex.lock();
		m_EventQueue.push_back(*event);
		m_Mutex.unlock();
	}

	//class overloaded functionality receiver
	template<class E>
	class CFunctionalEvent : public CReceiverInterface<E>
	{
	private:
		std::mutex m_Mutex;
	public:

		CFunctionalEvent() {};
		//should only be called out by other threads
		virtual void Handle(const E* event) final
		{
			Event(event);
		}
		//! Overload this function for classes where threaded functionality is needed
		virtual void Event(const E* event) = 0;

	};
	
}


#endif
*/