#pragma once

#include <base/handle.h>

#include <vector>
#include <map>
#include <string>

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

		std::string getFullPath() const;

		virtual void setWidth(float width);
		//virtual void setHeight(float height);

		//virtual void onEvent(MouseEvent *e);
		virtual void onEvent(Event *e);

	protected:

		base::Handle<TextInput> mFilename;
		base::Handle<TextButton> mOk;
		base::Handle<TextButton> mCancel;

		std::string mFolder;
		std::string mDialogTitle;

	};

}
}
