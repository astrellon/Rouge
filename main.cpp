//#define _ANDROID

#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"
#include "game.h"

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

	am::base::Game game;
	int loadResult = game.loadMap("test.ssff");
	if (loadResult != 0) {
		printf("Unable to load map! %d\n", loadResult);
	}

	std::cin.get();

	return 0;
}
