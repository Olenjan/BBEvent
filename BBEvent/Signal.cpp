#include "Signal.h"

namespace BB
{
	t_Signal::t_Signal()
	{
	}

	t_Signal::t_Signal(std::function<bool(const std::shared_ptr<CEvent>)> func, CEventSequencer* r)
		:	func(func),
			receiver(r) {}
	bool t_Signal::isValid()
	{
		return (func != NULL && receiver != NULL);
	}
}