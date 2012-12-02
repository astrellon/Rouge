#include "ievent_listener.h"

#include "event_interface.h"

namespace am {
namespace ui {

	IEventListener::~IEventListener()
	{
		ListeningList::iterator iter;
		/*for (iter = mListeningTo.begin(); iter != mListeningTo.end(); ++iter)
		{
			(*iter)->removeEventListener(this);
		}*/
		while (mListeningTo.size())
		{
			mListeningTo.back()->removeEventListener(this);
		}
		mListeningTo.clear();
	}

	void IEventListener::addListeningTo(EventInterface *e)
	{
		if (e != NULL)
		{
			mListeningTo.push_back(e);
		}
	}
	void IEventListener::removeListeningTo(EventInterface *e)
	{
		if (e != NULL)
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
