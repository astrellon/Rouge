#pragma once

#include <map>

#include "gfx_text_field.h"

namespace am {
namespace gfx {

	class TextList : public TextField {
	public:
		typedef vector<string> FilterList;

		TextList();
		~TextList();

		void addEntry(const char *text, const char *type);

		void setMaxEntries(int entries);
		int getMaxEntries() const;

		void setListUpwards(bool upwards);
		bool getListUpwards() const;

		void setFilter(const FilterList &filter);
		FilterList getFilter() const;

		// Renderable methods
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
