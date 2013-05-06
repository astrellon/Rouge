#include "quest.h"

#include <base/handle.h>
using namespace am::base;

#include <util/data_table.h>
#include <util/data_boolean.h>

#include "loading_state.h"

namespace am {
namespace game {

	//Quest::QuestMap Quest::sQuestMap;
	const int Quest::LUA_ID = 0x0D;
	const char *Quest::LUA_TABLENAME = "am_game_Quest";

	Quest::Quest(const char *questId) :
		mQuestId(questId),
		mEnabled(true),
		mAcceptedReward(false),
		mCompleted(false),
		mTitle("No Title"),
		mDescription("No Description"),
		mActiveText("No Active Text")
	{

	}
	Quest::~Quest()
	{

	}

	void Quest::setEnabled(bool enabled)
	{
		mEnabled = enabled;
	}
	bool Quest::isEnabled() const
	{
		return mEnabled;
	}

	bool Quest::startQuest()
	{
		if (isCompleted())
		{
			return false;
		}
		Handle<Event> e(new Event("startQuest"));
		fireEvent<Event>(e);
		return true;
	}
	bool Quest::finishQuest()
	{
		if (isCompleted())
		{
			return false;
		}
		setCompleted(true);
		Handle<Event> e(new Event("finishQuest"));
		fireEvent<Event>(e);
		return true;
	}

	void Quest::setCompleted(bool completed)
	{
		mCompleted = completed;
	}
	bool Quest::isCompleted()
	{
		return mCompleted;
	}

	void Quest::setTitle(const char *title)
	{
		mTitle = title;
	}
	const char *Quest::getTitle() const
	{
		return mTitle.c_str();;
	}

	void Quest::setDescription(const char *desc)
	{
		mDescription = desc;
	}
	const char *Quest::getDescription() const
	{
		return mDescription.c_str();
	}

	void Quest::setActiveText(const char *text)
	{
		mActiveText = text;
	}
	const char *Quest::getActiveText() const
	{
		return mActiveText.c_str();
	}

	void Quest::setQuestId(const char *questId)
	{
		mQuestId = questId;
	}
	const char *Quest::getQuestId() const
	{
		return mQuestId.c_str();
	}

	void Quest::setAcceptedReward(bool accepted)
	{
		mAcceptedReward = accepted;
	}
	bool Quest::hasAcceptedReward() const
	{
		return mAcceptedReward;
	}

	data::IData *Quest::serialise()
	{
		data::Table *output = new data::Table();
		output->at("completed", mCompleted);
		output->at("acceptedReward", mAcceptedReward);
		output->at("enabled", mEnabled);

		IAttributeData::serialise(output);

		return output;
	}
	int Quest::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "quest"));
		if (!dataMap)
		{
			return 0;
		}

		Handle<data::Boolean> boo(dataMap->at<data::Boolean>("completed"));
		if (boo)
		{
			setCompleted(boo->boolean());
		}

		boo = dataMap->at<data::Boolean>("acceptedReward");
		if (boo)
		{
			setAcceptedReward(boo->boolean());
		}

		boo = dataMap->at<data::Boolean>("enabled");
		if (boo)
		{
			setEnabled(boo->boolean());
		}

		IAttributeData::deserialise(state, dataMap);
		return 1;
	}

}
}
