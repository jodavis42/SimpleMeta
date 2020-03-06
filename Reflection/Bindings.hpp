#pragma once

#include "BoundType.hpp"
#include "Field.hpp"
#include "GetterSetter.hpp"
#include "ReflectionLibrary.hpp"
#include "StaticTypeId.hpp"
#include "FunctionBinding.hpp"

namespace SimpleReflection
{

template <typename ClassType>
void DefaultTypeSetup(ReflectionLibrary& library, BoundType& ownerType);

template <typename ClassType, typename FieldType>
void DefaultFieldSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& fieldType, Field& field);

template <typename ClassType, typename GetterSetterType>
void DefaultGetterSetterSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& getterSetterType, GetterSetter& getterSetter);

template <typename ClassType>
BoundType* CreateBoundType(ReflectionLibrary& library, const std::string& className, size_t id, size_t sizeInBytes)
{
  BoundType* boundType = StaticTypeId<ClassType>::GetBoundType();
  boundType->mName = className;
  boundType->mSizeInBytes = sizeInBytes;
  boundType->mId = id;
  library.AddBoundType(boundType);
  return boundType;
}

template <typename ClassType>
BoundType* CreateBoundType(ReflectionLibrary& library, const std::string& className, size_t id)
{
  return CreateBoundType<ClassType>(library, className, id, sizeof(ClassType));
}

template <typename FieldPointer, FieldPointer field, typename ClassType, typename FieldType>
static Field* FromField(ReflectionLibrary& library, BoundType& owner, const std::string& name, FieldType ClassType::*dummy, size_t offset)
{
  Field* f = new Field();
  f->mName = name;
  f->mOffset = offset;
  f->mType = StaticTypeId<FieldType>::GetBoundType();
  DefaultFieldSetup<ClassType, FieldType>(library, owner, *f->mType, *f);
  owner.mFields.push_back(f);
  return f;
}

template <typename ClassType, void (*BindingFn)(ReflectionLibrary& library, BoundType&)>
BoundType* BindClassType(ReflectionLibrary& library, const std::string& className, int id)
{
  BoundType* boundType = CreateBoundType<ClassType>(library, className, id);
  BindingFn(library, *boundType);
  boundType->mDefaultConstructor = FromConstructor<ClassType>();
  boundType->mCopyConstructor = FromCopyConstructor<ClassType>();
  boundType->mDestructor = FromDestructor<ClassType>();
  DefaultTypeSetup<ClassType>(library, *boundType);
  return boundType;
}

template <typename GetterType, GetterType getter, typename SetterType, SetterType setter, typename ClassType, typename GetType, typename SetType>
static GetterSetter* FromGetterSetter(ReflectionLibrary& library, BoundType& owner, const std::string& name, GetType(ClassType::*dummyGetter)()const, void (ClassType::*dummySetter)(SetType))
{
  auto getSet = new GetterSetter();
  getSet->mType = StaticTypeId<GetType>::GetBoundType();
  getSet->mName = name;
  getSet->mGetter = FromMethod<GetterType, getter>(getter);
  getSet->mSetter = FromMethod<SetterType, setter>(setter);
  DefaultGetterSetterSetup<ClassType, GetType>(library, owner, *getSet->mType, *getSet);
  owner.mGetterSetters.push_back(getSet);
  return getSet;
}

template <typename GetterType, GetterType getter, typename SetterType, SetterType setter, typename ClassType, typename GetType>
static GetterSetter* FromGetter(ReflectionLibrary& library, BoundType& owner, const std::string& name, GetType(ClassType::* dummyGetter)()const, std::nullptr_t)
{
  auto getSet = new GetterSetter();
  getSet->mType = StaticTypeId<GetType>::GetBoundType();
  getSet->mName = name;
  getSet->mGetter = FromMethod<GetterType, getter>(getter);
  DefaultGetterSetterSetup<ClassType, GetType>(library, owner, *getSet->mType, *getSet);
  owner.mGetterSetters.push_back(getSet);
  return getSet;
}

