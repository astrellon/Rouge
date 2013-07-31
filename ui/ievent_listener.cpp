#include "ievent_listener.h"

#include "event_interface.h"

namespace am {
namespace ui {

	IEventListener::~IEventListener()
	{
		ListeningList::iterator iter;
		ListeningList list = mListeningTo;
		for (ListeningList::iterator iter = list.begin(); iter != list.end(); ++iter)
		{
			(*iter)->removeEventListener(this);
		}
		mListeningTo.clear();
	}

	void IEventListener::addListeningTo(EventInterface *e)
	{
		if (e != nullptr)
		{
			mListeningTo.push_back(e);
		}
	}
	void IEventListener::removeListeningTo(EventInterface *e)
	{
		if (e != nullptr)
		{
			ListeningList::const_iterator iter;
			for (iter = mListeningTo.begin(); iter != mListeningTo.end(); ++iter)
			{
				if (*iter == e)
				{
					mListeningTo.erase(iter);
					break;
				}
			}
		}
	}
	const IEventListener::ListeningList &IEventListener::getListeningToList() const
	{
		return mListeningTo;
	}

}
}
