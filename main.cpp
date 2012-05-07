#define _ANDROID

#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"

#include <iostream>

int main() {

	const char *input = "{ /*Comment for this object*/"
		"name: 'Alan', "
		"age: 5.6, "
		"hist:["
			"1,2.3,{ /* position */ x:2,y:8,},'hello','there'"
		"],"
		"}";
	JsonValue inputObj = JsonValue::import(input);

	inputObj.display();
	std::cin.get();

	return 0;
}
