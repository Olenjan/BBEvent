#ifndef ENTITY_CLICK_EVENT_H
#define ENTITY_CLICK_EVENT_H

#include "Event.h"

namespace BB
{
	class CEntityClickEvent: public CEvent
	{
	public:
		CEntityClickEvent(CEntity* entity): CEntity
		{

		}

	};
}

#endif