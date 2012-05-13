#pragma once

#include <string>

using namespace std;

namespace am {
namespace ui {

	class Font;

	class TextField {
	public:
		TextField();
		TextField(Font *font);
		~TextField();

		Font *getFont();
		void setFont(Font *font);

		void setText(string &str);
		void appendText(string &str);
		string getText();

		void render(int x, int y);

	protected:
		string mText;
		Font *mFont;
	};

}
}
