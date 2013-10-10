#pragma once

// While it might be easier to organise things just into namespaces,
// it's probably best to keep things in order of Lua IDs.

// Game objects
#define LUA_ID_STATS			0x01
#define LUA_TABLE_STATS			"am_game_Stats"
#define LUA_ID_STATMODIFIERS	0x02
#define LUA_TABLE_STATMODIFIERS	"am_game_StatModifiers";
#define LUA_ID_BODYPART			0x03
#define LUA_TABLE_BODYPART		"am_game_BodyPart"
#define LUA_ID_CHARACTER		0x04
#define LUA_TABLE_CHARACTER		"am_game_Character"
#define LUA_ID_COINPURSE		0x05
#define LUA_TABLE_COINPURSE		"am_game_CoinPurse"
#define LUA_ID_ENGINE			0x06
#define LUA_TABLE_ENGINE		"am_game_Engine"
#define LUA_ID_GAME				0x07
#define LUA_TABLE_GAME			"am_game_Game"
#define LUA_ID_GAMEOBJECT		0x08
#define LUA_TABLE_GAMEOBJECT	"am_game_GameObject"
#define LUA_ID_INVENTORY		0x09
#define LUA_TABLE_INVENTORY		"am_game_Inventory"
#define LUA_ID_INVENTORYSPOT	0x0A
#define LUA_TABLE_INVENTORYSPOT	"am_game_InventorySpot"
#define LUA_ID_ITEM				0x0B
#define LUA_TABLE_ITEM			"am_game_Item"
#define LUA_ID_MAP				0x0C
#define LUA_TABLE_MAP			"am_game_Map"
#define LUA_ID_QUEST			0x0D
#define LUA_TABLE_QUEST			"am_game_Quest"
#define LUA_ID_RACE				0x0E
#define LUA_TABLE_RACE			"am_game_Race"
#define LUA_ID_TILE				0x0F
#define LUA_TABLE_TILE			"am_game_Tile"
#define LUA_ID_TILEINSTANCE		0x10
#define LUA_TABLE_TILEINSTANCE	"am_game_TileInstance"
#define LUA_ID_TILESET			0x11
#define LUA_TABLE_TILESET		"am_game_TileSet"
#define LUA_ID_TILETYPE			0x12
#define LUA_TABLE_TILETYPE		"am_game_TileType"
#define LUA_ID_DIALOGUE			0x14
#define LUA_TABLE_DIALOGUE		"am_game_Dialogue"
#define LUA_ID_DIALOGUECOMP		0x15
#define LUA_TABLE_DIALOGUECOMP	"am_game_DialogueComponent"
#define LUA_ID_STATMODIFIER		0x16
#define LUA_TABLE_STATMODIFIER	"am_game_StatModifier"
#define LUA_ID_BODYPARTS		0x17
#define LUA_TABLE_BODYPARTS		"am_game_BodyParts"
#define LUA_ID_DOOR				0x18
#define LUA_TABLE_DOOR			"am_game_Door"
#define LUA_ID_MAPREGION		0x19
#define LUA_TABLE_MAPREGION		"am_game_MapRegion"
#define LUA_ID_STORE			0x1A
#define LUA_TABLE_STORE			"am_game_Store"
// Utils
#define LUA_ID_TABLE			0x40
#define LUA_TABLE_TABLE			"am_util_data_Table"
// Graphics
#define LUA_ID_SPRITE			0x50
#define LUA_TABLE_SPRITE		"am_gfx_Sprite"
#define LUA_ID_ASSET			0x51
#define LUA_TABLE_ASSET			"am_gfx_Asset"
#define LUA_ID_TEXTURE			0x52
#define LUA_TABLE_TEXTURE		"am_gfx_Texture"
#define LUA_ID_GFXENGINE		0x53
#define LUA_TABLE_GFXENGINE		"am_gfx_Engine"
#define LUA_ID_FONT				0x54
#define LUA_TABLE_FONT			"am_gfx_Font"
#define LUA_ID_TEXTSTYLE		0x55
#define LUA_TABLE_TEXTSTYLE		"am_gfx_TextStyle"
