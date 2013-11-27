#include "string_pool.h"

#include <sstream>

#include <game/engine.h>
#include <game/game.h>
#include <game/character.h>
#include <game/race.h>

#include <log/logger.h>

namespace am {
namespace game {

	std::string StringPool::replace(const char *str)
	{
		if (str == nullptr || str[0] == '\0')
		{
			return std::string("");
		}
		Tokeniser tokeniser(str);
		const char *token = tokeniser.nextToken();
		if (token == nullptr)
		{
			return std::string("");
		}
		if (strcmp(token, "char") == 0)
		{
			token = tokeniser.nextToken();
			if (token == nullptr)
			{
				return std::string("{unfinished token 'player'}");
			}
			if (strcmp(token, "main") == 0)
			{
				Game *game = Engine::getEngine()->getCurrentGame();
				if (game == nullptr)
				{
					return std::string("{no current game for main player}");
				}
				Character *mainChar = game->getMainCharacter();
				if (mainChar == nullptr)
				{
					return std::string("{no main character}");
				}
				return replaceCharacter(tokeniser, mainChar);
			}
		}
		std::stringstream ss;
		ss << "{unknown replace token '" << token << "'}";
		return ss.str();
	}

	std::string StringPool::replaceCharacter(Tokeniser &tokeniser, Character *character)
	{
		const char *token = tokeniser.nextToken();
		if (token == nullptr || strcmp(token, "name") == 0)
		{
			return std::string(character->getName());
		}
		if (strcmp(token, "gender") == 0)
		{
			return std::string(Gender::getGenderName(character->getGender()));
		}
		if (strcmp(token, "race") == 0)
		{
			Race *race = character->getRace();
			if (race == nullptr)
			{
				race = Engine::getEngine()->getUnknownRace();
			}
			return string(race->getRaceName());
		}
		if (strcmp(token, "equip") == 0)
		{
			token = tokeniser.nextToken();
			if (token == nullptr)
			{
				return std::string("{need bodypart to get equipped}");
			}
			Item *equipped = character->getEquipped(token);
			if (equipped == nullptr)
			{
				return std::string("nothing");
			}
			else
			{
				return std::string(equipped->getFullItemName());
			}
		}
		if (strcmp(token, "stat") == 0)
		{
			token = tokeniser.nextToken();
			if (token == nullptr)
			{
				return std::string("{need stat name}");
			}
			bool getBase = false;
			if (strcmp(token, "base") == 0)
			{
				getBase = true;
				token = tokeniser.nextToken();
				if (token == nullptr)
				{
					return std::string("{need a stat name after flagging base stat}");
				}
			}
			Stat::StatType stat = Stat::getStatType(token);
			if (stat == Stat::MAX_STAT_LENGTH)
			{
				std::stringstream ss;
				ss << "{unknown stat '" << token << "'}";
				return ss.str();
			}
			std::stringstream ss;
			if (getBase)
			{
				ss << character->getStats()->getBaseStat(stat);
			}
			else
			{
				ss << character->getStats()->getStat(stat);
			}
			return ss.str();
		}
		std::stringstream ss;
		ss << "{unknown character token '" << token << "'}";
		return ss.str();
	}

	std::string StringPool::filterText(const std::string &str)
	{
		size_t index = str.find("${");
		if (index == string::npos)
		{
			return str;
		}
		std::string result = str;
		while (index != string::npos)
		{
			if (index == 0 || (index > 0 && str[index - 1] != '\\'))
			{
				index += 2;
				size_t end = result.find("}", index);
				if (end != string::npos)
				{
					std::string sub = result.substr(index, end - index);
					std::string replaced = replace(sub.c_str());
					result.replace(index - 2, end - index + 3, replaced.c_str());
				}
				index = result.find("${", index + 1);
			}
		}
		return result;
	}

}
}
