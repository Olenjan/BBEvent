#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "Event.h"

namespace BB
{
	//WINDOW EVENTS
	class CResizeEvent : public CEvent
	{
	public:
		int m_Width;
		int m_Height;
		CResizeEvent()
			:	CResizeEvent(-1, -1)
		{
		}

		CResizeEvent(int w, int h)
			:	CEvent(),
				m_Width(w),
				m_Height(h)
		{
		}

		virtual ~CResizeEvent()
		{
		}
	};

	class CMaximizeEvent : public CEvent
	{
	public:
		CMaximizeEvent() : CEvent()
		{
		}

		virtual ~CMaximizeEvent()
		{
		}
	};
}

#endif