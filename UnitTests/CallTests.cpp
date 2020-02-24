#include "AnyTests.hpp"

#include "Bindings.hpp"
#include "MetaLibrary.hpp"
#include "Call.hpp"

#include "UnitTesting.hpp"

struct SimpleGetterTest
{
  SimpleGetterTest() : mValue(0) {}
  SimpleGetterTest(float value) : mValue(value) { }

  float Get() { return mValue; }
  float GetConst() const { return mValue; }
  float& GetRef() { return mValue; }
  const float& GetConstRef() { return mValue; }
  const float& GetConstRefConst() const { return mValue; }
  float* GetPointer() { return &mValue; }
  const float* GetConstPointer() { return &mValue; }
  const float* GetConstPointerConst() const { return &mValue; }

  float mValue;
};

struct SimpleSetterTest
{
  SimpleSetterTest()
  {
    mData = 0;
    mPointerData = nullptr;
    mConstPointerData = nullptr;
  }
  void Set(float data) { Clear(); mData = data; }
  void SetRef(float& data) { Clear(); mPointerData = &data; }
  void SetConstRef(const float& data) { Clear(); mConstPointerData = &data; }
  void SetPointer(float* data) { Clear(); mPointerData = data; }
  void SetConstPointer(const float* data) { Clear(); mConstPointerData = data; }
  void Clear()
  {
    mData = 0;
    mPointerData = nullptr;
    mConstPointerData = nullptr;
  }

  float mData;
  float* mPointerData;
  const float* mConstPointerData;
};

struct Number
{
  Number() : mValue(0) { };
  explicit Number(float value) : mValue(value) { };
  Number(const Number& rhs) : mValue(rhs.mValue) {};

  bool operator==(const Number& rhs) const { return mValue == rhs.mValue; }
  bool operator!=(const Number& rhs) const { return !(mValue == rhs.mValue); }
  void operator+=(const Number& rhs) { mValue += rhs.mValue; }
  Number& operator-=(const Number& rhs) { mValue -= rhs.mValue; return *this; }

  static float AddStatic(const Number& lhs, const Number& rhs) { return lhs.mValue + rhs.mValue; }
  static float MulStatic(const Number& lhs, float scalar) { return lhs.mValue + scalar; }
  float Inv() const { return 1 / mValue; }
  void Invert() { mValue = 1 / mValue; }
  
  float mValue;
};

template <typename T, typename ClassType>
void Compare(T result, ClassType& instance)
{
  ErrorIf(result != instance.mValue, "Call failed");
}

template <typename T, typename ClassType>
void Compare(T* result, ClassType& instance)
{
  ErrorIf(*result != instance.mValue, "Call failed");
  ErrorIf(result != &instance.mValue, "Call Failed");
}

template <typename FnType, FnType fn, typename ClassType, typename ReturnType, typename ... Args>
void TestSimpleReturn(ClassType& instance, ReturnType(ClassType::*)(Args...))
{
  ScopedFunction boundFn = CreateMethod(fn);
  Call call(boundFn);
  call.SetPointerUnchecked(Call::This, &instance);
  call.Invoke();
  ReturnType ret = call.Get<ReturnType>(Call::Return);
  Compare(ret, instance);
}

template <typename FnType, FnType fn, typename ClassType, typename ReturnType, typename ... Args>
void TestSimpleReturn(ClassType& instance, ReturnType(ClassType::*)(Args...)const)
{
  ScopedFunction boundFn = CreateMethod(fn);
  Call call(boundFn);
  call.SetPointerUnchecked(Call::This, &instance);
  call.Invoke();
  ReturnType ret = call.Get<ReturnType>(Call::Return);
  Compare(ret, instance);
}
#define TestSimpleReturnFunction(instance, Fn) TestSimpleReturn<decltype(Fn), Fn>(instance, Fn);

