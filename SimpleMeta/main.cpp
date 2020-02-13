
#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "BinaryStream.hpp"
#include "TestTypes/Physics.hpp"
#include "TestTypes/Math.hpp"
#include "TestTypes/Mesh.hpp"
#include "TestTypes/Misc.hpp"
#include "TestTypes/Actor.hpp"
#include "TestTypes/ActorComponents.hpp"
#include "JsonSerializers.hpp"
#include <fstream>
#include <chrono>
#include <iterator>
#include <iostream>

template <typename T>
void TestBinaryRoundTrip(T& input, const std::string& filename)
{
  BoundType* boundType = StaticTypeId<T>::GetBoundType();

  std::ofstream outStream;
  outStream.open(filename, std::ofstream::binary | std::ofstream::out);
  BinarySaver saver(outStream);
  saver.Serialize(input);
  outStream.close();

  std::ifstream inStream;
  inStream.open(filename, std::ifstream::binary | std::ifstream::in);
  std::stringstream inMemoryStream;
  inMemoryStream << inStream.rdbuf();

  T output;
  BinaryLoader loader(inMemoryStream);
  auto currentTime = std::chrono::steady_clock::now();
  loader.Serialize(output);
  auto deltaTime = std::chrono::steady_clock::now() - currentTime;
  std::cout << "Time to load " << filename << " " << deltaTime.count() << "\n";

  if(!(input == output))
  {
    BoundType* boundType = StaticTypeId<T>::GetBoundType();
    printf("Type '%s' failed to serialize correctly\n", boundType->mName.c_str());
    __debugbreak();
  }
}

template <typename T>
void TestJsonRoundTrip(T& input, const std::string& filename)
{
  JsonSaver saver;
  saver.Serialize(input);
  std::string jsonData = saver.ToString();

  std::ofstream outStream;
  outStream.open(filename, std::ofstream::out);
  outStream << jsonData;
  outStream.close();

  T output;
  JsonLoader loader;
  auto currentTime = std::chrono::steady_clock::now();
  loader.Serialize(jsonData, output);
  auto deltaTime = std::chrono::steady_clock::now() - currentTime;
  std::cout << "Time to load " << filename << " " << deltaTime.count() << "\n";

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
  BindTypeAs(library, BoxCollider, "minecraft:box_collision", 'bcol');
  BindTypeAs(library, SphereCollider, "minecraft:sphere_collision", 'scol');
  BindType(library, PhysicsSpace, 'phys');
  BindType(library, NumberList, 'nmls');
  BindType(library, Dictionary, 'dict');
  BindType(library, AnimationKeyFrame, 'keyf');
  BindType(library, AnimationBone, 'bone');
  BindType(library, AnimationEffect, 'effe');
  BindType(library, Animation, 'anim');
  BindType(library, AnimationDictionary, 'anid');

  BindType(library, ActorDefinitionAsset, 'acta');
  BindType(library, ActorDefinition, 'actr');
  BindType(library, ActorDescription, 'desc');
  BindType(library, ActorComponent, 'acom');
  BindTypeAs(library, BreedableComponent, "minecraft:breedable", 'bred');
  BindTypeAs(library, FamilyComponent, "minecraft:type_family", 'fami');
  BindTypeAs(library, AttackDamageComponent, "minecraft:attack_damage", 'dmg ');
  BindTypeAs(library, CollisionBoxComponent, "minecraft:collision_box", 'coll');
  BindTypeAs(library, VariantComponent, "minecraft:variant", 'var ');

  {
    Mesh outMesh;
    for (int vertIndex = 0; vertIndex < 15000; ++vertIndex) {
        outMesh.mVertices.push_back(
            Vertex(
                Vec3(rand()/static_cast<float>(RAND_MAX), rand()/static_cast<float>(RAND_MAX), rand()/static_cast<float>(RAND_MAX)),
                Vec2(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX))));
    }

    outMesh.mName = "Quad";
  
    TestJsonRoundTrip(outMesh, "mesh.json");
    TestBinaryRoundTrip(outMesh, "mesh.bed");
  }
  
  {
    MyStruct myData;
    myData.mChar1 = 1;
    myData.mChar2 = 15;
    myData.mFloat = 1.23f;
    myData.mData = -1;
    myData.mDouble1 = 0.123456789f;
    myData.mString1 = "MyString290as9gy0a9sdhg0asdhng0ashdg09ahsd0gahsg";
  
    TestJsonRoundTrip(myData, "mystruct.json");
    TestBinaryRoundTrip(myData, "mystruct.bed");
  }
  
  {
    NumberList input;
    input.mData.push_back(1);
    input.mData.push_back(2);
    input.mData.push_back(1);
    input.mData.push_back(5);

    TestJsonRoundTrip(input, "numberlist.json");
    TestBinaryRoundTrip(input, "numberlist.bed");
  }

  {
    NameIdList input;
    input.mIds.push_back(NameId(0, "Zero"));
    input.mIds.push_back(NameId(1, "One"));
  
    TestJsonRoundTrip(input, "nameidlist.json");
    TestBinaryRoundTrip(input, "nameidlist.bed");
  }
  
  {
    Dictionary input;
    input.Add("ohio", "good morning");
    input.Add("konbanwa", "good evening");
    input.Add("taberu", "to eat");
    input.Add("tanoshi", "fun");
  
    TestJsonRoundTrip(input, "dictionary.json");
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

    TestJsonRoundTrip(input, "polymorphic.json");
    TestBinaryRoundTrip(input, "polymorphic.bed");
  }

  {
      AnimationKeyFrame key1;
      key1.lerpMode = "catmullrom";
      key1.postTransform.push_back(0.1f);
      key1.postTransform.push_back(0.3f);
      key1.postTransform.push_back(1.0f);

      AnimationBone boneBody, boneHead;
      boneBody.positions["0.0"] = key1;
      boneBody.positions["0.6"] = key1;
      boneHead.rotations["0.0"] = key1;
      boneHead.rotations["1.0"] = key1;

      AnimationDictionary animations;
      Animation wolfAttack;
      wolfAttack.blendWeight = 0.5f;
      wolfAttack.animTimeUpdate = "query.anim_time";
      wolfAttack.looping = true;
      wolfAttack.overridePreviousAnimation = true;
      wolfAttack.bones["body"] = boneBody;
      wolfAttack.bones["head"] = boneHead;

      animations.formatVersion = "1.8.0";
      animations.animations["wolf.attack"] = wolfAttack;
      animations.animations["wolf.beg"] = wolfAttack;

      TestJsonRoundTrip(animations, "animations.json");
      TestBinaryRoundTrip(animations, "animations.bed");
  }

  //{
  //    AnimationDictionary wolfAnimations;
  //    JsonLoader loader;

  //    std::fstream stream;
  //    stream.open("wolf.animations.json", std::ofstream::in);

  //    std::ostringstream ss;
  //    ss << stream.rdbuf(); // reading data
  //    std::string jsonData = ss.str();
  //    stream.close();

  //    loader.Serialize(jsonData, wolfAnimations);

  //    TestJsonRoundTrip(wolfAnimations, "wolf_animations_generated.json");
  //    TestBinaryRoundTrip(wolfAnimations, "wolf_animations.bed");
  //}

  //{
  //    ActorDefinitionAsset catDefinition;
  //    JsonLoader loader;

  //    std::fstream stream;
  //    stream.open("cat.entity.json", std::ofstream::in);

  //    std::ostringstream ss;
  //    ss << stream.rdbuf();
  //    std::string jsonData = ss.str();

  //    loader.Serialize(jsonData, catDefinition);
  //}

  return 0;
}
