#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include "Event.h"

namespace BB
{	
	enum class eKeyEventType
	{
		NONE,
		PRESS,
		RELEASE
	};
	enum class eKeyCode
	{
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,

	};

	//keyboard
	class CKeyEvent : public CEvent
	{
	public:
		eKeyEventType m_Type;
		int64_t key = -1;
		bool state = false;
		CKeyEvent()
		{
		}

		virtual ~CKeyStateChange()
		{
		}

	};

}

#endif