#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "TestTypes/Physics.hpp"
#include "TestTypes/Math.hpp"
#include "TestTypes/Mesh.hpp"
#include "TestTypes/Misc.hpp"
#include "ContainerSerializationPolicies.hpp"
#include "UnitTesting.hpp"

int main()
{
  ReflectionLibrary& coreLibrary = ReflectionProject::CreateLibrary("Core");
  BindPrimitiveType(coreLibrary, char);
  BindPrimitiveType(coreLibrary, int);
  BindPrimitiveType(coreLibrary, float);
  BindPrimitiveType(coreLibrary, double);
  BindType(coreLibrary, Vec2, 'vec2');
  BindType(coreLibrary, Vec3, 'vec3');
  BindPrimitiveTypeAs(coreLibrary, std::string, "string");

  ReflectionLibrary& miscLibrary = ReflectionProject::CreateLibrary("Misc");
  miscLibrary.AddDependency(&coreLibrary);
  BindTypeExternal(miscLibrary, MyStruct, 'myst', BindMyStruct);
  BindType(miscLibrary, Vertex, 'vtx');
  BindType(miscLibrary, Mesh, 'mesh');
  BindType(miscLibrary, NameId, 'nid');
  BindType(miscLibrary, NameIdList, 'idlt');
  BindType(miscLibrary, Collider, 'col');
  BindType(miscLibrary, BoxCollider, 'bcol');
  BindType(miscLibrary, SphereCollider, 'scol');
  BindType(miscLibrary, PhysicsSpace, 'phys');
  BindType(miscLibrary, NumberList, 'nmls');
  BindType(miscLibrary, Dictionary, 'dict');

  RunUnitTests();

  return 0;
}
