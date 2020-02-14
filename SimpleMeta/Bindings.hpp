#pragma once

#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "MetaSerialization.hpp"
#include "StaticTypeId.hpp"

template <typename FieldPointer, FieldPointer field, typename Class, typename FieldType>
static Field* FromField(ReflectionLibrary& library, BoundType& owner, const std::string& name, FieldType Class::*dummy, size_t offset)
{
  Field f = Field();
  f.mName = name;
  f.mOffset = offset;
  f.mType = StaticTypeId<FieldType>::GetBoundType();
  if(f.mType->mMetaSerialization == nullptr)
  {
    f.mType->mMetaSerialization = new TypedMetaSerialization<FieldType>();
  }

  owner.mFields.push_back(f);
  return &owner.mFields.back();
}

template <typename ClassType, void (*BindingFn)(ReflectionLibrary& library, BoundType&)>
void BindClassType(ReflectionLibrary& library, const std::string& className, int id)
{
  BoundType* boundType = StaticTypeId<ClassType>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(ClassType);
  boundType->mId = id;

  boundType->mMetaSerialization = new TypedMetaSerialization<ClassType>();
  BindingFn(library, *boundType);
  library.AddBoundType(boundType);
}

template <typename Type>
void BindPrimitiveTypeToLibrary(ReflectionLibrary& library, const std::string& className)
{
  BoundType* boundType = StaticTypeId<Type>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(Type);
  boundType->mMetaSerialization = new TypedMetaSerialization<Type>();
  library.AddBoundType(boundType);
}

template <typename BaseType>
void BindBaseType(ReflectionLibrary& library, BoundType& derrivedType)
{
  BoundType* baseType = StaticTypeId<BaseType>::GetBoundType();
  derrivedType.mBaseType = baseType;
  derrivedType.mFields = baseType->mFields;
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
