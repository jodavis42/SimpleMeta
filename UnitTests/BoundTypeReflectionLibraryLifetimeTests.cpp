#include "AnyTests.hpp"

#include "Bindings.hpp"
#include "MetaSerialization.hpp"
#include "Any.hpp"

#include "UnitTesting.hpp"
#include "TestTypes/Math.hpp"
#include <sstream>

struct SubStruct
{
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, SubStruct, mFloat);
    BindProperty(library, boundType, SubStruct, mBool);
  }

  float mFloat;
  bool mBool;
};

struct LifetimeTest
{
  static void Bind(SimpleReflection::ReflectionLibrary& library, SimpleReflection::BoundType& boundType)
  {
    BindProperty(library, boundType, LifetimeTest, mSubStruct);
  }

  SubStruct mSubStruct;
};

void RunBoundTypeReflectionLibraryLifetimesTests()
{
  SimpleReflection::ReflectionLibrary* coreLibrary = SimpleReflection::ReflectionProject::FindLibrary("Core");

  for(size_t i = 0; i < 10; ++i)
  {
    std::stringstream stream;
    stream << "LifetimeTest" << i;
    SimpleReflection::ReflectionLibrary& lifetimeTestLibrary = SimpleReflection::ReflectionProject::CreateLibrary(stream.str());
    lifetimeTestLibrary.AddDependency(coreLibrary);
    // Create some native bound types
    BindType(lifetimeTestLibrary, SubStruct, 'ss');
    BindType(lifetimeTestLibrary, LifetimeTest, 'ltt');
    // Create a dynamic bound type (lifetime owned by the library)
    BoundType* boundType = new BoundType();
    boundType->mName = "Dynamic";
    boundType->mSizeInBytes = 1;
    lifetimeTestLibrary.AddBoundType(boundType);
    
    lifetimeTestLibrary.Finalize();
    SimpleReflection::ReflectionProject::DestroyLibrary(lifetimeTestLibrary);
  }

  {
    SimpleReflection::ScopedAssertHandler handler;
  
    SimpleReflection::ReflectionLibrary& library = SimpleReflection::ReflectionProject::CreateLibrary("DoublePrimitive");
    library.AddDependency(coreLibrary);
    // Try to double bind a primitive type (from a dependent library)
    BindPrimitiveType(library, float, 'flot');
    library.Finalize();
  
    handler.StopListening();
    ReflectionErrorIf(handler.mAsserts.size() == 0, "Expected asserts");
    SimpleReflection::ReflectionProject::DestroyLibrary(library);
  }
  
  {
    SimpleReflection::ScopedAssertHandler handler;
  
    SimpleReflection::ReflectionLibrary& library = SimpleReflection::ReflectionProject::CreateLibrary("DoubleType");
    library.AddDependency(coreLibrary);
    // Try to double bind a type
    BindType(library, SubStruct, 'ss');
    BindType(library, SubStruct, 'ss');
    library.Finalize();
  
    handler.StopListening();
    ReflectionErrorIf(handler.mAsserts.size() == 0, "Expected asserts");
    SimpleReflection::ReflectionProject::DestroyLibrary(library);
  }
}
