#pragma once

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
#	include <lua/src/lstate.h>
} 

#include <string>
#include <map>
#include <ostream>
using namespace std;

#include <base/imanaged.h>
using namespace am::base;

#define lua_tofloat(L,i)	static_cast<float>(lua_tonumber(L,i))
#define lua_tobool(L,i)		(lua_toboolean(L,i) > 0)
#define lua_isnum(L,i)		(lua_type(L,i) == LUA_TNUMBER)
#define lua_isstr(L,i)		(lua_type(L,i) == LUA_TSTRING)
#define lua_isbool(L,i)		(lua_type(L,i) == LUA_TBOOLEAN)
#define lua_first(L)		lua_pushvalue(L,1); return 1;

// Wraps the lua call function with a try catch and a log message with the same file and line as the error if there is one.
// Should only be used for unit tests as it does not deal with the error, only log that there was one.
#define lua_acall(L, i, n)	try { L.call(i, n); } catch (...) { am::lua::LuaState::displayLineError(L, __FILE__, __LINE__); }

namespace am {
namespace lua {
	
	/**
	 * Helper struct for storing Lua userdata.
	 * Contains an id which identifies the class and a pointer to
	 * the current instance.
	 */
	typedef struct {
		int id;
		void *ptr;
	} LuaUData;

	/**
	 * Allocates userdata memory in Lua.
	 * Allocates enough space for a LuaUData struct.
	 *
	 * @param lua The lua_State to allocate the memory on.
	 * @param obj The object to allocate the memory for.
	 */
	template <class T>
	inline void newUData(lua_State *lua, T *obj)
	{
		LuaUData *udata = reinterpret_cast<LuaUData *>(
			lua_newuserdata(lua, sizeof(LuaUData)));
		udata->id = T::LUA_ID;
		udata->ptr = obj;
	}

	/**
	 * Attempts to cast the parameter at index n to the given class T.
	 * This only works if the memory was allocated by the newUData function,
	 * or if the userdata is structurally the same as a LuaUData struct.
	 *
	 * @param lua The lua_State to work on.
	 * @param n The stack index to the value.
	 * @return A non-NULL value if the id's match
	 */
	template <class T>
	inline T *castUData(lua_State *lua, int n)
	{
		if (lua_type(lua, n) == LUA_TUSERDATA)
		{
			LuaUData *udata = reinterpret_cast<LuaUData *>(lua_touserdata(lua, n));
			if (udata->id == T::LUA_ID)
			{
				return reinterpret_cast<T *>(udata->ptr);
			}
		}
		return NULL;
	}
	int getUDataType(lua_State *lua, int n);

	/**
	 * Wraps an object instance into Lua.
	 * This requires that this class have the static consts
	 * - int LUA_ID
	 * - const char *LUA_TABLENAME
	 * These are both used to identify the class type at a later date
	 * and for applying which metatable.
	 *
	 * Once the object is wrapped it is left on top of the Lua stack.
	 *
	 * @param lua The lua_State to work on.
	 * @param object The object to wrap.
	 */
	template <class T>
	inline void wrapObject(lua_State *lua, T *object)
	{
		newUData<T>(lua, object);

		luaL_getmetatable(lua, T::LUA_TABLENAME);
		lua_setmetatable(lua, -2);
	}
	/**
	 * Wraps an object instance that extends IManaged into Lua.
	 * This requires that this class have the static consts
	 * - int LUA_ID
	 * - const char *LUA_TABLENAME
	 * - extends am::base::IManaged
	 * The first two are both used to identify the class type at a later date
	 * and for applying which metatable.
	 *
	 * Once the object is wrapped and retained then left on top of the Lua stack.
	 *
	 * @param lua The lua_State to work on.
	 * @param object The object to wrap.
	 */
	template <class T>
	inline void wrapRefObject(lua_State *lua, T *object)
	{
		newUData<T>(lua, object);

		object->retain();

		luaL_getmetatable(lua, T::LUA_TABLENAME);
		lua_setmetatable(lua, -2);
	}

	/**
	 * Wrapped/Extension of the lua_State struct to provide extra functionality.
	 *
	 * Can either create a new lua_State struct or uses one that is passed
	 * in. Created lua_State's can either be extended with extra functions
	 * specific to the game engine, or can be left at only the default lua libraries.
	 */
	class LuaState : public IManaged {
	public:

