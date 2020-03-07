#pragma once

#include <string>
#include <vector>

namespace SimpleReflection
{

struct AssertData
{
  std::string mExpression;
  std::string mMessage;
  size_t mLine;
};

struct ScopedAssertHandler
{
  ScopedAssertHandler();
  ~ScopedAssertHandler();

  void BeginListening();
  void StopListening();

  void AddAssert(const AssertData& assertData);

  bool mPreventsDebugBreak = true;
  bool mIsListening = false;
  std::vector<AssertData> mAsserts;
};

struct AssertHandlers
{
  static AssertHandlers& GetInstance();

  void Add(ScopedAssertHandler* handler);
  void Remove(ScopedAssertHandler* handler);

  bool Assert(const AssertData& assertData);

  std::vector<ScopedAssertHandler*> mHandlers;
};

bool SignalError(const char* expression, const char* file, size_t line, bool& ignore, const char* msgFormat, ...);

#define ReflectionErrorIf(Expression, ...) \
  do { static bool __ignore = false; if((Expression) && SimpleReflection::SignalError(#Expression, __FILE__, __LINE__, __ignore,##__VA_ARGS__)) \
  __debugbreak(); } while(false)

}//namespace SimpleReflection
