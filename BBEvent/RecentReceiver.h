#ifndef BB_RECENT_RECEIVER_H
#define BB_RECENT_RECEIVER_H

#include "Receiver.h"

namespace BB
{
	template <class E>
	class CReceiver<E>::CRecent : public CInterface
	{
	private:
		E eventData;
		bool m_Updated = false;
		std::mutex m_Mutex;
	public:
		CRecent() : CInterface()
		{
			static_assert(std::is_base_of<CEvent, E>(), "E is not an event");
		}

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
		virtual bool Event(const std::shared_ptr<E> event) final;
	};

	template<class E>
	bool CReceiver<E>::CRecent::Event(const std::shared_ptr<E> event)
	{
		m_Mutex.lock();
		((CEvent*)event)->valid = true;
		eventData = *event;
		m_Updated = true;
		m_Mutex.unlock();
		return true;
	}

}

#endif