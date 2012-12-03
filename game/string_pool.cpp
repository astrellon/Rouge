#include "string_pool.h"

#include <sstream>

#include <game/engine.h>
#include <game/game.h>
#include <game/character.h>
#include <game/race.h>

#include <log/logger.h>

namespace am {
namespace game {

	string StringPool::replace(const char *str)
	{
		if (str == NULL || str[0] == '\0')
		{
			return string("");
		}
		Tokeniser tokeniser(str);
		const char *token = tokeniser.nextToken();
		if (token == NULL)
		{
			return string("");
		}
		if (strcmp(token, "char") == 0)
		{
			token = tokeniser.nextToken();
			if (token == NULL)
			{
				return string("{unfinished token 'player'}");
			}
			if (strcmp(token, "main") == 0)
			{
				Game *game = Engine::getEngine()->getCurrentGame();
				if (game == NULL)
				{
					return string("{no current game for main player}");
				}
				Character *mainChar = game->getMainCharacter();
				if (mainChar == NULL)
				{
					return string("{no main character}");
				}
				return replaceCharacter(tokeniser, mainChar);
			}
		}
		stringstream ss;
		ss << "{unknown replace token '" << token << "'}";
		return ss.str();
	}

	string StringPool::replaceCharacter(Tokeniser &tokeniser, Character *character)
	{
		const char *token = tokeniser.nextToken();
		if (token == NULL || strcmp(token, "name") == 0)
		{
			return string(character->getName());
		}
		if (strcmp(token, "gender") == 0)
		{
			return string(Gender::getGenderName(character->getGender()));
		}
		if (strcmp(token, "race") == 0)
		{
			Race *race = character->getRace();
			if (race == NULL)
			{
				race = Engine::getEngine()->getUnknownRace();
			}
			return string(race->getRaceName());
		}
		if (strcmp(token, "equip") == 0)
		{
			token = tokeniser.nextToken();
			if (token == NULL)
			{
				return string("{need bodypart to get equipped}");
			}
			Item *equipped = character->getEquipped(token);
			if (equipped == NULL)
			{
				return ("nothing");
			}
			else
			{
				return string(equipped->getFullItemName());
			}
		}
		if (strcmp(token, "stat") == 0)
		{
			token = tokeniser.nextToken();
			if (token == NULL)
			{
				return string("{need stat name}");
			}
			bool getBase = false;
			if (strcmp(token, "base") == 0)
			{
				getBase = true;
				token = tokeniser.nextToken();
				if (token == NULL)
				{
					return string("{need a stat name after flagging base stat}");
				}
			}
			Stat::StatType stat = Stat::getStatType(token);
			if (stat == Stat::MAX_STAT_LENGTH)
			{
				stringstream ss;
				ss << "{unknown stat '" << token << "'}";
				return ss.str();
			}
			stringstream ss;
			if (getBase)
			{
				ss << character->getStats().getBaseStat(stat);
			}
			else
			{
				ss << character->getStats().getStat(stat);
			}
			return ss.str();
		}
		stringstream ss;
		ss << "{unknown character token '" << token << "'}";
		return ss.str();
	}

	string StringPool::filterText(const string &str)
	{
		size_t index = str.find("${");
		if (index == string::npos)
		{
			return str;
		}
		string result = str;
		while (index != string::npos)
		{
			if (index == 0 || (index > 0 && str[index - 1] != '\\'))
			{
				index += 2;
				size_t end = result.find("}", index);
				if (end != string::npos)
				{
					string sub = result.substr(index, end - index);
					string replaced = replace(sub.c_str());
					result.replace(index - 2, end - index + 3, replaced.c_str());
				}
				index = result.find("${", index + 1);
			}
		}
		return result;
	}

}
}
