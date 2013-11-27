#pragma once

#include <string>
#include <map>

namespace am {
namespace gfx {

	class TextStyleSelector {
	public:

		enum Flag 
		{
			NONE, HOVER, DOWN
		};

		typedef std::map<std::string, std::string> AttributeMap;
		typedef std::map<std::string, bool> ClassMap;

		TextStyleSelector();
		TextStyleSelector(const char *nodeType);
		~TextStyleSelector();

		virtual bool setNodeType(const char *type);
		virtual const char *getNodeType() const;

		virtual bool addClass(const char *klass);
		virtual bool removeClass(const char *klass);
		virtual bool hasClass(const char *klass) const;
		virtual ClassMap &getClasses();

		virtual AttributeMap &getAttributes();

		virtual bool setAttribute(const char *name, const char *value);
		virtual bool removeAttribute(const char *name);
		virtual const char *getAttribute(const char *name) const;

		virtual bool setFlag(Flag flag);
		virtual Flag getFlag() const;

		virtual int match(const TextStyleSelector &node) const;

		virtual bool operator == (const TextStyleSelector &rhs) const;
		virtual bool operator != (const TextStyleSelector &rhs) const;

	protected:

		int mPosition;
		bool mPopFlag;
		std::string mNodeType;
		Flag mFlag;
		AttributeMap mAttributes;
		ClassMap mClasses;

	};

}
}
