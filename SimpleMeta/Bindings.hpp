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

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember));
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName) BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName)
#define BindPrimitiveType(Library, PrimitiveType) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType)

#define BindTypeAs(Library, ClassType, ClassTypeName) BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName)
#define BindType(Library, ClassType) BindTypeAs(Library, ClassType, #ClassType)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, BindingFn) BindClassType<ClassType, BindingFn>(Library, ClassTypeName)
#define BindTypeExternal(Library, ClassType, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, BindingFn)
