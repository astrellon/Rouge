#pragma once

#include <base/handle.h>

#include <ui/ui_inspector.h>
#include <ui/ui_character_screen.h>
#include <ui/ui_component.h>
#include <ui/ui_dialogue_box.h>
#include <ui/ui_dialogue_choices.h>

namespace am {
namespace ui {

	class GameHud : public UIComponent 
	{
	public:
		GameHud();
		~GameHud();

		virtual Inspector *getInspector();
		virtual CharacterScreen *getCharacterScreen();
		virtual DialogueBox *getDialogueBox();
		virtual DialogueChoices *getDialogueChoices();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		base::Handle<Inspector> mInspector;
		base::Handle<CharacterScreen> mCharacterScreen;
		base::Handle<DialogueBox> mDialogueBox;
		base::Handle<DialogueChoices> mDialogueChoices;
		
	};

}
}
