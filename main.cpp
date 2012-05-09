//#define _ANDROID

#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"
#include "game.h"
#include "engine.h"

#include <iostream>

int main() {

	// const char *input = "{ /*Comment for this object*/"
	// 	"name: 'Alan', "
	// 	"age: 5.6, "
	// 	"hist:["
	// 		"1,2.3,{ /* position */ x:2,y:8,},'hello','there'"
	// 	"],"
	// 	"}";
	// am::util::JsonValue inputObj = am::util::JsonValue::import(input);

	//inputObj.display();

	am::base::Engine *engine = new am::base::Engine();
	int tileResult = engine->registerTiles("tileDefs.ssff");
	if (tileResult != 0) {
		printf("Unable to load tile definitions!\n");
		delete engine;

		std::cin.get();
		return -1;
	}

	am::base::Game *game = engine->createGame();
	int loadResult = game->loadMap("test.ssff");
	if (loadResult != 0) {
		printf("Unable to load map! %d\n", loadResult);
	}

	delete game;
	delete engine;

	std::cin.get();

	return 0;
}
