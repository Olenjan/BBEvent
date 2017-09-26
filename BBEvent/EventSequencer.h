#ifndef BB_RECEIVER_SYNCH_H
#define BB_RECEIVER_SYNCH_H

#include <mutex>

namespace BB
{
	//! Receiverbase must be inherited to any receiver, for thread safety.
	//! When enabled, only 1 thread can access receivers at the time
	class CEventSequencer
	{
	private:
		bool m_Sequential = true;
		std::mutex m_SequentialityLock;

	public:

		CEventSequencer();
		virtual ~CEventSequencer();

		bool isSequential();

		//! Sets event solving sequentiality either true or false
		//! When sequentiality is true, all events are solved one after the other and not parallel
		//! When sequentiality if false, all events are allowed to be solved parallel
		void setSequentiality(bool value);

		//	Returns CEventSequencer when data is ready
		//! Locks m_SequentialityLock, released when release called
		//! Returns NULL When data is not ready
		CEventSequencer* getThis();


		//! Locks the m_SequentialityLock
		void lockEvents();

		//! Unlocks the m_SequentialityLock
		void unlockEvents();

		//! Tries to lock the m_SequentialityLock, returns immediatly whether or not the locking was successful
		bool tryLockEvents();

	};
}

#endif