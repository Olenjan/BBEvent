#include "EventSequencer.h"

namespace BB
{

	CEventSequencer::CEventSequencer() {}
	CEventSequencer::~CEventSequencer()
	{
		m_SequentialityLock.lock();
		m_SequentialityLock.unlock();
	}

	bool CEventSequencer::isSequential()
	{
		return m_Sequential;
	}
	void CEventSequencer::setSequentiality(bool value)
	{
		m_Sequential = value;
	}
	CEventSequencer* CEventSequencer::getThis()
	{
		return this;
	}
	void CEventSequencer::lockEvents()
	{
		m_SequentialityLock.lock();
	}
	void CEventSequencer::unlockEvents()
	{
		m_SequentialityLock.unlock();
	}
	bool CEventSequencer::tryLockEvents()
	{
		return m_SequentialityLock.try_lock();
	}	
	
}