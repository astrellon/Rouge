#pragma once

#include <string>
#include <vector>
#include <map>

namespace am {
namespace game {

	class DialogueChoice
	{
	public:

		typedef std::map<std::string, std::string> Attributes;

		DialogueChoice(const char *text);
		~DialogueChoice();

		void setText(const char *text);
		const char *getText() const;

		void setAttribute(const char *attribute, const char *value = nullptr);
		const char *getAttribute(const char *attribute) const;
		bool hasAttribute(const char *attribute) const;
		bool removeAttribute(const char *attribute);
		void clearAttributes();

		const Attributes &getAttributes() const;

	protected:

		std::string mText;
		Attributes mAttributes;

	};

}
}
