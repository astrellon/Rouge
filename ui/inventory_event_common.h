#pragma once

namespace am {
namespace ui {

    class Inventory
    {
    public:
    	enum EventType 
	    {
		    INVENTORY_ADD, INVENTORY_REMOVE, INVENTORY_BEFORE_ADD, 
            INVENTORY_BEFORE_REMOVE
    	};

    	static const char *EventTypeName[];
    };

}
}

