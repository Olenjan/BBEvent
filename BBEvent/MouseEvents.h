#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Event.h"

namespace BB
{
	namespace Mouse
	{
		enum t_MouseButton: int8_t
		{
			UNKNOWN = -1,
			LEFT = 0,
			RIGHT = 1,
			MIDDLE = 2, 
			WHEELUP = 3,
			WHEELDOWN = 4
		};

	}
	//CMouseStateChange: 0 = left, 1 = right, 2 = middle, 3 = whup, 4 = whdown (wheel true += +1 false += -1)
	//mouse position clamped between 0 and 1
	class CMouseStateChange : public CEvent
	{
	public:
		float x;
		float y;
		Mouse::t_MouseButton button = Mouse::t_MouseButton::UNKNOWN;
		bool state = false;

		CMouseStateChange()
			:	CMouseStateChange(Mouse::t_MouseButton::UNKNOWN, false, -1, -1)
		{
		}

		CMouseStateChange(Mouse::t_MouseButton key, bool state)
			:	CEvent(),
				button(key),
				state(state)
		{
		}

		CMouseStateChange(int64_t key, bool state)
			: CEvent(),
			button((Mouse::t_MouseButton)key),
			state(state)
		{
		}

		CMouseStateChange(Mouse::t_MouseButton key, bool state, float x, float y)
			:	CEvent(),
				button(key),
				state(state),
				x(x),
				y(y)
		{
		}

		CMouseStateChange(int64_t key, bool state, float x, float y)
			: CEvent(),
			button((Mouse::t_MouseButton)key),
			state(state),
			x(x),
			y(y)
		{
		}

		virtual ~CMouseStateChange()
		{
		}
	};

	class CMouseClickEvent : public CEvent
	{
	public:
		float x;
		float y;
	public:
		CMouseClickEvent()
			:	CMouseClickEvent(-1, -1)
		{
		}

		CMouseClickEvent(float x, float y)
			:	x(x),
				y(y)
		{
		}
	};

	class CMouseReleaseEvent : public CEvent
	{
	public:
		float x;
		float y;
	public:
		CMouseReleaseEvent()
			:	CMouseReleaseEvent(-1, -1)
		{
		}

		CMouseReleaseEvent(float x, float y)
			:	x(x),
				y(y)
		{
		}
	};

	//! Grab event
	//! location of grab
	class CMouseGrabEvent : public CEvent
	{
	public:
		float	x;
		float	y;
	public:
		CMouseGrabEvent()
			:	CMouseGrabEvent(-1, -1)
		{
		}

		CMouseGrabEvent(float x, float y)
			:	x(x),
				y(y)
		{
		}

	};
	//TODO:
}

#endif