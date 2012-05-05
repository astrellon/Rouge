#pragma once
/*
#define JV_NULL	0
#define JV_UNDEF	1
#define JV_INT	2
#define JV_FLOAT	3
#define JV_BOOL	4
#define JV_STR	5
#define JV_OBJ	6
#define JV_ARR	7
*/
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
/*
#define ALT_REFS(type, refmap, value, retain)	\
	map<type, int>::iterator iter;	\
	iter = refmap.find(value);	\
	if (iter != refmap.end()) {	\
		int count = iter->second + (retain ? 1 : -1);	\
		if (count <= 0) {	\
			type ptr = iter->first;	\
			refmap.erase(iter);	\
			printf("Deleting refernce to %s\n", #type);	\
			delete ptr;	\
		}	\
	}
	*/
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
		return mContent.i;
	}
	float getFloat() const {
		return mContent.f;
	}
	bool getBool() const {
		return mContent.b;
	}
	string *getStr() {
		return mContent.s;
	}
	const char *getCStr() {
		if (mType == JV_STR) {
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
		altRefs<string>(sStrRefs, str, retain);
	}
	void altObjRef(const JsonObject *obj, bool retain) {
		altRefs<JsonObject>(sObjRefs, obj, retain);
	}
	void altArrRef(const JsonArray *arr, bool retain) {
		altRefs<JsonArray>(sArrRefs, arr, retain);
	}

private:
	static map<const string *, int> sStrRefs;
	static map<const JsonObject *, int> sObjRefs;
	static map<const JsonArray *, int> sArrRefs;

	template <class T>
	void altRefs(map<const T *, int> &refMap, const T *value, bool retain) {
		map<const T *, int>::iterator iter;
		iter = refMap.find(value);
		if (iter != refMap.end()) {
			int count = iter->second + (retain ? 1 : -1);
			if (count <= 0) {
				const T *ptr = iter->first;
				refMap.erase(iter);
				printf("Deleting refernce to %s\n", typeid(T).name());
				delete ptr;
			}
			else {
				iter->second = count;
			}
		}
		else if (retain) {
			refMap[value] = 1;
		}
	}

};

JsonValue JsonValue::JsonUndef;
map<const string *, int> JsonValue::sStrRefs;
map<const JsonObject *, int> JsonValue::sObjRefs;
map<const JsonArray *, int> JsonValue::sArrRefs;
