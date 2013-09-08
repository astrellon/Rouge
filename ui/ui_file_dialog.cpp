#include "ui_file_dialog.h"

#include <sstream>

#include <log/logger.h>
#include <util/utils.h>

namespace am {
namespace ui {

	FileDialog::FileDialog(const char *folder, const char *title) :
		Panel(),
		mFilename(new TextInput()),
		mFolder(folder)
	{
		setDialogTitle(title);

		mFilename->setSize(80.0f, 16.0f);
		mFilename->setParentOffset(15.0f, 25.0f);
		addChild(mFilename);

		mOk = new TextButton("ui:small_button", "Okay");
		mOk->setSize(60.0f, 20.0f);
		mOk->setParentAnchor(X_RIGHT, Y_BOTTOM);
		mOk->setAnchor(X_RIGHT, Y_BOTTOM);
		mOk->getLabelField()->getGfxComponent()->setColour(1, 1, 1, 1);
		mOk->setParentOffset(-15.0f, -15.0f);
		addChild(mOk);

		mCancel = new TextButton("ui:small_button", "Cancel");
		mCancel->setSize(60.0f, 20.0f);
		mCancel->setParentAnchor(X_RIGHT, Y_BOTTOM);
		mCancel->setAnchor(X_RIGHT, Y_BOTTOM);
		mCancel->getLabelField()->getGfxComponent()->setColour(1, 1, 1, 1);
		mCancel->setParentOffset(-80.0f, -15.0f);
		addChild(mCancel);

		mOk->addEventListener("click", this);
		mCancel->addEventListener("click", this);
	}
	FileDialog::~FileDialog()
	{
		mOk->removeEventListener("click", this);
		mCancel->removeEventListener("click", this);
	}

	void FileDialog::setDialogTitle(const char *title)
	{
		mDialogTitle = title;
		setTitle((mDialogTitle + ": " + mFolder).c_str());
	}
	const char *FileDialog::getDialogTitle() const
	{
		return mDialogTitle.c_str();
	}

	void FileDialog::setFolder(const char *folder)
	{
		mFolder = folder;
		setTitle((mDialogTitle + ": " + mFolder).c_str());
	}
	const char *FileDialog::getFolder() const
	{
		return mFolder.c_str();
	}

	void FileDialog::setFilename(const char *filename)
	{
		mFilename->setText(filename);
	}
	const char *FileDialog::getFilename() const
	{
		return mFilename->getText();
	}

	void FileDialog::onEvent(Event *e)
	{
		if (e->getEventTarget() == mOk)
		{
			Handle<Event> e(new Event("dialog_ok", this));
			fireEvent<Event>(e);
			setVisible(false);
		}
		else if (e->getEventTarget() == mCancel)
		{
			Handle<Event> e(new Event("dialog_cancel", this));
			fireEvent<Event>(e);
			setVisible(false);
		}
	}

	void FileDialog::setWidth(float width)
	{
		Panel::setWidth(width);
		mFilename->setWidth(width - 30.0f);
	}

}
}
