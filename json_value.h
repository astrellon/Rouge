#pragma once

enum JsonType {
	JV_NULL,
	JV_UNDEF,
	JV_INT,
	JV_FLOAT,
	JV_BOOL,
	JV_STR,
	JV_OBJ,
	JV_ARR
};

#include <string>
#include <map>
#include <vector>

using namespace std;

class JsonValue;

typedef map<string, JsonValue> JsonObject;
typedef vector<JsonValue> JsonArray;

typedef union {
	int i;
	float f;
	bool b;
	string *s;
	JsonObject *o;
	JsonArray *a;
} JsonUnion;

#define ALT_REFS(type, refMap, value, retain)	\
	map<const type *, int>::iterator iter;	\
	iter = refMap.find(value);	\
	if (iter != refMap.end()) {	\
		int count = iter->second + (retain ? 1 : -1);	\
		if (count <= 0) {	\
			const type *ptr = iter->first;	\
			refMap.erase(iter);	\
			delete ptr;	\
		}	\
		else {	\
			iter->second = count;	\
		}	\
	}	\
	else if (retain) {	\
		refMap[value] = 1;	\
	}

class JsonValue {
public:
	JsonValue() {
		mType = JV_UNDEF;
	}
	JsonValue(JsonType type) {
		mType = type;
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
	JsonValue(int val) {
		mType = JV_INT;
		mContent.i = val;
	}
	JsonValue(float val) {
		mType = JV_FLOAT;
		mContent.f = val;
	}
	JsonValue(bool val) {
		mType = JV_BOOL;
		mContent.b = val;
	}
	JsonValue(const char *val) {
		mType = JV_STR;
		string *str = new string();
		*str = val;
		altStrRef(str, true);
		mContent.s = str;
	}
	JsonValue(const string &val) {
		mType = JV_STR;
		string *str = new string();
		*str = val;
		altStrRef(str, true);
		mContent.s = str;
	}
	JsonValue(const JsonArray &val) {
		mType = JV_ARR;
		JsonArray *arr = new JsonArray();
		*arr = val;
		altArrRef(arr, true);
		mContent.a = arr;
	}
	JsonValue(const JsonObject &val) {
		mType = JV_OBJ;
		JsonObject *obj = new JsonObject();
		*obj = val;
		altObjRef(obj, true);
		mContent.o = obj;
	}
	JsonValue(JsonArray *val) {
		mType = JV_ARR;
		altArrRef(val, true);
		mContent.a = val;
	}
	JsonValue(JsonObject *val) {
		mType = JV_OBJ;
		altObjRef(val, true);
		mContent.o = val;
	}
	JsonValue(const JsonValue &val) {
		*this = val;
	}
	~JsonValue() {
		checkRefs(false);
	}
	
	int getType() const {
		return mType;
	}
	int getInt() const {
		if (mType == JV_FLOAT) {
			return static_cast<int>(mContent.f);
		}
		return mContent.i;
	}
	float getFloat() const {
		if (mType == JV_INT) {
			return static_cast<float>(mContent.i);
		}
		return mContent.f;
	}
	bool getBool() const {
		return mContent.b;
	}
	string *getStr() {
		return mContent.s;
	}
	const char *getCStr() {
		if (mType == JV_STR && mContent.s != NULL) {
			return mContent.s->c_str();
		}
		return NULL;
	}
	JsonObject *getObj() {
		return mContent.o;
	}
	JsonArray *getArr() {
		return mContent.a;
	}
	
	JsonValue operator[](int i) const {
		return at(i);
	}
	JsonValue operator[](const char *i) const {
		return at(i);
	}
	JsonValue operator[](const string &i) const {
		return at(i);
	}
	JsonValue &operator[](int i) {
		return at(i);
	}
	JsonValue &operator[](const char *i) {
		return at(i);
	}
	JsonValue &operator[](const string &i) {
		return at(i);
	}
	
	JsonValue at(int i) const {
		if (mType == JV_ARR) {
			return mContent.a->at(i);
		}
		return JsonUndef;
	}
	JsonValue at(const char *i) const {
		return at(string(i));
	}
	JsonValue at(const string &i) const {
		if (mType == JV_OBJ) {
			return mContent.o->at(i);
		}
		return JsonUndef;
	}

	JsonValue &at(int i) {
		if (mType == JV_ARR) {
			return mContent.a->at(i);
		}
		return JsonUndef;
	}
	JsonValue &at(const char *i) {
		return at(string(i));
	}
	JsonValue &at(const string &i) {
		if (mType == JV_OBJ) {
			return mContent.o->at(i);
		}
		return JsonUndef;
	}

	int length() const {
		if (mType == JV_ARR) {
			return static_cast<int>(mContent.a->size());
		}
		else if (mType == JV_OBJ) {
			return static_cast<int>(mContent.o->size());
		}
		return 0;
	}

	bool has(const char *i) const {
		return has(string(i));
	}
	bool has(const string &i) const {
		if (mType == JV_OBJ) {
			return mContent.o->find(i) != mContent.o->end();
		}
		return false;
	}

	JsonValue &operator=(const JsonValue &rhs) {
		checkRefs(false);
		mType = rhs.mType;
		mContent = rhs.mContent;
		
		checkRefs(true);
		return *this;
	}
	
	static JsonValue JsonUndef;

	void display() {
		displayValue(*this, 1);
	}
	
protected:
	JsonType mType;
	JsonUnion mContent;

	void checkRefs(bool retain) {
		if (mType == JV_STR) {
			altStrRef(mContent.s, retain);
		}
		else if (mType == JV_ARR) {
			altArrRef(mContent.a, retain);
		}
		else if (mType == JV_OBJ) {
			altObjRef(mContent.o, retain);
		}
	}

	void altStrRef(const string *str, bool retain) {
		ALT_REFS(string, sStrRefs, str, retain);
	}
	void altObjRef(const JsonObject *obj, bool retain) {
		ALT_REFS(JsonObject, sObjRefs, obj, retain);
	}
	void altArrRef(const JsonArray *arr, bool retain) {
		ALT_REFS(JsonArray, sArrRefs, arr, retain);
	}

	void displayValue(JsonValue &value, int depth) {
		
		JsonObject::iterator oiter;
		JsonArray::iterator aiter;
		switch (value.mType) {
		case JV_BOOL:
			printf("%s\n", (value.getBool() ? "true" : "false"));
			break;
		case JV_STR:
			printf("%s\n", value.getCStr());
			break;
		case JV_INT:
			printf("%d\n", value.getInt());
			break;
		case JV_FLOAT:
			printf("%f\n", value.getFloat());
			break;
		case JV_OBJ:
			oiter = value.getObj()->begin();
			printf("{\n");
			
			for (;oiter != value.getObj()->end(); ++oiter) {
				printf("%*c %s = ", depth * 2, ' ', oiter->first.c_str());
				displayValue(oiter->second, depth + 1);
			}
			printf("%*c}\n", (depth - 1) * 2, ' ');
			break;
		case JV_ARR:
			aiter = value.getArr()->begin();
			printf("[\n");
			
			for (;aiter != value.getArr()->end(); ++aiter) {
				printf("%*c", depth * 2, ' ');
				displayValue(*aiter, depth + 1);
			}
			printf("%*c]\n", (depth - 1) * 2, ' ');
			break;
		default:
			printf("<Unknown Type>\n");
			break;
		}
	}

private:
	static map<const string *, int> sStrRefs;
	static map<const JsonObject *, int> sObjRefs;
	static map<const JsonArray *, int> sArrRefs;

};
