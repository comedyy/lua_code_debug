
extern "C" 
{
#include "lua-5.4.0/lua.h"
#include "lua-5.4.0/lualib.h"
#include "lua-5.4.0/lauxlib.h"
#include "lua-5.4.0/lobject.h"
}

#include <iostream>

int howdy(lua_State* state)
{
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);

	printf("howdy() was called with %d arguments:\n", args);

	for (int n = 1; n <= args; ++n) {
		printf("  argument %d: '%s'\n", n, lua_tostring(state, n));
	}

	// Push the return value on top of the stack. NOTE: We haven't popped the
	// input arguments to our function. To be honest, I haven't checked if we
	// must, but at least in stack machines like the JVM, the stack will be
	// cleaned between each function call.

	lua_pushnumber(state, 123);

	// Let Lua know how many return values we've passed
	return 1;
}

void print_error(lua_State* state) {
	// The error message is on top of the stack.
	// Fetch it, print it and then pop it off the stack.
	const char* message = lua_tostring(state, -1);
	puts(message);
	lua_pop(state, 1);
}

void execute(const char* filename)
{
	lua_State *state = luaL_newstate();
	lua_register(state, "howdy", howdy);

	// Make standard libraries available in the Lua object
	luaL_openlibs(state);

	lua_createtable(state, 0, 0);//top/tb/
	lua_pushinteger(state, 9999);//top/9999/tb
	std::cout << lua_gettop(state) << std::endl;

	std::cout << lua_tointeger(state, 2) << std::endl;
	std::cout << lua_istable(state, 1) << std::endl;

	lua_seti(state, 1, 100) ;//tb  tb[100]=9999

	std::cout << lua_gettop(state) << std::endl;
	std::cout << lua_istable(state, 1) << std::endl;

	std::cout << lua_geti(state, 1, 100) << std::endl;//top/9999/tb
	std::cout << lua_tointeger(state, 2) << std::endl;;

	lua_pop(state, 1);

	lua_setglobal(state, "KKK");

	int result;

	// Load the program; this supports both source code and bytecode files.
	result = luaL_loadfile(state, filename);

	if (result != LUA_OK) {
		print_error(state);
		return;
	}

	std::cout << lua_gettop(state) << std::endl;
	std::cout << lua_iscfunction(state, 1) << std::endl;
	std::cout << lua_isfunction(state, 1) << std::endl;

	std::cout << sizeof(TValue) << std::endl;
	std::cout << sizeof(Value) << std::endl;
	std::cout << sizeof(int) << std::endl;
	std::cout << sizeof(GCObject) << std::endl;
	
	// Finally, execute the program by calling into it.
	// Change the arguments if you're not running vanilla Lua code.

	result = lua_pcall(state, 0, LUA_MULTRET, 0);

	if (result != LUA_OK) {
		print_error(state);
		return;
	}
}

int main(int argc, char** argv)
{
	execute("E:/work/test/pythonTool/test.lua");

	char c;
	std::cin >> c;

	return 0;
}