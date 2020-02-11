#pragma once

#include "Bindings.hpp"



struct NameId
{
  NameId()
  {
  }
  NameId(int id, const std::string& name) : mId(id), mName(name)
  {
  }

  bool operator==(const NameId& rhs) const
  {
    return mId == rhs.mId && mName == rhs.mName;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameId, mId);
    BindField(library, boundType, NameId, mName);
  }

  int mId;
  std::string mName;
};

struct NameIdList
{
  bool operator==(const NameIdList& rhs) const
  {
    return mIds == rhs.mIds;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NameIdList, mIds);
  }

  std::vector<NameId> mIds;
};

struct NumberList
{
  bool operator==(const NumberList& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, NumberList, mData);
  }

  std::vector<int> mData;
};

struct Dictionary
{
  bool operator==(const Dictionary& rhs) const
  {
    return mData == rhs.mData;
  }
  static void Bind(MetaLibrary& library, BoundType& boundType)
  {
    BindField(library, boundType, Dictionary, mData);
  }
  
  void Add(const std::string& key, const std::string& value)
  {
    mData.insert(std::make_pair(key, value));
  }

  std::unordered_map<std::string, std::string> mData;
};

struct MyStruct
{
  int mData;
  float mFloat;
  char mChar1;
  int value = 5;
  char mChar2;
  double mDouble1;
  std::string mString1;

  bool operator==(const MyStruct& rhs) const
  {
    return mData == rhs.mData &&
      mFloat == rhs.mFloat &&
      mChar1 == rhs.mChar1 &&
      value == rhs.value &&
      mChar2 == rhs.mChar2 &&
      mDouble1 == rhs.mDouble1 &&
      mString1 == rhs.mString1;
  }
};

inline void BindMyStruct(MetaLibrary& library, BoundType& boundType)
{
  BindField(library, boundType, MyStruct, mData);
  BindField(library, boundType, MyStruct, mFloat);
  BindField(library, boundType, MyStruct, mChar1);
  BindField(library, boundType, MyStruct, mChar2);
  BindField(library, boundType, MyStruct, mDouble1);
  BindField(library, boundType, MyStruct, mString1);
}