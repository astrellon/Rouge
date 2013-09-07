#pragma once

#include <base/handle.h>
using namespace am::base;

#include <vector>
#include <map>
#include <string>
using namespace std;

#include <ui/ui_panel.h>
#include <ui/ui_text_input.h>
#include <ui/ui_text_button.h>

namespace am {
namespace ui {

	class FileDialog : public Panel {
	public:
		FileDialog(const char *folder, const char *title);
		~FileDialog();

		void setFolder(const char *folder);
		const char *getFolder() const;

		void setDialogTitle(const char *title);
		const char *getDialogTitle() const;

		virtual void setWidth(float width);
		//virtual void setHeight(float height);

		//virtual void onEvent(MouseEvent *e);
		virtual void onEvent(Event *e);

	protected:

		Handle<TextInput> mFilename;
		Handle<TextButton> mOk;
		Handle<TextButton> mCancel;

		string mFolder;
		string mDialogTitle;

	};

}
}
