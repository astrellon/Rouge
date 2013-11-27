#pragma once

#include <base/handle.h>

#include <ui/ui_inspector.h>
#include <ui/ui_character_screen.h>
#include <ui/ui_component.h>
#include <ui/ui_dialogue_box.h>
#include <ui/ui_dialogue_choices.h>

namespace am {
namespace ui {

	class GameHud : public UIComponent {
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

		am::base::Handle<Inspector> mInspector;
		am::base::Handle<CharacterScreen> mCharacterScreen;
		am::base::Handle<DialogueBox> mDialogueBox;
		am::base::Handle<DialogueChoices> mDialogueChoices;
		
	};

}
}
