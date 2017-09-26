#ifndef BB_QUEUED_RECEIVER_H
#define BB_QUEUED_RECEIVER_H

#include "Receiver.h"

namespace BB
{
	//! CUnorderedQueue is intended for solving of events sequentially without locking of  CEventSequencer.
	//!		In the case of CEventSequencer::m_SequentialityLock being disabled it provides way of sequencing event calls
	//! EventManager::handleAllEvents<T>(eSolveFlags::ANY). But without locking CEventSequencer
	template<class E>
	class CReceiver<E>::CUnorderedQueue : public CInterface
	{
		std::mutex m_Mutex;
		std::vector<std::shared_ptr<E>> m_EventQueue;
	public:
		std::vector<std::shared_ptr<E>> consume() { m_Mutex.lock(); std::vector<std::shared_ptr<E>> result = m_EventQueue; m_EventQueue.clear(); m_Mutex.unlock(); return result; }

		CUnorderedQueue() {}

	private:
		virtual void Event(std::shared_ptr<E> event) final;
	};

	template<class E>
	void CReceiver<E>::CUnorderedQueue::Event(std::shared_ptr<E> event)
	{
		m_Mutex.lock();
		m_EventQueue.push_back(event);
		//deletion of event should not happend, cannot store event pointer
		m_Mutex.unlock();
	}
}

#endif