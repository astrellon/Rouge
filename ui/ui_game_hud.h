#pragma once

#include <base/handle.h>

#include <ui/ui_inspector.h>
#include <ui/ui_character_screen.h>
#include <ui/ui_component.h>
#include <ui/ui_dialogue_box.h>
#include <ui/ui_dialogue_choices.h>
#include <ui/ui_store_screen.h>

namespace am {
namespace ui {

	class GameHud : public UIComponent 
	{
	public:
		GameHud();
		~GameHud();

		virtual Inspector *getInspector() const;
		virtual CharacterScreen *getCharacterScreen() const;
		virtual DialogueBox *getDialogueBox() const;
		virtual DialogueChoices *getDialogueChoices() const;
		virtual StoreScreen *getStoreScreen() const;

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		base::Handle<Inspector> mInspector;
		base::Handle<CharacterScreen> mCharacterScreen;
		base::Handle<DialogueBox> mDialogueBox;
		base::Handle<DialogueChoices> mDialogueChoices;
		base::Handle<StoreScreen> mStoreScreen;
		
	};

}
}
