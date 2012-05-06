#pragma once

#include "json_value.h"
#include "tokeniser.h"

#include <string>

using std::string;

class JsonImporter {
public:
	
	static JsonValue import(const char *str);

protected:
	
	static JsonValue import_object(Tokeniser &tokeniser, const char *startToken = NULL);

	static char getChar(const char *token);

};

#ifdef _ANDROID
#	include "json_importer.cpp"
#endif
