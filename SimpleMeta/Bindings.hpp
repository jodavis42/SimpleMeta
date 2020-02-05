#pragma once

#include "BoundType.hpp"
#include "MetaLibrary.hpp"
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
  f.mType->mStride = sizeof(FieldType);

  owner.mFields.push_back(f);
}

template <typename ClassType, void (*BindingFn)(MetaLibrary& library, BoundType&)>
void BindClassType(MetaLibrary& library, const std::string& className)
{
  BoundType* boundType = StaticTypeId<ClassType>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(ClassType);
  BindingFn(library, *boundType);
  library.AddBoundType(boundType);
}

template <typename Type>
void BindPrimitiveTypeToLibrary(MetaLibrary& library, const std::string& className)
{
  BoundType* boundType = StaticTypeId<Type>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(Type);
  library.AddBoundType(boundType);
}

#define BindField(Library, boundType, Owner, FieldName) \
  FromField<decltype(&Owner::FieldName), &Owner::FieldName>(library, boundType, #FieldName, &Owner::FieldName, offsetof(Owner, FieldName));
#define BindPrimitiveType(Library, PrimitiveType) BindPrimitiveTypeToLibrary<PrimitiveType>(Library, #PrimitiveType)
#define BindType(Library, ClassType) BindClassType<ClassType, &ClassType::Bind>(Library, #ClassType)
#define BindTypeExternal(Library, ClassType, BindingFn) BindClassType<ClassType, BindingFn>(Library, #ClassType)
