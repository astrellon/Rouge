#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using am::base::IManaged;

#include <vector>
using std::vector;

#include "event.h"
#include "mouse_event.h"
#include "keyboard_event.h"
#include "inventory_event.h"
#include "equip_event.h"
#include "value_change_event.h"
#include "dialogue_event.h"
#include "stat_event.h"
#include "list_event.h"
#include "item_event.h"
#include "game_object_event.h"

namespace am {
namespace ui {

	class EventInterface;

	class IEventListener : virtual public IManaged {
	public:

		typedef vector< Handle<EventInterface> > ListeningList;

		virtual ~IEventListener();

		virtual void onEvent(Event *e) {}
		virtual void onEvent(MouseEvent *e) {}
		virtual void onEvent(KeyboardEvent *e) {}
		virtual void onEvent(ValueChangeEvent *e) {}
		virtual void onEvent(ListEvent *e) {}
		
		virtual void onEvent(InventoryEvent *e) {}
		virtual void onEvent(EquipEvent *e) {}
		virtual void onEvent(DialogueEvent *e) {}
		virtual void onEvent(StatEvent *e) {}
		virtual void onEvent(ItemEvent *e) {}
		virtual void onEvent(GameObjectEvent *e) {}

		virtual bool compareListeners(const IEventListener *rhs) const { return this == rhs; }

		void addListeningTo(EventInterface *e);
		void removeListeningTo(EventInterface *e);
		const ListeningList &getListeningToList() const;

	protected:

		ListeningList mListeningTo;

	};

}
}
