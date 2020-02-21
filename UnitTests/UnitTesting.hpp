#pragma once


#include <fstream>
#include <sstream>

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
  BoundType* boundType = StaticTypeId<T>::GetBoundType();

  JsonSaver saver;
  saver.Serialize(input);
  std::string jsonData = saver.ToString();

  std::ofstream outStream;
  std::string fileName = boundType->mName;
  fileName += ".json";
  outStream.open(fileName.c_str(), std::ofstream::out);
  outStream << jsonData;

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

void RunUnitTests();