		typedef map<string, lua_CFunction> WrapperMap;
		typedef map<int, string> WrapperIdMap;

		/// Creates a new lua_State.
		/// If includeLibraries is true then the 'import' function is exposed.
		/// The 'import' function allows scripts to obtain wrappers for the
		/// game classes as they need them.
		/// An 'am_log' function is always added which is a simple pass through to
		/// the internal logger functionality.
		LuaState(bool includeLibraries = true);
		/// Wraps an existing lua_State.
		/// Does not expose any new functions to the Lua registry.
		LuaState(lua_State *lua);
		~LuaState();

		/// Returns the internal lua_State.
		lua_State *getLua();
		/// Calls lua_close on the internal lua_State.
		void close();

		bool loadFile(const char *filename);
		bool loadString(const char *luaString);

		void call(int n, int r);

		/// Automatically allows for a LuaState to be cast back
		/// to a lua_State when it's needed.
		operator lua_State *();

		int newTable(const char *tableName = NULL);
		bool getTable(int tableRef);
		bool getTable(const char *tableName);
		bool compareRefs(int ref1, int ref2) const;

		void pop(int num);
		void push(const char *str);
		void push(int num);
		void push(double num);
		void push(bool val);
		void pushnil();

		void setTableValue(const char *key, const char *value);
		void setTableValue(const char *key, int value);
		void setTableValue(const char *key, double value);
		void setTableValue(const char *key, bool value);

		const char *getTableString(const char *key);
		bool getTableInt(const char *key, int &value);
		bool getTableDouble(const char *key, double &value);
		bool getTableBool(const char *key, bool &value);

		bool isTableNumber(const char *key);
		bool isTableString(const char *key);
		bool isTableTable(const char *key);
		bool isTableBool(const char *key);

		int hasTableValue(const char *key);

		float toNumber();
		int toInteger();
		bool toBool();

		bool hasAmType(const char *keyName, int luaType);
		bool hasGlobalFunction(const char *func, bool popAfter = false);
		int getGlobalInt(const char *name);
		double getGlobalDouble(const char *name);
		bool getGlobalBool(const char *name);
		string getGlobalString(const char *name);
		bool getGlobal(const char *name);

		static int onError(lua_State *mLua);

		void logStack(const char *cat);
		void printStack(ostream &output);

		void logTable(const char *cat, int n);
		void printTable(ostream &output, int n, bool includeType = true);

		bool operator==(const lua_State *lua) const;
		bool operator==(const LuaState &rhs) const;
		bool operator!=(const lua_State *lua) const;
		bool operator!=(const LuaState &rhs) const;

		static void registerWrapper(const char *name, int id);
		static int luaEquals(lua_State *lua);
		static int luaNotEquals(lua_State *lua);
		static int luaPrintStack(lua_State *lua);
		static int getMaxRegisteredId();

		static void logStack(lua_State *lua, const char *cat);
		static void printStack(lua_State *lua, ostream &output);
		static const char *getType(lua_State *lua, int n);
		static void printTypeValue(lua_State *lua, int n, ostream &output, bool includeType = true);

		static int luaAmLog(lua_State *lua);

		static void displayLineError(lua_State *lua, const char *file, int line);

		static int expectedArgs(lua_State *lua, const char *funcName, const char *expected);
		static int expectedArgs(lua_State *lua, const char *funcName, int n, ...);
		static int expectedContext(lua_State *lua, const char* funcName, const char *expected);
		static void warning(lua_State *lua, const char *message);

		static void setMaxTableDepth(int depth);
		static int getMaxTableDepth();

	protected:
		lua_State *mLua;
		int mAmTableRef;
		int mAmTableIndexRef;

		static WrapperIdMap sWrapperIdMap;
		static int sWrapperMaxId;

		static int sDepth;
		static int sMaxDepth;
		static jmp_buf sRecoverBuff;
	};

	class LuaHandle {
	public:
		LuaHandle(lua_State *lua, unsigned int popElements = 0);
		virtual ~LuaHandle();

		void setPop(unsigned int pop);
		unsigned int getPop() const;

	protected:
		lua_State *mLua;
		unsigned int mPop;
	};

}
}
