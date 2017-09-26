#include "Event.h"

namespace BB
{
	CEvent::CEvent(eSolveFlags solveFlags)
		: m_SolveFlags(solveFlags)
	{
	}
	CEvent::~CEvent()
	{}

	void CEvent::setSolveOrigin(eSolveFlags flags)
	{
		m_SolveOrigin = flags;
	}

	const eSolveFlags& CEvent::getSolveOrigin() const
	{
		return m_SolveOrigin;
	}

	const eSolveFlags& CEvent::getSolveFlags() const
	{
		return m_SolveFlags;
	}
	void CEvent::setSolveFlags(eSolveFlags solveFlags)
	{
		m_SolveFlags = solveFlags;
	}
}