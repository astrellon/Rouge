#include "json_value.h"

#include <sstream>
#include <streambuf>
#include <fstream>

#include <log/logger.h>

#include "utils.h"

namespace am {
namespace util {

	JsonValue JsonValue::JsonUndef;
	map<const string *, int> JsonValue::sStrRefs;
	map<const JsonObject *, int> JsonValue::sObjRefs;
	map<const JsonArray *, int> JsonValue::sArrRefs;
	const char *JsonValue::sTypeNames[] = {
		"Null", "Undefined", "Integer", "Float", "Boolean"
		"String", "Object", "Array", "Comment", "BAD_JSON_TYPE"};

	JsonValue::JsonValue() :
		mType(JV_UNDEF)
	{
	}
	JsonValue::JsonValue(JsonType type) :
		mType(type)
	{
		if (type == JV_ARR) {
			mContent.a = new JsonArray();
			altArrRef(mContent.a, true);
		}
		else if (type == JV_OBJ) {
			mContent.o = new JsonObject();
			altObjRef(mContent.o, true);
		}
		else {
			mContent.i = 0;
		}
	}
	JsonValue::JsonValue(int val) :
		mType(JV_INT)
	{
		mContent.i = val;
	}
	JsonValue::JsonValue(float val) :
		mType(JV_FLOAT)
	{
		mContent.f = val;
	}
	JsonValue::JsonValue(bool val) :
		mType(JV_BOOL)
	{
		mContent.b = val;
	}
	JsonValue::JsonValue(const char *val) :
		mType(JV_STR)
	{
		string *str = new string();
		*str = val;
		altStrRef(str, true);
		mContent.s = str;
	}
	JsonValue::JsonValue(const string &val) :
		mType(JV_STR)
	{
		string *str = new string();
		*str = val;
		altStrRef(str, true);
		mContent.s = str;
	}
	JsonValue::JsonValue(const JsonArray &val) :
		mType(JV_ARR)
	{
		JsonArray *arr = new JsonArray();
		*arr = val;
		altArrRef(arr, true);
		mContent.a = arr;
	}
	JsonValue::JsonValue(const JsonObject &val) :
		mType(JV_OBJ)
	{
		JsonObject *obj = new JsonObject();
		*obj = val;
		altObjRef(obj, true);
		mContent.o = obj;
	}
	JsonValue::JsonValue(JsonArray *val) :
		mType(JV_ARR)
	{
		altArrRef(val, true);
		mContent.a = val;
	}
	JsonValue::JsonValue(JsonObject *val) :
		mType(JV_OBJ)
	{
		altObjRef(val, true);
		mContent.o = val;
	}
	JsonValue::JsonValue(const JsonValue &val) {
		*this = val;
	}
	JsonValue::~JsonValue() {
		checkRefs(false);
	}

	JsonType JsonValue::getType() const {
		return mType;
	}
	const char *JsonValue::getTypeName() const {
		return sTypeNames[mType];
	}
	int JsonValue::getInt() const {
		if (mType == JV_FLOAT) {
			return static_cast<int>(mContent.f);
		}
		return mContent.i;
	}
	float JsonValue::getFloat() const {
		if (mType == JV_INT) {
			return static_cast<float>(mContent.i);
		}
		return mContent.f;
	}
	bool JsonValue::getBool() const {
		return mContent.b;
	}
	string *JsonValue::getStr() const {
		return mContent.s;
	}
	const char *JsonValue::getCStr() const {
		if ((mType == JV_STR || mType == JV_CMT) && mContent.s != NULL) {
			return mContent.s->c_str();
		}
		return NULL;
	}
	JsonObject *JsonValue::getObj() const {
		return mContent.o;
	}
	JsonArray *JsonValue::getArr() const {
		return mContent.a;
	}

	bool JsonValue::castFloat(float &value) const
	{
		switch(mType)
		{
		default:
			return false;
		case JV_FLOAT:
			value = mContent.f;
			return true;
		case JV_INT:
			value = static_cast<float>(mContent.i);
			return true;
		case JV_BOOL:
			value = mContent.b ? 1.0f : 0.0f;
			return true;
		case JV_STR:
		case JV_CMT:
			return Utils::fromString<float>(value, mContent.s->c_str());
		}
		return false;
	}
	
	JsonValue JsonValue::operator[](int i) const {
		return at(i);
	}
	JsonValue JsonValue::operator[](const char *i) const {
		return at(i);
	}
	JsonValue JsonValue::operator[](const string &i) const {
		return at(i);
	}
	JsonValue &JsonValue::operator[](int i) {
		return at(i);
	}
	JsonValue &JsonValue::operator[](const char *i) {
		return at(i);
	}
	JsonValue &JsonValue::operator[](const string &i) {
		return at(i);
	}
	
	JsonValue JsonValue::at(int i) const {
		if (mType == JV_ARR) {
			return mContent.a->at(i);
		}
		return JsonValue();
	}
	JsonValue JsonValue::at(const char *i) const {
		return at(string(i));
	}
	JsonValue JsonValue::at(const string &i) const {
		if (mType == JV_OBJ) {
			return mContent.o->at(i);
		}
		return JsonValue();
	}

