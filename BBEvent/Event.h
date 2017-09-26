#ifndef BB_EVENT_H
#define BB_EVENT_H

#include <inttypes.h>
#include <vector>
#include <string>
#include <map>
#include "SolveFlag.h"

namespace BB
{
	class CEvent
	{
	private:
		eSolveFlags m_SolveFlags = eSolveFlags::ANY;
		eSolveFlags m_SolveOrigin= eSolveFlags::NONE; //in Receiver::Handle, should never be NONE.

	public:
		bool valid = false;

		//! SolveFlags constructor, default value is eSolveFlags::ANY
		CEvent(eSolveFlags solveFlags);
		virtual ~CEvent();

		void setSolveOrigin(eSolveFlags flags);

		const eSolveFlags& getSolveOrigin() const;

		const eSolveFlags& getSolveFlags() const;

		void setSolveFlags(eSolveFlags solveFlags);
	};
}

#endif