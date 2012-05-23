#include "gl_text_list.h"

#include "gl_font.h"

#include "gl.h"

namespace am {
namespace gfx {

	GlTextList::GlTextList(GlGfxEngine *engine) :
		GlTextField(engine),
		mMaxEntries(100),
		mListUpwards(false)
	{

	}
	GlTextList::~GlTextList()
	{

	}

	void GlTextList::addEntry(const char *text, const char *type)
	{
		if (type == NULL)
		{
			type = "";
		}
		if (mEntries.size() + 1 > mMaxEntries)
		{
			mEntries.erase(mEntries.begin());
		}
		mEntries.push_back(Entry(text, type));
	}

	void GlTextList::setMaxEntries(int entries)
	{
		if (entries < 1)
		{
			entries = 1;
		}
		if (entries < mEntries.size())
		{
			int diff = mEntries.size() - entries;
			mEntries.erase(mEntries.begin(), mEntries.begin() + diff);
		}
		mMaxEntries = entries;
	}
	int GlTextList::getMaxEntries() const
	{
		return mMaxEntries;
	}

	void GlTextList::setListUpwards(bool upwards)
	{
		mListUpwards = upwards;
	}
	bool GlTextList::getListUpwards() const
	{
		return mListUpwards;
	}

	void GlTextList::setFilter(const FilterList &filter)
	{
		mFilterList = filter;
		mFilterMap.clear();

		FilterList::const_iterator iter;
		for (iter = filter.begin(); iter != filter.end(); ++iter)
		{
			mFilterMap[*iter] = true;
		}
	}
	GlTextList::FilterList GlTextList::getFilter() const
	{
		return mFilterList;
	}

	void GlTextList::render(float dt)
	{
		if (mFont == NULL || !mFont->isLoaded())
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
