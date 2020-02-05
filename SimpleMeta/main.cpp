
#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "StaticTypeId.hpp"
#include "Bindings.hpp"
#include "BinaryStream.hpp"

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, MyStruct, mData);
    BindField(library, boundType, MyStruct, mFloat);
    BindField(library, boundType, MyStruct, mChar1);
    BindField(library, boundType, MyStruct, mChar2);
    BindField(library, boundType, MyStruct, mDouble1);
  }
};

struct DynamicArray
{
  std::vector<int> mArray;

  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, DynamicArray, mArray);
  }
};

void BindMyStruct(MetaLibrary& library, BoundType& boundType)
{
  BindField(library, boundType, MyStruct, mData);
  BindField(library, boundType, MyStruct, mFloat);
  BindField(library, boundType, MyStruct, mChar1);
  BindField(library, boundType, MyStruct, mChar2);
  BindField(library, boundType, MyStruct, mDouble1);
}

int main()
{
  MetaLibrary library;
  BindPrimitiveType(library, char);
  BindPrimitiveType(library, int);
  BindPrimitiveType(library, float);
  BindPrimitiveType(library, double);
  BindTypeExternal(library, MyStruct, BindMyStruct);
  BindType(library, DynamicArray);

  MyStruct myData;
  myData.mChar1 = 1;
  myData.mChar2 = 15;
  myData.mFloat = 1.23f;
  myData.mData = -1;
  myData.mDouble1 = 0.000000001f;

  BinaryStream stream;
  std::vector<char> streamData;
  stream.Write(myData, streamData);
  MyStruct myNewData;
  myNewData.value = 1;
  stream.Read(streamData, myNewData);

  return 0;
}
