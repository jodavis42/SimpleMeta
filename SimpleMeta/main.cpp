#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "MetaSerialization.hpp"
#include "Attributes.hpp"
#include "TestTypes/Physics.hpp"
#include "TestTypes/Math.hpp"
#include "TestTypes/Mesh.hpp"
#include "TestTypes/Misc.hpp"
#include "ContainerSerializationPolicies.hpp"
#include "UnitTesting.hpp"
#include "DataDriving/DataDrivenTesting.hpp"

int main()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  ReflectionLibrary& reflectionLibrary = ReflectionProject::CreateLibrary("Reflection");
  CreateBoundType<ReflectionComponent>(reflectionLibrary, "ReflectionComponent", 'rcmp');
  CreateBoundType<Attribute>(reflectionLibrary, "Attribute", 'att');
  CreateBoundType<SerializedAttribute>(reflectionLibrary, "SerializedAttribute", 'attS');
  CreateBoundType<MetaSerialization>(reflectionLibrary, "MetaSerialization", 'mser');

  ReflectionLibrary& coreLibrary = ReflectionProject::CreateLibrary("Core");
  coreLibrary.AddDependency(&reflectionLibrary);
  CreateBoundType<void>(coreLibrary, "void", 1, 0);
  BindPrimitiveType(coreLibrary, bool, 2);
  BindPrimitiveType(coreLibrary, char, 3);
  BindPrimitiveType(coreLibrary, int, 4);
  BindPrimitiveType(coreLibrary, float, 5);
  BindPrimitiveType(coreLibrary, double, 6);
  BindPrimitiveTypeAs(coreLibrary, std::string, "string", 7);
  BindType(coreLibrary, Vec2, 'vec2');
  BindType(coreLibrary, Vec3, 'vec3');
  
  ReflectionLibrary& miscLibrary = ReflectionProject::CreateLibrary("Misc");
  miscLibrary.AddDependency(&coreLibrary);
  BindTypeExternal(miscLibrary, MyStruct, 'myst', BindMyStruct);
  BindType(miscLibrary, Mesh, 'mesh');
  BindType(miscLibrary, Vertex, 'vtx');
  BindType(miscLibrary, NameId, 'nid');
  BindType(miscLibrary, NameIdList, 'idlt');
  BindType(miscLibrary, Collider, 'col');
  BindType(miscLibrary, BoxCollider, 'bcol');
  BindType(miscLibrary, SphereCollider, 'scol');
  BindType(miscLibrary, PhysicsSpace, 'phys');
  BindType(miscLibrary, NumberList, 'nmls');
  BindType(miscLibrary, Dictionary, 'dict');
  miscLibrary.Finalize();

  RunUnitTests();

  {
    ReflectionLibrary& dataDrivenLibrary = ReflectionProject::CreateLibrary("DataDriven");
    dataDrivenLibrary.AddDependency(&miscLibrary);
    BindType(dataDrivenLibrary, DataDrivenField, 'ddf');
    BindType(dataDrivenLibrary, DataDrivenType, 'ddt');
    BindType(dataDrivenLibrary, DataDrivenTypes, 'ddts');
    dataDrivenLibrary.Finalize();
 
    std::string testDir = "DataDrivenTests";
    RunDataDrivenTests(testDir, dataDrivenLibrary);
  }

  return 0;
}