	JsonValue &JsonValue::at(int i) {
		if (mType == JV_ARR) {
			return mContent.a->at(i);
		}
		return JsonUndef;
	}
	JsonValue &JsonValue::at(const char *i) {
		return at(string(i));
	}
	JsonValue &JsonValue::at(const string &i) {
		if (mType == JV_OBJ) {
			return mContent.o->at(i);
		}
		return JsonUndef;
	}

	int JsonValue::length() const {
		if (mType == JV_ARR) {
			return static_cast<int>(mContent.a->size());
		}
		else if (mType == JV_OBJ) {
			return static_cast<int>(mContent.o->size());
		}
		return 0;
	}

	bool JsonValue::has(const char *i) const {
		return has(string(i));
	}
	bool JsonValue::has(const string &i) const {
		if (mType == JV_OBJ) {
			return mContent.o->find(i) != mContent.o->end();
		}
		return false;
	}

	bool JsonValue::has(const char *i, JsonType type) const {
		return has(string(i), type);
	}
	bool JsonValue::has(const string &i, JsonType type) const {
		if (mType == JV_OBJ) {
			JsonObject::iterator it = mContent.o->find(i);
			if (it == mContent.o->end()) {
				return false;
			}
			if (it->second.getType() == type) {
				return true;
			}
		}
		return false;
	}

	JsonValue &JsonValue::operator=(const JsonValue &rhs) {
		checkRefs(false);
		mType = rhs.mType;
		mContent = rhs.mContent;
		
		checkRefs(true);
		return *this;
	}

	bool JsonValue::operator==(const JsonValue &rhs) const
	{
		if (mType != rhs.mType)
		{
			return false;
		}
		switch(mType)
		{
		default:
		case JV_BOOL:
			return mContent.b == rhs.mContent.b;
		case JV_INT:
			return mContent.i == rhs.mContent.i;
		case JV_FLOAT:
			return mContent.f == rhs.mContent.f;
		case JV_CMT:
		case JV_STR:
			return mContent.s->compare(*rhs.mContent.s) == 0;
		case JV_ARR:
			if (mContent.a->size() != rhs.mContent.a->size())
			{
				return false;
			}
			for (size_t i = 0, len = mContent.a->size(); i < len; ++i)
			{
				if (mContent.a->at(i) != rhs.mContent.a->at(i))
				{
					return false;
				}
			}
			return true;
		case JV_OBJ:
			if (mContent.o->size() != mContent.o->size())
			{
				return false;
			}
			for (JsonObject::const_iterator iter = mContent.o->begin(),
				 end = mContent.o->end(), end2 = rhs.mContent.o->end();
				 iter != end; ++iter)
			{
				JsonObject::const_iterator find = rhs.mContent.o->find(iter->first);
				if (find == end2)
				{
					return false;
				}
				if (find->second != iter->second)
				{
					return false;
				}
			}
			return true;
		}
	}
	bool JsonValue::operator!=(const JsonValue &rhs) const
	{
		return !(*this == rhs);
	}
	void JsonValue::display(ostream &stream) {
		displayValue(stream, *this, 1);
	}

	JsonValue JsonValue::import(const char *str) {
		Tokeniser tokeniser(str);
		return import_object(tokeniser);
	}

	void JsonValue::checkRefs(bool retain) {
		if (mType == JV_STR || mType == JV_CMT) {
			altStrRef(mContent.s, retain);
		}
		else if (mType == JV_ARR) {
			altArrRef(mContent.a, retain);
		}
		else if (mType == JV_OBJ) {
			altObjRef(mContent.o, retain);
		}
	}

	void JsonValue::altStrRef(const string *str, bool retain) {
		//ALT_REFS(string, sStrRefs, str, retain);
		ALT_REFS<string>(sStrRefs, str, retain);
	}
	void JsonValue::altObjRef(const JsonObject *obj, bool retain) {
		//ALT_REFS(JsonObject, sObjRefs, obj, retain);
		ALT_REFS<JsonObject>(sObjRefs, obj, retain);
	}
	void JsonValue::altArrRef(const JsonArray *arr, bool retain) {
		//ALT_REFS(JsonArray, sArrRefs, arr, retain);
		ALT_REFS<JsonArray>(sArrRefs, arr, retain);
	}

