#ifndef SIGNAL_FUNCTOR_H
#define SIGNAL_FUNCTOR_H

#include "Signal.h"
#include "Event.h"

namespace BB
{
	//! Purpose of this functor is to run jobs thread safe, mutexed by CEventSequencer when enabled
	struct t_SignaledEventFunctor
	{
		bool solved;
		t_Signal signal;
		std::shared_ptr<CEvent> event = NULL;

		t_SignaledEventFunctor();
		t_SignaledEventFunctor(t_Signal s, std::shared_ptr<CEvent> e);


		void t_SignaledEventFunctor::operator()();

		bool isValid();
	};
}

#endif