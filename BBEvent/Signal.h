#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include "Event.h"
#include "EventSequencer.h"

namespace BB
{
	//! Signals is a pair of function and receiver
	struct t_Signal
	{
		std::function<bool(const std::shared_ptr<CEvent>)> func = NULL;
		CEventSequencer* receiver = NULL;

		t_Signal();
		t_Signal(std::function<bool(const std::shared_ptr<CEvent>)> func, CEventSequencer* r);

		bool isValid();
	};
}

#endif