	void JsonValue::displayValue(ostream &stream, JsonValue &value, int depth) {
		
		JsonObject *jObj = NULL;
		JsonObject::iterator oiter;
		JsonArray::iterator aiter;
		bool first = true;

		switch (value.mType) {
		case JV_BOOL:
			stream << (value.getBool() ? "true" : "false") << '\n';
			break;
		case JV_STR:
			if (value.getStr() == NULL) {
				stream << "\"null\"";
			}
			else {
				stream << '"' << *value.getStr() << '"';
			}
			break;
		case JV_INT:
			stream << value.getInt();
			break;
		case JV_FLOAT:
			stream << value.getFloat();
			break;
		case JV_OBJ:
			jObj = value.getObj();
		
			stream << "{";
			oiter = jObj->find("__comment");
			if (oiter != jObj->end() && oiter->second.getType() == JV_CMT) {
				stream << " /* " << *oiter->second.getStr() << " */";
			}
			stream << '\n';
			
			for (oiter = jObj->begin() ;oiter != jObj->end(); ++oiter) {
				if (oiter->second.getType() == JV_CMT) 
				{
					continue;
				}
				if (!first) 
				{
					stream << ",\n";
				}
				displayDepth(stream, depth);
				stream << oiter->first << ": ";
				displayValue(stream, oiter->second, depth + 1);
				first = false;
				JsonType ot = oiter->second.getType();
				if (ot == JV_ARR || ot == JV_OBJ) {
					stream << " /* end " << oiter->first << " */";
				}
			}
			stream << '\n';
			displayDepth(stream, depth - 1);
			stream << '}';
			break;
		case JV_ARR:
			aiter = value.getArr()->begin();
			stream << "[\n";
			
			for (;aiter != value.getArr()->end(); ++aiter) {
				if (!first) {
					stream << ",\n";
				}
				displayDepth(stream, depth);
				displayValue(stream, *aiter, depth + 1);
				first = false;
			}
			stream << '\n';
			displayDepth(stream, depth - 1);
			stream << ']';
			break;
		default:
			stream << "<Unknown Type>";
			break;
		}
	}

	JsonValue JsonValue::import_object(Tokeniser &tokeniser, const char *startToken) {
		const char *token = startToken;
		if (token == NULL) {
			token = nextToken(tokeniser);
		}
		if (token == NULL) {
			return JsonValue();
		}
		
		char ch = getChar(token);
		if (ch == '{') {
			token = nextToken(tokeniser, false);
			if (token != NULL) {
				ch = getChar(token);
			}
			string valueName;
			JsonObject *obj = new JsonObject();
		
			if (ch == '/') {
				token += 2;
				if (token[0] == ' ') {
					token++;
				}
				string cmtStr = token;
				size_t e = cmtStr.size() - 1;
				if (cmtStr[e] == '/' && cmtStr[e - 1] == '*') {
					int offset = -1;
					if (cmtStr[e - 2] == ' ') {
						offset--;
					}
					cmtStr = cmtStr.substr(0, e + offset);
				}

				(*obj)["__comment"] = cmtStr;
				(*obj)["__comment"].mType = JV_CMT;
				token = nextToken(tokeniser);
				ch = getChar(token);
			}

			while (token != NULL && ch != '}') {
				if (ch == '"' || ch == '\'') {
					valueName = token + 1;
				}
				else {
					valueName = token;
				}
				// Should be a ':'
				nextToken(tokeniser);	

				JsonValue imported_object = import_object(tokeniser);
				(*obj)[valueName] = imported_object;

				token = nextToken(tokeniser);
				ch = getChar(token);
				if (ch == ',') {
					token = nextToken(tokeniser);
					ch = getChar(token);
				}
			}

			return JsonValue(obj);
		}
		else if(ch == '[') {

			token = nextToken(tokeniser);
			ch = getChar(token);

			JsonArray *arr = new JsonArray();

			while(token != NULL && ch != ']') {
			 
				arr->push_back(import_object(tokeniser, token));

				token = nextToken(tokeniser);
				ch = getChar(token);
				if (ch == ',') {
					token = nextToken(tokeniser);
					ch = getChar(token);
				}
			}

			return JsonValue(arr);
		}
		else if(ch == '"' || ch == '\'') {
			return JsonValue(token + 1);
		}
		else if(ch == 't' || ch == 'T') {
			return JsonValue(true);
		}
		else if(ch == 'f' || ch == 'F') {
			return JsonValue(false);
		}
		else {
			string num = token;
			if (num.find('.') == string::npos) {
				int val;
				//sscanf(num.c_str(), "%d", &val);
				Utils::fromString<int>(val, num);
				return JsonValue(val);
			}
			else {
				float val;
				//sscanf(num.c_str(), "%f", &val);
				Utils::fromString<float>(val, num);
				return JsonValue(val);
			}
		}
		return JsonValue();
	}

	JsonValue JsonValue::import_from_file(const char *filename) {
		
		JsonValue loaded(-1);
		try
		{
			std::ifstream t(filename);
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

			loaded = JsonValue::import(str.c_str());
		}
		catch(...)
		{
			std::stringstream ss;
			ss << "Error importing JSON: " << filename;
			am_log("JSON", ss);
		}
		
		return loaded;
	}

	void JsonValue::displayDepth(ostream &stream, int depth) {
		for (int i = 0; i < depth; i++) {
			stream << "  ";
		}
	}

	char JsonValue::getChar(const char *token) {
		if (token != NULL) {
			return token[0];
		}
		return '\0';
	}

	const char *JsonValue::nextToken(Tokeniser &tokeniser, bool skipComments) {
		const char *token = tokeniser.nextToken();
		while(skipComments && token != NULL && token[0] == '/') {
			printf("Comment: %s\n", token);
			token = tokeniser.nextToken();
		}
		return token;
	}

}
}
