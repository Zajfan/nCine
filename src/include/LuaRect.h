#ifndef CLASS_NCINE_LUARECT
#define CLASS_NCINE_LUARECT

#define NCINE_INCLUDE_LUA
#include "common_headers.h"

#include "LuaRectUtils.h"

namespace ncine {

namespace LuaNames {
namespace Rect {
	static const char *Rect = "rect";

	static const char *create = "create";
	static const char *fromCenterAndSize = "create_center_size";
}}

/// Lua bindings around the `Rect` template class
template <class T>
class LuaRect
{
  public:
	static void expose(lua_State *L);

  private:
	static int create(lua_State *L);
	static int fromCenterAndSize(lua_State *L);
};

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

template <class T>
void LuaRect<T>::expose(lua_State *L)
{
	lua_createtable(L, 0, 2);

	LuaUtils::addFunction(L, LuaNames::Rect::create, create);
	LuaUtils::addFunction(L, LuaNames::Rect::fromCenterAndSize, fromCenterAndSize);

	lua_setfield(L, -2, LuaNames::Rect::Rect);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

template <class T>
int LuaRect<T>::create(lua_State *L)
{
	const T x = LuaUtils::retrieve<T>(L, -4);
	const T y = LuaUtils::retrieve<T>(L, -3);
	const T w = LuaUtils::retrieve<T>(L, -2);
	const T h = LuaUtils::retrieve<T>(L, -1);

	const Rect<T> rect(x, y, w, h);

	LuaRectUtils<T>::push(L, rect);

	return 1;
}

template <class T>
int LuaRect<T>::fromCenterAndSize(lua_State *L)
{
	const T x = LuaUtils::retrieve<T>(L, -4);
	const T y = LuaUtils::retrieve<T>(L, -3);
	const T w = LuaUtils::retrieve<T>(L, -2);
	const T h = LuaUtils::retrieve<T>(L, -1);

	const Rect<T> rect = Rect<T>::fromCenterAndSize(x, y, w, h);

	LuaRectUtils<T>::push(L, rect);

	return 1;
}

}

#endif
