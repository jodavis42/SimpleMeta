#pragma once

#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "MetaSerialization.hpp"
#include "StaticTypeId.hpp"

template <typename FieldPointer, FieldPointer field, typename Class, typename FieldType>
static void FromField(MetaLibrary& library, BoundType& owner, const std::string& name, FieldType Class::*dummy, size_t offset)
{
  Field f = Field();
  f.mName = name;
  f.mOffset = offset;
  f.mType = StaticTypeId<FieldType>::GetBoundType();

  owner.mFields.push_back(f);
}

template <typename ArrayPointer, ArrayPointer field, typename Class, typename FieldType>
static void FromField(MetaLibrary& library, BoundType& owner, const std::string& name, std::vector<FieldType> Class::*dummy, size_t offset)
{
  Field f = Field();
  f.mName = name;
  f.mOffset = offset;
  f.mType = StaticTypeId<std::vector<FieldType>>::GetBoundType();
  f.mType->mMetaSerialization = new ArrayMetaSerialization<FieldType>();

  BoundType* subType = StaticTypeId<FieldType>::GetBoundType();
  Field subField;
  subField.mType = subType;
  f.mType->mFields.push_back(subField);
  f.mType->mStride = sizeof(FieldType);

  owner.mFields.push_back(f);
}

template <typename MapPointer, MapPointer field, typename Class, typename KeyType, typename ValueType>
static void FromField(MetaLibrary& library, BoundType& owner, const std::string& name, std::unordered_map<KeyType, ValueType> Class::*dummy, size_t offset)
{
  typedef std::unordered_map<KeyType, ValueType> MapType;
  Field f = Field();
  f.mName = name;
  f.mOffset = offset;
  f.mType = StaticTypeId<MapType>::GetBoundType();
  f.mType->mMetaSerialization = new MapMetaSerialization<KeyType, ValueType>();

  //BoundType* subType = StaticTypeId<FieldType>::GetBoundType();
  //Field subField;
  //subField.mType = subType;
  //f.mType->mFields.push_back(subField);
  //f.mType->mStride = sizeof(FieldType);

  owner.mFields.push_back(f);
}

template <typename ClassType, void (*BindingFn)(MetaLibrary& library, BoundType&)>
void BindClassType(MetaLibrary& library, const std::string& className, int id)
{
  BoundType* boundType = StaticTypeId<ClassType>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(ClassType);
  boundType->mId = id;
  BindingFn(library, *boundType);
  library.AddBoundType(boundType);
}

template <typename Type>
void BindPrimitiveTypeToLibrary(MetaLibrary& library, const std::string& className)
{
  BoundType* boundType = StaticTypeId<Type>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(Type);
  boundType->mMetaSerialization = new PrimitiveMetaSerialization<Type>();
  library.AddBoundType(boundType);
}

template <typename BaseType>
void BindBaseType(MetaLibrary& library, BoundType& derrivedType)
{
  BoundType* baseType = StaticTypeId<BaseType>::GetBoundType();
  derrivedType.mBaseType = baseType;
}

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember));
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName) BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName)
#define BindPrimitiveType(Library, PrimitiveType) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType)

#define BindTypeAs(Library, ClassType, ClassTypeName, Id) BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName, Id)
#define BindType(Library, ClassType, Id) BindTypeAs(Library, ClassType, #ClassType, Id)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, Id, BindingFn) BindClassType<ClassType, BindingFn>(Library, ClassTypeName, Id)
#define BindTypeExternal(Library, ClassType, Id, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, Id, BindingFn)

#define BindBase(Library, BoundType, BaseType) BindBaseType<BaseType>(Library, BoundType)
