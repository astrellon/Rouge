#pragma once

#include <string>
#include <map>
using namespace std;

namespace am {
namespace util {

	class Selector {
	public:

		typedef map<string, string> AttributeMap;
		
		Selector();
		Selector(const char *id);
		~Selector();

		virtual bool setId(const char *type);
		virtual const char *getId() const;

		virtual AttributeMap &getAttributes();

		virtual bool setAttribute(const char *name, const char *value);
		virtual bool removeAttribute(const char *name);
		virtual const char *getAttribute(const char *name) const;

		virtual int match(const Selector &node) const;

		virtual bool operator == (const Selector &rhs) const;
		virtual bool operator != (const Selector &rhs) const;

	protected:

		string mId;
		AttributeMap mAttributes;

	};

}
}
