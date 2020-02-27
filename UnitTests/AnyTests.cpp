#include "AnyTests.hpp"

#include "Bindings.hpp"
#include "Any.hpp"

#include "UnitTesting.hpp"
#include "TestTypes/Math.hpp"

struct LargeType
{
  LargeType()
  {
    Build(0);
  }
  LargeType(int offset)
  {
    Build(offset);
  }
  void Build(int offset)
  {
    for(size_t i = 0; i < mCount; ++i)
    {
      float val = static_cast<float>(i + offset);
      mData[i] = Vec3(val, val, val);
    }
  }
  bool operator==(const LargeType& rhs) const
  {
    bool isEqual = true;
    for(size_t i = 0; i < mCount; ++i)
    {
      isEqual &= mData[i] == rhs.mData[i];
    }
    return isEqual;
  }
  static constexpr size_t mCount = 100;
  Vec3 mData[mCount];
};

struct DeepCopyType
{
  DeepCopyType()
  {
    mData = new char[1];
    mData[0] = 1;
  }
  DeepCopyType(char value)
  {
    mData = new char[1];
    mData[0] = value;
  }
  DeepCopyType(const DeepCopyType& rhs)
  {
    mData = new char[1];
    mData[0] = rhs.mData[0];
  }
  ~DeepCopyType()
  {
    delete mData;
  }

  bool operator==(const DeepCopyType& rhs) const
  {
    return mData[0] == rhs.mData[0];
  }

  char* mData = nullptr;
};

template <typename T>
void TestSimple(const T& input1, const T& input2)
{
  Any any;
  any.Set(input1);

  // Scope this to make sure deep copies work
  {
    T output1 = any.Get<T>();
    ErrorIf(!(input1 == output1), "Any Get/Set didn't match.");
  }

  Any any2 = any;
  T output2 = any2.Get<T>();
  ErrorIf(!(input1 == output2), "Any CopyConstructor failed.");

//  ErrorIf(!(any == any2), "Operator== failed. Any's should be equal.");

  Any any3;
  ErrorIf(any == any3, "Operator== failed. Any's should have unrelated bound types.");

  any3.Set(input2);
  ErrorIf(any == any3, "Operator== failed. Any's should have different data.");
}

void TestPrimitive()
{
  TestSimple(1324, 1);
}

void TestMathType()
{
  TestSimple(Vec2(0, 0), Vec2(1, 3));
}

void TestLargeType()
{
  TestSimple(LargeType(), LargeType(3));
}

void TestDeepCopyType()
{
  DeepCopyType a(1);
  DeepCopyType b(2);
  TestSimple(a, b);
}

void RunAnyTests()
{
  ReflectionLibrary& anyLibrary = ReflectionProject::CreateLibrary("Any");
  anyLibrary.AddDependency(ReflectionProject::FindLibrary("Core"));
  BindTypeExternal(anyLibrary, LargeType, 'lrgt', DummyBind);
  BindTypeExternal(anyLibrary, DeepCopyType, 'dpcp', DummyBind);
  anyLibrary.Finalize();

  TestPrimitive();
  TestMathType();
  TestLargeType();
  TestDeepCopyType();

  ReflectionProject::DestroyLibrary(anyLibrary);
}
