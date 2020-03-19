#include "AnyTests.hpp"

#include "Bindings.hpp"
#include "ReflectionLibrary.hpp"

#include "UnitTesting.hpp"
#include "TestTypes/Math.hpp"

float OperatorGet(const Vec3* self, size_t index)
{
  if(index == 0)
    return self->x;
  else if(index == 1)
    return self->y;
  else
    return self->z;
}

void OperatorSet(Vec3* self, size_t index, float value)
{
  if(index == 0)
    self->x = value;
  else if(index == 1)
    self->y = value;
  else
    self->z = value;
}

float LengthSq(Vec3* self)
{
  return self->x * self->x + self->y * self->y + self->z * self->z;
}

float TestGet(SimpleReflection::ReflectionLibrary& library, Vec3& self, size_t index)
{
  SimpleReflection::FunctionType fnType;
  fnType.SetReturnType(ReflectionBoundTypeOf(float));
  fnType.SetThisType(ReflectionBoundTypeOf(const Vec3*));
  fnType.SetParamType(0, ReflectionBoundTypeOf(size_t));
  SimpleReflection::Function* function = library.FindExtensionFunction(SimpleReflection::StaticTypeId<Vec3>::GetBoundType(), "Get", fnType);

  SimpleReflection::Call call(function);
  call.SetPointerUnchecked(SimpleReflection::Call::This, &self);
  call.Set(0, index);
  call.Invoke();
  return call.Get<float>(SimpleReflection::Call::Return);
}

void TestSet(SimpleReflection::ReflectionLibrary& library, Vec3& self, size_t index, float value)
{
  SimpleReflection::FunctionType fnType;
  fnType.SetReturnType(ReflectionBoundTypeOf(void));
  fnType.SetThisType(ReflectionBoundTypeOf(const Vec3*));
  fnType.SetParamType(0, ReflectionBoundTypeOf(size_t));
  fnType.SetParamType(1, ReflectionBoundTypeOf(float));
  SimpleReflection::Function* function = library.FindExtensionFunction(SimpleReflection::StaticTypeId<Vec3>::GetBoundType(), "Set", fnType);

  SimpleReflection::Call call(function);
  call.SetPointerUnchecked(SimpleReflection::Call::This, &self);
  call.Set(0, index);
  call.Set(1, value);
  call.Invoke();
}

void TestGetSet(SimpleReflection::ReflectionLibrary& library)
{
  Vec3 self(1, 2, 3);
  float result = TestGet(library, self, 0);
  ReflectionErrorIf(result != self.x, "Invalid extension get");

  TestSet(library, self, 2, result);
  ReflectionErrorIf(result != self.z, "Invalid extension set");
}

void TestLengthSq(SimpleReflection::ReflectionLibrary& library)
{
  Vec3 self(1, 2, 3);
  
  SimpleReflection::FunctionType fnType;
  fnType.SetReturnType(ReflectionBoundTypeOf(float));
  fnType.SetThisType(ReflectionBoundTypeOf(const Vec3*));
  SimpleReflection::Function* function = library.FindExtensionFunction(SimpleReflection::StaticTypeId<Vec3>::GetBoundType(), "LengthSq", fnType);

  SimpleReflection::Call call(function);
  call.SetPointerUnchecked(SimpleReflection::Call::This, &self);
  call.Invoke();

  float length = call.Get<float>(SimpleReflection::Call::Return);
  float expectedLength = 1 + 4 + 9;
  ReflectionErrorIf(length != expectedLength, "Computed length is incorrect");
}

void RunExtensionFunctionTests()
{
  SimpleReflection::ReflectionLibrary& extensionLibrary1 = SimpleReflection::ReflectionProject::CreateLibrary("Extension1");
  extensionLibrary1.AddDependency(SimpleReflection::ReflectionProject::FindLibrary("Core"));
  BindExtensionFunctionAs(extensionLibrary1, Vec3, OperatorGet, "Get");
  BindExtensionFunctionAs(extensionLibrary1, Vec3, OperatorSet, "Set");
  extensionLibrary1.Finalize();

  SimpleReflection::ReflectionLibrary& extensionLibrary2 = SimpleReflection::ReflectionProject::CreateLibrary("Extension2");
  extensionLibrary2.AddDependency(&extensionLibrary1);
  BindExtensionFunction(extensionLibrary2, Vec3, LengthSq);
  extensionLibrary2.Finalize();

  TestGetSet(extensionLibrary2);
  TestLengthSq(extensionLibrary2);

  SimpleReflection::ReflectionProject::DestroyLibrary(extensionLibrary2);
  SimpleReflection::ReflectionProject::DestroyLibrary(extensionLibrary1);
}
