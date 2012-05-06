#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"

#include <iostream>

int main() {

	const char *input = "{\"name\": \"Alan\", \"age\": 5.6, hist:[1,2.3,{x:2,y:8,},'hello','there']}";
	JsonValue inputObj = JsonValue::import(input);

	//printf("Imported: %s, %f\n", inputObj["name"].getCStr(), inputObj["age"].getFloat());
	inputObj.display();
	std::cin.get();

	return 0;
}
