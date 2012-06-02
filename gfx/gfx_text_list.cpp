#include "gfx_text_list.h"

#include "gfx_font.h"

#include "../gl.h"

namespace am {
namespace gfx {

	TextList::TextList(GfxEngine *engine) :
		TextField(engine),
		mMaxEntries(100),
		mListUpwards(false)
	{

	}
	TextList::~TextList()
	{

	}

	void TextList::addEntry(const char *text, const char *type)
	{
		if (type == NULL)
		{
			type = "";
		}
		if (static_cast<int>(mEntries.size()) + 1 > mMaxEntries)
		{
			mEntries.erase(mEntries.begin());
		}
		mEntries.push_back(Entry(text, type));
	}

	void TextList::setMaxEntries(int entries)
	{
		if (entries < 1)
		{
			entries = 1;
		}
		if (entries < static_cast<int>(mEntries.size()))
		{
			int diff = mEntries.size() - entries;
			mEntries.erase(mEntries.begin(), mEntries.begin() + diff);
		}
		mMaxEntries = entries;
	}
	int TextList::getMaxEntries() const
	{
		return mMaxEntries;
	}

	void TextList::setListUpwards(bool upwards)
	{
		mListUpwards = upwards;
	}
	bool TextList::getListUpwards() const
	{
		return mListUpwards;
	}

	void TextList::setFilter(const FilterList &filter)
	{
		mFilterList = filter;
		mFilterMap.clear();

		FilterList::const_iterator iter;
		for (iter = filter.begin(); iter != filter.end(); ++iter)
		{
			mFilterMap[*iter] = true;
		}
	}
	TextList::FilterList TextList::getFilter() const
	{
		return mFilterList;
	}

	void TextList::render(float dt)
	{
		if (!mVisible || mFont == NULL || !mFont->isLoaded())
		{
			return;
		}

		preRender();

		string sep = ": ";

		for (int i = mEntries.size() - 1; i >= 0; i--)
		{
			const Entry &entry = mEntries[i];
			if (entry.second.size() > 0)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				renderText(entry.second);
				glColor3f(1.0f, 1.0f, 1.0f);
				renderText(sep);
			}
			renderText(entry.first);

			newLine();
		}

		renderText(mText);
		postRender();
	}

}
}
