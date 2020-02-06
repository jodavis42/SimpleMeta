
#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "BinaryStream.hpp"
#include "TestTypes.hpp"

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;
  std::string mString1;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, MyStruct, mData);
    BindField(library, boundType, MyStruct, mFloat);
    BindField(library, boundType, MyStruct, mChar1);
    BindField(library, boundType, MyStruct, mChar2);
    BindField(library, boundType, MyStruct, mDouble1);
    BindField(library, boundType, MyStruct, mString1);
  }
};

void BindMyStruct(MetaLibrary& library, BoundType& boundType)
{
  BindField(library, boundType, MyStruct, mData);
  BindField(library, boundType, MyStruct, mFloat);
  BindField(library, boundType, MyStruct, mChar1);
  BindField(library, boundType, MyStruct, mChar2);
  BindField(library, boundType, MyStruct, mDouble1);
  BindField(library, boundType, MyStruct, mString1);
}

template <typename T>
void TestRoundTrip(T& input)
{
  BinarySaver saver;
  saver.Serialize(input);

  T output;
  BinaryLoader loader;
  loader.mStream = std::move(saver.mStream);
  loader.Serialize(output);
}

int main()
{
  MetaLibrary library;
  BindPrimitiveType(library, char);
  BindPrimitiveType(library, int);
  BindPrimitiveType(library, float);
  BindPrimitiveType(library, double);
  BindPrimitiveType(library, Vec2);
  BindPrimitiveType(library, Vec3);
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
  BindType(library, Dictionary, 'dict');

  {
    Mesh outMesh;
    outMesh.mVertices.push_back(Vertex(Vec3(0, 0, 0), Vec2(0, 0)));
    outMesh.mVertices.push_back(Vertex(Vec3(1, 0, 0), Vec2(1, 0)));
    outMesh.mVertices.push_back(Vertex(Vec3(1, 1, 0), Vec2(1, 1)));
    outMesh.mVertices.push_back(Vertex(Vec3(0, 1, 0), Vec2(0, 1)));
    outMesh.mName = "Quad";

    TestRoundTrip(outMesh);
  }

  {
    MyStruct myData;
    myData.mChar1 = 1;
    myData.mChar2 = 15;
    myData.mFloat = 1.23f;
    myData.mData = -1;
    myData.mDouble1 = 0.123456789f;
    myData.mString1 = "MyString290as9gy0a9sdhg0asdhng0ashdg09ahsd0gahsg";

    TestRoundTrip(myData);
  }

  {
    NameIdList input;
    input.mIds.push_back(NameId(0, "Zero"));
    input.mIds.push_back(NameId(1, "One"));

    TestRoundTrip(input);
  }

  {
    Dictionary input;
    input.Add("ohio", "good morning");

    TestRoundTrip(input);
  }

   {
    SphereCollider* sphere1 = new SphereCollider();
    sphere1->mId = 1;
    sphere1->mRadius = 1.0f;
    BoxCollider* box1 = new BoxCollider();
    box1->mId = 3;
    box1->mSize = Vec3(1, 2, 3);

    PhysicsSpace input;
    input.mColliders.push_back(sphere1);
    input.mColliders.push_back(box1);

    TestRoundTrip(input);
  }

  return 0;
}
