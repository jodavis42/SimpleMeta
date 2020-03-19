#include "AnyTests.hpp"

#include "Bindings.hpp"
#include "MetaSerialization.hpp"
#include "Any.hpp"

#include "UnitTesting.hpp"
#include "TestTypes/Math.hpp"

using SimpleReflection::ReflectionLibrary;
using SimpleReflection::ReflectionProject;
using SimpleReflection::BoundType;

struct GetSetTest
{
  GetSetTest()
  {

  }
  GetSetTest(const Vec3& value1, const Vec3& value2, const Vec3& value3)
    : mValue1(value1)
    , mValue2(value2)
    , mValue3(value3)
  {

  }

  Vec3 GetValue1() const
  {
    return mValue1;
  }
  void SetValue2(const Vec3& value2)
  {
    mValue2 = value2;
  }
  Vec3 GetValue3() const
  {
    return mValue3;
  }
  void SetValue3(const Vec3& value3)
  {
    mValue3 = value3;
  }

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindGetter(library, boundType, GetSetTest, Value1);
    BindSetter(library, boundType, GetSetTest, Value2);
    BindGetterSetter(library, boundType, GetSetTest, Value3);
  }

  Vec3 mValue1;
  Vec3 mValue2;
  Vec3 mValue3;
};

struct FullGetSetTest
{
  FullGetSetTest()
  {

  }
  FullGetSetTest(const float& scalar)
    : mScalar(scalar)
  {

  }

  float GetScalar() const
  {
    return mScalar;
  }
  void SetScalar(float scalar)
  {
    mScalar = scalar;
  }
  // Simple functions to force overload resolution to be necesarry
  float GetScalar(float multiplier) const
  {
    return mScalar * multiplier;
  }
  void SetScalar(float scalar, float multiplier)
  {
    mScalar = scalar * multiplier;
  }

  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    FullBindGetterAs(library, boundType, FullGetSetTest, &FullGetSetTest::GetScalar, ReflectionConstInstanceOverload(FullGetSetTest, float), "ScalarG");
    FullBindSetterAs(library, boundType, FullGetSetTest, &FullGetSetTest::SetScalar, ReflectionInstanceOverload(FullGetSetTest, void, float), "ScalarS");
    FullBindGetterSetterAs(library, boundType, FullGetSetTest,
      &FullGetSetTest::GetScalar, ReflectionConstInstanceOverload(FullGetSetTest, float),
      &FullGetSetTest::SetScalar, ReflectionInstanceOverload(FullGetSetTest, void, float),
      "ScalarGS");
  }

  float mScalar = 0;
};

SimpleReflection::GetterSetter* FindGetterSetter(SimpleReflection::BoundType* boundType, const std::string& name)
{
  for(auto range = boundType->GetGetterSetters(); !range.Empty(); range.PopFront())
  {
    if(range.Front()->mName == name)
      return range.Front();
  }
  return nullptr;
}

void TestGets()
{
  GetSetTest test(Vec3(1, 2, 3), Vec3(4, 5, 6), Vec3(7, 8, 9));
  SimpleReflection::BoundType* boundType = StaticTypeId<GetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "Value1");
  
  Any any = getSet->Get(&test);
  Vec3 getData = any.Get<Vec3>();
  ReflectionErrorIf(!(getData == test.mValue1), "Getter failed");
}

void TestSets()
{
  GetSetTest test(Vec3(1, 2, 3), Vec3(4, 5, 6), Vec3(7, 8, 9));
  SimpleReflection::BoundType* boundType = StaticTypeId<GetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "Value2");

  Vec3 setData(-1, -2, -3);
  Any any;
  any.Set(setData);
  getSet->Set(&test, any);
  ReflectionErrorIf(!(setData == test.mValue2), "Setter failed");
}

void TestGetSets()
{
  GetSetTest test(Vec3(1, 2, 3), Vec3(4, 5, 6), Vec3(7, 8, 9));
  SimpleReflection::BoundType* boundType = StaticTypeId<GetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "Value3");

  Vec3 setData(-1, -2, -3);
  Any any;
  any.Set(setData);
  getSet->Set(&test, any);
  ReflectionErrorIf(!(setData == test.mValue3), "Setter failed");

  any = getSet->Get(&test);
  Vec3 getData = any.Get<Vec3>();
  ReflectionErrorIf(!(getData == test.mValue3), "Getter failed");
}

void TestFullGets()
{
  FullGetSetTest test(1);
  SimpleReflection::BoundType* boundType = StaticTypeId<FullGetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "ScalarG");

  Any any = getSet->Get(&test);
  float getData = any.Get<float>();
  ReflectionErrorIf(!(getData == test.mScalar), "Getter failed");
}

void TestFullSets()
{
  FullGetSetTest test(1);
  SimpleReflection::BoundType* boundType = StaticTypeId<FullGetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "ScalarS");

  float setData = -1;
  Any any;
  any.Set(setData);
  getSet->Set(&test, any);
  ReflectionErrorIf(!(setData == test.mScalar), "Setter failed");
}

void TestFullGetSets()
{
  FullGetSetTest test(0);
  SimpleReflection::BoundType* boundType = StaticTypeId<FullGetSetTest>::GetBoundType();
  SimpleReflection::GetterSetter* getSet = FindGetterSetter(boundType, "ScalarGS");

  float setData = 3;
  Any any;
  any.Set(setData);
  getSet->Set(&test, any);
  ReflectionErrorIf(!(setData == test.mScalar), "Setter failed");

  any = getSet->Get(&test);
  float getData = any.Get<float>();
  ReflectionErrorIf(!(getData == test.mScalar), "Getter failed");
}

void RunGetSetTests()
{
  ReflectionLibrary& getSetLibrary = ReflectionProject::CreateLibrary("GetSet");
  getSetLibrary.AddDependency(ReflectionProject::FindLibrary("Core"));
  BindType(getSetLibrary, GetSetTest, 'gst');
  BindType(getSetLibrary, FullGetSetTest, 'fgst');
  getSetLibrary.Finalize();

  TestGets();
  TestSets();
  TestGetSets();

  TestFullGets();
  TestFullSets();
  TestFullGetSets();

  ReflectionProject::DestroyLibrary(getSetLibrary);
}
