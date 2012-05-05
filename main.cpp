#include <stdio.h>
#include "tokeniser.h"
#include "json_value.h"
#include "json_importer.h"

#include <iostream>

int main() {

	const char *input = "{\"name\": \"Alan\", \"age\": 5.6}";
	JsonValue inputObj = JsonImporter::import(input);

	printf("Imported: %s, %f\n", inputObj["name"].getCStr(), inputObj["age"].getFloat());

	std::cin.get();

	return 0;
}
