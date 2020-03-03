#pragma once

namespace SimpleReflection
{

inline bool SignalError(const char* expression, const char* file, size_t line, bool& ignore, const char* msgFormat, ...)
{
  return !ignore;
}

#define ErrorIf(Expression, ...) \
  do { static bool __ignore = false; if((Expression) && SimpleReflection::SignalError(#Expression, __FILE__, __LINE__, __ignore,##__VA_ARGS__)) \
  __debugbreak(); } while(false)

}//namespace SimpleReflection
