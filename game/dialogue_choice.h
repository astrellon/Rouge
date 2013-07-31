#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace am {
namespace game {

	class DialogueChoice {
	public:

		typedef map<string, string> Attributes;

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

		string mText;
		Attributes mAttributes;

	};

}
}
