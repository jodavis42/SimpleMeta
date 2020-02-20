#pragma once

#include "BoundType.hpp"
#include "MetaLibrary.hpp"
#include "MetaSerialization.hpp"
#include "StaticTypeId.hpp"
#include "Attributes.hpp"

template <typename ClassType>
BoundType* CreateBoundType(ReflectionLibrary& library, const std::string& className, int id)
{
  BoundType* boundType = StaticTypeId<ClassType>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(ClassType);
  boundType->mId = id;

  library.AddBoundType(boundType);
  return boundType;
}

template <typename FieldPointer, FieldPointer field, typename Class, typename FieldType>
static Field* FromField(ReflectionLibrary& library, BoundType& owner, const std::string& name, FieldType Class::*dummy, size_t offset)
{
  Field* f = new Field();
  f->mName = name;
  f->mOffset = offset;
  f->mType = StaticTypeId<FieldType>::GetBoundType();
  if(f->mType->QueryComponentType<MetaSerialization>() == nullptr)
  {
    f->mType->AddComponentType<TypedMetaSerialization<FieldType>>();
  }

  owner.mFields.push_back(f);
  return f;
}

template <typename ClassType, void (*BindingFn)(ReflectionLibrary& library, BoundType&)>
BoundType* BindClassType(ReflectionLibrary& library, const std::string& className, int id)
{
  BoundType* boundType = CreateBoundType<ClassType>(library, className, id);
  boundType->AddComponentType<TypedMetaSerialization<ClassType>>();
  BindingFn(library, *boundType);
  return boundType;
}

template <typename GetterType, GetterType getter, typename SetterType, SetterType setter, typename ClassType, typename GetType, typename SetType>
static GetterSetter* FromGetterSetter(ReflectionLibrary& library, BoundType& owner, const std::string& name, GetType(ClassType::*dummyGetter)()const, void (ClassType::*dummySetter)(SetType))
{
  auto getSet = new GetterSetterTyped<ClassType, GetterType, GetType, SetterType, SetType>();
  getSet->mType = StaticTypeId<GetType>::GetBoundType();
  getSet->mName = name;
  getSet->mGetter = getter;
  getSet->mSetter = setter;
  getSet->mType->AddComponentType<TypedMetaSerialization<GetType>>();
  owner.mGetterSetters.push_back(getSet);
  return getSet;
}

template <typename Type>
BoundType* BindPrimitiveTypeToLibrary(ReflectionLibrary& library, const std::string& className)
{
  BoundType* boundType = StaticTypeId<Type>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeof(Type);
  boundType->AddComponentType<TypedMetaSerialization<Type>>();
  library.AddBoundType(boundType);
  return boundType;
}

template <typename BaseType>
void BindBaseType(ReflectionLibrary& library, BoundType& derrivedType)
{
  BoundType* baseType = StaticTypeId<BaseType>::GetBoundType();
  derrivedType.mBaseType = baseType;
  derrivedType.mFields = baseType->mFields;
  derrivedType.mGetterSetters = baseType->mGetterSetters;
}

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember))
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPropertyAs(Library, boundType, Owner, FieldMember, FieldName) BindFieldAs(Library, boundType, Owner, FieldMember, FieldName)->AddComponentTypeChainable<SerializedAttribute>()
#define BindProperty(Library, boundType, Owner, FieldMember) BindPropertyAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindGetterSetterAs(Library, boundType, Owner, FieldName, Getter, Setter) \
  FromGetterSetter<decltype(&Owner::Getter), &Owner::Getter, decltype(&Owner::Setter), &Owner::Setter, Owner>(library, boundType, FieldName, &Owner::Getter, &Owner::Setter)->AddComponentTypeChainable<SerializedAttribute>()
#define BindGetterSetter(Library, boundType, Owner, FieldName) BindGetterSetterAs(Library, boundType, Owner, #FieldName, Get##FieldName, Set##FieldName)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName) BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName)
#define BindPrimitiveType(Library, PrimitiveType) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType)

#define BindTypeAs(Library, ClassType, ClassTypeName, Id) BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName, Id)
#define BindType(Library, ClassType, Id) BindTypeAs(Library, ClassType, #ClassType, Id)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, Id, BindingFn) BindClassType<ClassType, BindingFn>(Library, ClassTypeName, Id)
#define BindTypeExternal(Library, ClassType, Id, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, Id, BindingFn)

#define BindBase(Library, BoundType, BaseType) BindBaseType<BaseType>(Library, BoundType)
