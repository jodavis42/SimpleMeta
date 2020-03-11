#include "Precompiled.hpp"

#include <stdarg.h>
#include "Asserts.hpp"

namespace SimpleReflection
{

ScopedAssertHandler::ScopedAssertHandler()
{
  BeginListening();
}

ScopedAssertHandler::~ScopedAssertHandler()
{
  StopListening();
}

void ScopedAssertHandler::BeginListening()
{
  if(!mIsListening)
    AssertHandlers::GetInstance().Add(this);
  mIsListening = true;
}

void ScopedAssertHandler::StopListening()
{
  if(mIsListening)
    AssertHandlers::GetInstance().Remove(this);
  mIsListening = false;
}

void ScopedAssertHandler::AddAssert(const AssertData& assertData)
{
  mAsserts.emplace_back(assertData);
}

AssertHandlers& AssertHandlers::GetInstance()
{
  static AssertHandlers sInstance;
  return sInstance;
}

void AssertHandlers::Add(ScopedAssertHandler* handler)
{
  mHandlers.emplace_back(handler);
}

void AssertHandlers::Remove(ScopedAssertHandler* handler)
{
  for(auto it = mHandlers.begin(); it != mHandlers.end(); ++it)
  {
    if(*it == handler)
    {
      mHandlers.erase(it);
      break;
    }
  }
}

bool AssertHandlers::HandleAssert(const AssertData& assertData)
{
  bool shouldBreak = true;
  for(ScopedAssertHandler* handler : mHandlers)
  {
    handler->AddAssert(assertData);
    shouldBreak &= !handler->mPreventsDebugBreak;
  }
  return shouldBreak;
}

bool SignalError(const char* expression, const char* file, size_t line, bool& ignore, const char* msgFormat, ...)
{
  char buffer[300];
  va_list args;
  va_start(args, msgFormat);
  vsprintf_s(buffer, msgFormat, args);
  va_end(args);

  AssertData data;
  data.mMessage = buffer;
  data.mLine = line;
  data.mExpression = expression;
  return AssertHandlers::GetInstance().HandleAssert(data);
}

}//namespace SimpleReflection
