
#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "BinaryStream.hpp"
#include "TestTypes/Physics.hpp"
#include "TestTypes/Math.hpp"
#include "TestTypes/Mesh.hpp"
#include "TestTypes/Misc.hpp"
#include "JsonSerializers.hpp"
#include <fstream>

template <typename T>
void TestBinaryRoundTrip(T& input)
{
  BoundType* boundType = StaticTypeId<T>::GetBoundType();

  std::ofstream outStream;
  outStream.open(boundType->mName, std::ofstream::binary | std::ofstream::out);
  BinarySaver saver(outStream);
  saver.Serialize(input);
  outStream.close();

  std::ifstream inStream;
  inStream.open(boundType->mName, std::ifstream::binary | std::ifstream::in);

  T output;
  BinaryLoader loader(inStream);
  loader.Serialize(output);

  if(!(input == output))
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    printf("Type '%s' failed to serialize correctly\n", boundType->mName.c_str());
    __debugbreak();
  }
}

template <typename T>
void TestJsonRoundTrip(T& input)
{
  JsonSaver saver;
  saver.Serialize(input);
  std::string jsonData = saver.ToString();

  T output;
  JsonLoader loader;
  loader.Serialize(jsonData, output);

  if(!(input == output))
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    printf("Type '%s' failed to serialize correctly\n", boundType->mName.c_str());
    __debugbreak();
  }
}

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

  {
    Mesh outMesh;
    outMesh.mVertices.push_back(Vertex(Vec3(0, 0, 0), Vec2(0, 0)));
    outMesh.mVertices.push_back(Vertex(Vec3(1, 0, 0), Vec2(1, 0)));
    outMesh.mVertices.push_back(Vertex(Vec3(1, 1, 0), Vec2(1, 1)));
    outMesh.mVertices.push_back(Vertex(Vec3(0, 1, 0), Vec2(0, 1)));
    outMesh.mName = "Quad";
  
    TestJsonRoundTrip(outMesh);
    TestBinaryRoundTrip(outMesh);
  }
  
  {
    MyStruct myData;
    myData.mChar1 = 1;
    myData.mChar2 = 15;
    myData.mFloat = 1.23f;
    myData.mData = -1;
    myData.mDouble1 = 0.123456789f;
    myData.mString1 = "MyString290as9gy0a9sdhg0asdhng0ashdg09ahsd0gahsg";
  
    TestJsonRoundTrip(myData);
    TestBinaryRoundTrip(myData);
  }
  
  {
    NumberList input;
    input.mData.push_back(1);
    input.mData.push_back(2);
    input.mData.push_back(1);
    input.mData.push_back(5);

    TestJsonRoundTrip(input);
    TestBinaryRoundTrip(input);
  }

  {
    NameIdList input;
    input.mIds.push_back(NameId(0, "Zero"));
    input.mIds.push_back(NameId(1, "One"));
  
    TestJsonRoundTrip(input);
    TestBinaryRoundTrip(input);
  }
  
  {
    Dictionary input;
    input.Add("ohio", "good morning");
    input.Add("konbanwa", "good evening");
    input.Add("taberu", "to eat");
    input.Add("tanoshi", "fun");
  
    TestJsonRoundTrip(input);
    TestBinaryRoundTrip(input);
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

    TestJsonRoundTrip(input);
    TestBinaryRoundTrip(input);
  }

  return 0;
}
