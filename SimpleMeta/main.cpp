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
  MetaLibrary library;
  BindPrimitiveType(library, char);
  BindPrimitiveType(library, int);
  BindPrimitiveType(library, float);
  BindPrimitiveType(library, double);
  BindType(library, Vec2, 'vec2');
  BindType(library, Vec3, 'vec3');
  BindPrimitiveTypeAs(library, std::string, "string");
  BindTypeExternal(library, MyStruct, 'myst', BindMyStruct);
  BindType(library, Vertex, 'vtx');
  BindType(library, Mesh, 'mesh');
  BindType(library, NameId, 'nid');
  BindType(library, NameIdList, 'idlt');
  BindType(library, Collider, 'col');
  BindType(library, BoxCollider, 'bcol');
  BindType(library, SphereCollider, 'scol');
  BindType(library, PhysicsSpace, 'phys');
  BindType(library, NumberList, 'nmls');
  BindType(library, Dictionary, 'dict');

  RunUnitTests();

  return 0;
}
