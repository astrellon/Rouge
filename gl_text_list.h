#pragma once

#include "itext_list.h"

#include <map>

#include "gl_text_field.h"

namespace am {
namespace gfx {

	class GlGfxEngine;

	class GlTextList : public GlTextField, public ITextList {
	public:
		typedef vector<string> FilterList;

		GlTextList(GlGfxEngine *engine);
		~GlTextList();

		// ITextList methods
		virtual void addEntry(const char *text, const char *type);

		virtual void setMaxEntries(int entries);
		virtual int getMaxEntries() const;

		virtual void setListUpwards(bool upwards);
		virtual bool getListUpwards() const;

		virtual void setFilter(const FilterList &filter);
		virtual FilterList getFilter() const;

		// GlTextField methods
		virtual void render(float dt);

	protected:

		typedef pair<string, string> Entry;
		typedef vector<Entry> Entries;
		Entries mEntries;

		int mMaxEntries;
		bool mListUpwards;

		FilterList mFilterList;
		
		typedef map<string, bool> FilterMap;
		FilterMap mFilterMap;
	};

}
}
