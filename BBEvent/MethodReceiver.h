#ifndef BB_METHOD_RECEIVER_H
#define BB_METHOD_RECEIVER_H

#include "Receiver.h"

namespace BB
{
	template<class E>
	class CReceiver<E>::CMethod: public CInterface
	{
	private:
		std::mutex m_Mutex;
	public:
		CMethod()
		{
		}

		//! Overload this function for classes where threaded functionality is needed
		virtual bool Event(std::shared_ptr<E> event) = 0;

	};
}

#endif