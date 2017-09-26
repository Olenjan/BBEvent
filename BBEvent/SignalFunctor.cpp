#include "SignalFunctor.h"
#include <ctime>
namespace BB
{
	t_SignaledEventFunctor::t_SignaledEventFunctor()
		:	signal(),
			event(NULL)
	{
	}

	t_SignaledEventFunctor::t_SignaledEventFunctor(t_Signal s, std::shared_ptr<CEvent> e)
		:	signal(s),
			event(e)
	{
	}

	void t_SignaledEventFunctor::operator()()
	{
		if (event == NULL)
			return;
		if(signal.receiver->isSequential())
			signal.receiver->lockEvents();

		solved = signal.func(event);

		if (signal.receiver->isSequential())
			signal.receiver->unlockEvents();

		return;
	}
	bool t_SignaledEventFunctor::isValid()
	{
		return signal.isValid() && event != NULL;
	}
}