template <typename GetterType, GetterType getter, typename SetterType, SetterType setter, typename ClassType, typename SetType>
static GetterSetter* FromSetter(ReflectionLibrary& library, BoundType& owner, const std::string& name, std::nullptr_t, void (ClassType::* dummySetter)(SetType))
{
  using FieldType = UnqualifiedType<SetType>::type;
  auto getSet = new GetterSetter();
  getSet->mType = StaticTypeId<FieldType>::GetBoundType();
  getSet->mName = name;
  getSet->mSetter = FromMethod<SetterType, setter>(setter);
  DefaultGetterSetterSetup<ClassType, FieldType>(library, owner, *getSet->mType, *getSet);
  owner.mGetterSetters.push_back(getSet);
  return getSet;
}

template <typename FunctionType, FunctionType FunctionToBind>
static Function* FromFunction(ReflectionLibrary& library, BoundType& owner, const std::string& name, FunctionType)
{
  Function* function = FromMethod<FunctionType, FunctionToBind>(FunctionToBind);
  owner.mFunctionMap[name].push_back(function);
  return function;
}

template <typename PrimitiveType>
BoundType* BindPrimitiveTypeToLibrary(ReflectionLibrary& library, const std::string& className, size_t id)
{
  BoundType* boundType = CreateBoundType<PrimitiveType>(library, className, id, sizeof(PrimitiveType));
  return boundType;
}

template <typename BaseType>
void BindBaseType(ReflectionLibrary& library, BoundType& derrivedType)
{
  BoundType* baseType = StaticTypeId<BaseType>::GetBoundType();
  derrivedType.mBaseType = baseType;
}

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  SimpleReflection::FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember))
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPropertyAs(Library, boundType, Owner, FieldMember, FieldName) BindFieldAs(Library, boundType, Owner, FieldMember, FieldName)
#define BindProperty(Library, boundType, Owner, FieldMember) BindPropertyAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindGetterSetterAs(Library, boundType, Owner, FieldName, Getter, Setter) \
  SimpleReflection::FromGetterSetter<decltype(&Owner::Getter), &Owner::Getter, decltype(&Owner::Setter), &Owner::Setter, Owner>(library, boundType, FieldName, &Owner::Getter, &Owner::Setter)
#define BindGetterSetter(Library, boundType, Owner, FieldName) BindGetterSetterAs(Library, boundType, Owner, #FieldName, Get##FieldName, Set##FieldName)

#define BindGetterAs(Library, boundType, Owner, FieldName, Getter) \
  SimpleReflection::FromGetter<decltype(&Owner::Getter), &Owner::Getter, decltype(nullptr), nullptr, Owner>(library, boundType, FieldName, &Owner::Getter, nullptr)
#define BindGetter(Library, boundType, Owner, FieldName) BindGetterAs(Library, boundType, Owner, #FieldName, Get##FieldName)

#define BindSetterAs(Library, boundType, Owner, FieldName, Setter) \
  SimpleReflection::FromSetter<decltype(nullptr), nullptr, decltype(&Owner::Setter), &Owner::Setter, Owner>(library, boundType, FieldName, nullptr, &Owner::Setter)
#define BindSetter(Library, boundType, Owner, FieldName) BindSetterAs(Library, boundType, Owner, #FieldName, Set##FieldName)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName, Id) SimpleReflection::BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName, Id)
#define BindPrimitiveType(Library, PrimitiveType, Id) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType, Id)

#define BindTypeAs(Library, ClassType, ClassTypeName, Id) SimpleReflection::BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName, Id)
#define BindType(Library, ClassType, Id) BindTypeAs(Library, ClassType, #ClassType, Id)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, Id, BindingFn) SimpleReflection::BindClassType<ClassType, BindingFn>(Library, ClassTypeName, Id)
#define BindTypeExternal(Library, ClassType, Id, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, Id, BindingFn)

#define BindFunctionAs(Library, BoundType, ClassType, FunctionName, Function) SimpleReflection::FromFunction<decltype(&ClassType::Function), &ClassType::Function>(Library, BoundType, FunctionName, &ClassType::Function)
#define BindFunction(Library, BoundType, ClassType, Function) BindFunctionAs(Library, BoundType, ClassType, #Function, Function)

#define BindBase(Library, BoundType, BaseType) SimpleReflection::BindBaseType<BaseType>(Library, BoundType)

}//namespace SimpleReflection
