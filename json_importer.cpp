#ifndef _ANDROID
#	include "json_importer.h"
#endif

JsonValue JsonValue::JsonUndef;
map<const string *, int> JsonValue::sStrRefs;
map<const JsonObject *, int> JsonValue::sObjRefs;
map<const JsonArray *, int> JsonValue::sArrRefs;


JsonValue JsonImporter::import(const char *str) {
	Tokeniser tokeniser(str);
	return import_object(tokeniser);
}

JsonValue JsonImporter::import_object(Tokeniser &tokeniser, const char *startToken) {
	const char *token = startToken;
	if (token == NULL) {
		token = tokeniser.nextToken();
	}
	if (token == NULL) {
		return JsonValue();
	}
		
	char ch = getChar(token);
	if (ch == '{') {
		token = tokeniser.nextToken();
		ch = getChar(token);
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
			ch = getChar(token);
			if (ch == ',') {
				token = tokeniser.nextToken();
				ch = getChar(token);
			}
		}

		return JsonValue(obj);
	}
	else if(ch == '[') {

		token = tokeniser.nextToken();
		ch = getChar(token);

		JsonArray *arr = new JsonArray();

		while(token != NULL && ch != ']') {
			arr->push_back(import_object(tokeniser, token));

			token = tokeniser.nextToken();
			ch = getChar(token);
			if (ch == ',') {
				token = tokeniser.nextToken();
				ch = getChar(token);
			}
		}

		return JsonValue(arr);
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

char JsonImporter::getChar(const char *token) {
	if (token != NULL) {
		return token[0];
	}
	return '\0';
}
