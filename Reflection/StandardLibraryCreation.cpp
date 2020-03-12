#include "Precompiled.hpp"

#include "StandardLibraryCreation.hpp"

#include "ReflectionLibrary.hpp"
#include "Bindings.hpp"

namespace SimpleReflection
{

ReflectionLibrary* CreateReflectionLibrary(const std::vector<ReflectionLibrary*>& dependencies, bool finalize)
{
  ReflectionLibrary& reflectionLibrary = ReflectionProject::CreateLibrary("Reflection");
  for(ReflectionLibrary* dependency : dependencies)
    reflectionLibrary.AddDependency(dependency);

  CreateBoundType<ReflectionComponent>(reflectionLibrary, "ReflectionComponent", 'rcmp');
  if(finalize)
    reflectionLibrary.Finalize();
  return &reflectionLibrary;
}

ReflectionLibrary* CreateCoreLibrary(const std::vector<ReflectionLibrary*>& dependencies, bool finalize)
{
  ReflectionLibrary& coreLibrary = ReflectionProject::CreateLibrary("Core");
  for(ReflectionLibrary* dependency : dependencies)
    coreLibrary.AddDependency(dependency);

  size_t id = 1;
  CreateBoundType<void>(coreLibrary, "void", id++, 0);
  BindPrimitiveType(coreLibrary, bool, id++);
  BindPrimitiveType(coreLibrary, char, id++);
  BindPrimitiveType(coreLibrary, unsigned char, id++);
  BindPrimitiveType(coreLibrary, short, id++);
  BindPrimitiveType(coreLibrary, unsigned short, id++);
  BindPrimitiveType(coreLibrary, int, id++);
  BindPrimitiveType(coreLibrary, unsigned int, id++);
  BindPrimitiveType(coreLibrary, float, id++);
  BindPrimitiveType(coreLibrary, double, id++);

  if(finalize)
    coreLibrary.Finalize();

  return &coreLibrary;
}

}//namespace SimpleReflection
