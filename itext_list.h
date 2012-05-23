#pragma once

#include "itext_field.h"

#include <vector>
#include <string>

using namespace std;

namespace am {
namespace gfx {

	class ITextList {
	public:
		virtual void addEntry(const char *text, const char *type = NULL) = 0;

		virtual void setMaxEntries(int entries) = 0;
		virtual int getMaxEntries() const = 0;

		virtual void setListUpwards(bool upwards) = 0;
		virtual bool getListUpwards() const = 0;

		virtual void setFilter(const vector<string> &filter) = 0;
		virtual vector<string> getFilter() const = 0;
	};

}
}
