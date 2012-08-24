#include "string_pool.h"

#include <sstream>

#include <game/engine.h>
#include <game/game.h>
#include <game/character.h>

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
			return string("GENDER");
		}
		if (strcmp(token, "race") == 0)
		{
			return string("RACE");
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

}
}
