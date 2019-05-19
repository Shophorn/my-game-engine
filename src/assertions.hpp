#pragma once

#include <cassert>

/*
re-define this in case we want to do something else than vanilla assert
*/
#define NG_ASSERT assert

#define SUPRESS_NO_RETURN #pragma clang diagnostic ignored "-Wreturn-type"