// A bunch of simple tests on getting a float with various qualifiers
void TestSimpleGetters()
{
  SimpleGetterTest self(1);
  TestSimpleReturnFunction(self, &SimpleGetterTest::Get);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetConst);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetRef);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetConstRef);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetConstRefConst);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetPointer);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetConstPointer);
  TestSimpleReturnFunction(self, &SimpleGetterTest::GetConstPointerConst);
}

// A bunch of simple tests on setting a float with various qualifiers
void TestSimpleSetters()
{
  float data = 1;
  float& refData = data;
  const float& constRefData = data;
  float* ptrData = &data;
  const float* constPtrData = &data;

  {
    float data = 1;
    SimpleSetterTest instance;
    ScopedFunction boundFn = CreateMethod(&SimpleSetterTest::Set);
    Call call(boundFn);
    call.SetPointerUnchecked(Call::This, &instance);
    call.Set(0, data);
    call.Invoke();
    ErrorIf(instance.mData != data, "Set(float) failed");
  }
  
  {
    
    SimpleSetterTest instance;
    ScopedFunction boundFn = CreateMethod(&SimpleSetterTest::SetRef);
    Call call(boundFn);
    call.SetPointerUnchecked(Call::This, &instance);
    call.SetReference(0, refData);
    call.Invoke();
    ErrorIf(instance.mPointerData != &refData, "Set(float&) failed");
  }

  {

    SimpleSetterTest instance;
    ScopedFunction boundFn = CreateMethod(&SimpleSetterTest::SetConstRef);
    Call call(boundFn);
    call.SetPointerUnchecked(Call::This, &instance);
    call.SetReference(0, refData);
    call.Invoke();
    ErrorIf(instance.mConstPointerData != &refData, "Set(const float&) failed");
  }

  {

    SimpleSetterTest instance;
    ScopedFunction boundFn = CreateMethod(&SimpleSetterTest::SetPointer);
    Call call(boundFn);
    call.SetPointerUnchecked(Call::This, &instance);
    call.Set(0, ptrData);
    call.Invoke();
    ErrorIf(instance.mPointerData != &data, "Set(float*) failed");
  }

  {

    SimpleSetterTest instance;
    ScopedFunction boundFn = CreateMethod(&SimpleSetterTest::SetConstPointer);
    Call call(boundFn);
    call.SetPointerUnchecked(Call::This, &instance);
    call.Set(0, ptrData);
    call.Invoke();
    ErrorIf(instance.mConstPointerData != &data, "Set(const float*) failed");
  }
}


void TestNumber()
{
  Number one(1);
  Number two(2);
  Number three(3);

  {
    ScopedFunction fn = CreateMethod(&Number::AddStatic);
    Call call(fn);
    call.Set(0, one);
    call.Set(1, two);
    call.Invoke();
    float fResult = call.Get<float>(Call::Return);
    ErrorIf(fResult != 3, "Call Failed");
  }
  
  {
    ScopedFunction fn = CreateMethod(&Number::Inv);
    Call call(fn);
    call.SetPointerUnchecked(Call::This, &two);
    call.Invoke();
    float fResult = call.Get<float>(Call::Return);
    ErrorIf(fResult != 0.5f, "Call Failed");
  }

  {
    Number self(0);
    ScopedFunction fn = CreateMethod(&Number::operator+=);
    Call call(fn);
    call.SetPointerUnchecked(Call::This, &self);
    call.SetReference(0, one);
    call.Invoke();
    ErrorIf(self != one, "Call Failed");
  }
}

void RunCallTests()
{
  ReflectionLibrary& callLibrary = ReflectionProject::CreateLibrary("Call");
  BindTypeExternal(callLibrary, Number, 'numb', DummyBind);
  BindTypeExternal(callLibrary, SimpleGetterTest, 'sgt', DummyBind);
  BindTypeExternal(callLibrary, SimpleSetterTest, 'sst', DummyBind);
  callLibrary.Finalize();

  TestSimpleGetters();
  TestSimpleSetters();
  TestNumber();

  ReflectionProject::DestroyLibrary(callLibrary);
}
