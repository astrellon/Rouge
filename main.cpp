#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"

#include <iostream>

int main() {
	
	JsonValue i = 5;
	JsonValue f = 3.4f;
	string ss = "Hello";
	JsonValue s = ss;

	{
	JsonObject *obj = new JsonObject();
	JsonValue o(obj);
	(*obj)["name"] = "Alan";
	(*obj)["age"] = 23.4f;
	
	printf("Object: %s, %f\n", o["name"].getCStr(), o["age"].getFloat());
	}
	/*Tokeniser tok("{\"name\": \"Alan\", \"age\": 5.6}");
	const char *token = tok.nextToken();
	while(token != NULL) {
		printf("Token: >%s<\n", token);
		token = tok.nextToken();
	}*/

	std::cin.get();

	return 0;
}
