#pragma once

#include "json_value.h"
#include "tokeniser.h"

#include <string>

using std::string;

class JsonImporter {
public:
	
	static JsonValue import(const char *str) {
		Tokeniser tokeniser(str);
		return import_object(tokeniser);
	}

protected:
	
	static JsonValue import_object(Tokeniser &tokeniser) {
		const char *token = tokeniser.nextToken();
		if (token == NULL) {
			return JsonValue();
		}
		//string str = token;
		//string str;
		char ch = token[0];
		if (ch == '{') {
			token = tokeniser.nextToken();
			ch = token[0];
			string valueName;
			JsonObject *obj = new JsonObject();

			while (token != NULL && ch != '}') {
				if (ch == '"' || ch == '\'') {
					valueName = token + 1;
				}
				else {
					valueName = token;
				}
				// Should be a ':'
				tokeniser.nextToken();

				(*obj)[valueName] = import_object(tokeniser);

				token = tokeniser.nextToken();
				if (token != NULL) {
					ch = token[0];
					if (ch == ',') {
						token = tokeniser.nextToken();
						if (token != NULL) {
							ch = token[0];
						}
					}
				}
			}

			return JsonValue(obj);
		}
		else if(ch == '[') {

		}
		else if(ch == '"' || ch == '\'') {
			return JsonValue(token + 1);
		}
		else {
			string num = token;
			if (num.find('.') == string::npos) {
				int val;
				sscanf(num.c_str(), "%d", &val);
				return JsonValue(val);
			}
			else {
				float val;
				sscanf(num.c_str(), "%f", &val);
				return JsonValue(val);
			}
		}
		return JsonValue();
	}

};
