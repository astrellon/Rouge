#pragma once

#include <base/handle.h>

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

		void setFilename(const char *filename);
		const char *getFilename() const;

		string getFullPath() const;

		virtual void setWidth(float width);
		//virtual void setHeight(float height);

		//virtual void onEvent(MouseEvent *e);
		virtual void onEvent(Event *e);

	protected:

		am::base::Handle<TextInput> mFilename;
		am::base::Handle<TextButton> mOk;
		am::base::Handle<TextButton> mCancel;

		string mFolder;
		string mDialogTitle;

	};

}
}
