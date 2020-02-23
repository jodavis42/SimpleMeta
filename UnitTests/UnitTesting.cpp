#include "UnitTesting.hpp"

#include "AnyTests.hpp"
#include "CallTests.hpp"
#include "BinaryStream.hpp"
#include "JsonSerializers.hpp"
#include "TestTypes/Physics.hpp"
#include "TestTypes/Math.hpp"
#include "TestTypes/Mesh.hpp"
#include "TestTypes/Misc.hpp"

ScopedFunction::ScopedFunction(Function* function) : mFunction(function)
{

}

ScopedFunction::~ScopedFunction()
{
  delete mFunction;
}

ScopedFunction::operator Function*()
{
  return mFunction;
}

void DummyBind(ReflectionLibrary& library, BoundType& boundType)
{

}

void RunUnitTests()
{
  RunAnyTests();
  RunCallTests();

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
    SphereCollider* sphere2 = new SphereCollider();
    sphere2->mId = 4;
    sphere2->mRadius = 1.2f;
    sphere2->SetGhost(true);
    sphere2->SetSendsEvents(true);

    PhysicsSpace input;
    input.mColliders.push_back(sphere1);
    input.mColliders.push_back(box1);
    input.mColliders.push_back(sphere2);

    TestJsonRoundTrip(input);
    TestBinaryRoundTrip(input);
  }
}
