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
static Field* FromField(ReflectionLibrary& library, BoundType& owner, const std::string& name, FieldType ClassType::* dummy, size_t offset)
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
  // Have to early out here to prevent memory leaks
  if(boundType->mIsSetup)
  {
    ReflectionErrorIf(boundType->mIsSetup, "ClassType '%s' was bound twice. You can only bind a type once", className.c_str());
    return boundType;
  }
  
  BindingFn(library, *boundType);
  DefaultTypeSetup<ClassType>(library, *boundType);
  boundType->mIsSetup = true;
  return boundType;
}

template <typename GetterType, GetterType getter, typename SetterType, SetterType setter, typename ClassType, typename GetType, typename SetType>
static GetterSetter* FromGetterSetter(ReflectionLibrary& library, BoundType& owner, const std::string& name, GetType(ClassType::* dummyGetter)()const, void (ClassType::* dummySetter)(SetType))
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

template <typename ClassType, typename ... Args>
static Function* FromClassConstructor(ReflectionLibrary& library, BoundType& owner)
{
  Function* function = FromConstructor<ClassType, Args...>();
  owner.mConstructors.push_back(function);
  return function;
}

template <typename ClassType>
static Function* FromClassDefaultConstructor(ReflectionLibrary& library, BoundType& owner)
{
  if(owner.mDefaultConstructor != nullptr)
    delete owner.mDefaultConstructor;
  Function* function = FromConstructor<ClassType>();
  owner.mDefaultConstructor = function;
  return function;
}

template <typename ClassType>
static Function* FromClassCopyConstructor(ReflectionLibrary& library, BoundType& owner)
{
  if(owner.mCopyConstructor != nullptr)
    delete owner.mCopyConstructor;
  Function* function = FromCopyConstructor<ClassType>();
  owner.mCopyConstructor = function;
  return function;
}

template <typename ClassType>
static Function* FromClassDestructor(ReflectionLibrary& library, BoundType& owner)
{
  if(owner.mDestructor != nullptr)
    delete owner.mDestructor;
  Function* function = FromDestructor<ClassType>();
  owner.mDestructor = function;
  return function;
}

template <typename PrimitiveType>
BoundType* BindPrimitiveTypeToLibrary(ReflectionLibrary& library, const std::string& className, size_t id)
{
  BoundType* boundType = CreateBoundType<PrimitiveType>(library, className, id, sizeof(PrimitiveType));
  ReflectionErrorIf(boundType->mIsSetup, "Primitive '%s' was bound twice. You can only bind a type once", className.c_str());
  boundType->mIsSetup = true;
  return boundType;
}

template <typename BaseType>
void BindBaseType(ReflectionLibrary& library, BoundType& derrivedType)
{
  BoundType* baseType = StaticTypeId<BaseType>::GetBoundType();
  derrivedType.mBaseType = baseType;
}

/// Macros to aid in overload resolution for functions
#define ReflectionNoOverload
#define ReflectionStaticOverload(ReturnType, ...) (ReturnType (*)(__VA_ARGS__))
#define ReflectionInstanceOverload(ClassType, ReturnType, ...) (ReturnType (ClassType::*)(__VA_ARGS__))
#define ReflectionConstInstanceOverload(ClassType, ReturnType, ...) (ReturnType (ClassType::*)(__VA_ARGS__) const)

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  SimpleReflection::FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember))
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPropertyAs(Library, boundType, Owner, FieldMember, FieldName) BindFieldAs(Library, boundType, Owner, FieldMember, FieldName)
#define BindProperty(Library, boundType, Owner, FieldMember) BindPropertyAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define FullBindGetterSetterAs(Library, boundType, Owner, GetPointer, GetOverloadResolution, SetPointer, SetOverloadResolution, FieldName) \
  SimpleReflection::FromGetterSetter<decltype(GetOverloadResolution GetPointer), GetPointer, decltype(SetOverloadResolution SetPointer), SetPointer, Owner>(library, boundType, FieldName, GetOverloadResolution(GetPointer), SetOverloadResolution(SetPointer))
#define BindGetterSetterAs(Library, boundType, Owner, Getter, Setter, FieldName) \
  FullBindGetterSetterAs(Library, boundType, Owner, &Owner::Getter, ReflectionNoOverload, &Owner::Setter, ReflectionNoOverload, FieldName)
#define BindGetterSetter(Library, boundType, Owner, FieldName) BindGetterSetterAs(Library, boundType, Owner, Get##FieldName, Set##FieldName, #FieldName)

#define FullBindGetterAs(Library, boundType, Owner, FnPointer, FnOverloadResolution, FieldName)\
  SimpleReflection::FromGetter<decltype(FnOverloadResolution FnPointer), FnPointer, decltype(nullptr), nullptr, Owner>(library, boundType, FieldName, FnOverloadResolution(FnPointer), nullptr)
#define BindGetterAs(Library, boundType, Owner, Getter, FieldName) FullBindGetterAs(Library, boundType, Owner, &Owner::Getter, ReflectionNoOverload, FieldName)
#define BindGetter(Library, boundType, Owner, FieldName) BindGetterAs(Library, boundType, Owner, Get##FieldName, #FieldName)

#define FullBindSetterAs(Library, boundType, Owner, FnPointer, FnOverloadResolution, FieldName)\
    SimpleReflection::FromSetter<decltype(nullptr), nullptr, decltype(FnOverloadResolution FnPointer), FnPointer, Owner>(library, boundType, FieldName, nullptr, FnOverloadResolution(FnPointer))
#define BindSetterAs(Library, boundType, Owner, Setter, FieldName) FullBindSetterAs(Library, boundType, Owner, &Owner::Setter, ReflectionNoOverload, FieldName)
#define BindSetter(Library, boundType, Owner, FieldName) BindSetterAs(Library, boundType, Owner, Set##FieldName, #FieldName)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName, Id) SimpleReflection::BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName, Id)
#define BindPrimitiveType(Library, PrimitiveType, Id) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType, Id)

#define BindTypeAs(Library, ClassType, ClassTypeName, Id) SimpleReflection::BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName, Id)
#define BindType(Library, ClassType, Id) BindTypeAs(Library, ClassType, #ClassType, Id)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, Id, BindingFn) SimpleReflection::BindClassType<ClassType, BindingFn>(Library, ClassTypeName, Id)
#define BindTypeExternal(Library, ClassType, Id, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, Id, BindingFn)

#define FullBindFunctionAs(Library, BoundType, FnPointer, FnOverloadResolution, FunctionName)\
  SimpleReflection::FromFunction<decltype(FnOverloadResolution FnPointer), FnPointer>(Library, BoundType, FunctionName, FnOverloadResolution(FnPointer))
#define BindFunctionAs(Library, BoundType, ClassType, Function, FunctionName)  FullBindFunctionAs(Library, BoundType, &ClassType::Function, ReflectionNoOverload, FunctionName)
#define BindFunction(Library, BoundType, ClassType, Function) BindFunctionAs(Library, BoundType, ClassType, Function, #Function)

#define BindDefaultConstructor(Library, BoundType, ClassType) SimpleReflection::FromClassDefaultConstructor<ClassType>(Library, BoundType)
#define BindCopyConstructor(Library, BoundType, ClassType) SimpleReflection::FromClassCopyConstructor<ClassType>(Library, BoundType)
#define BindConstructor(Library, BoundType, ClassType, ...) SimpleReflection::FromClassConstructor<ClassType, ##__VA_ARGS__>(Library, BoundType)
#define BindDestructor(Library, BoundType, ClassType) SimpleReflection::FromClassDestructor<ClassType>(Library, BoundType)

#define BindBase(Library, BoundType, BaseType) SimpleReflection::BindBaseType<BaseType>(Library, BoundType)

#define DeclareVirtualBoundType(ClassType)                          \
virtual BoundType* VirtualGetBoundType() const                      \
{                                                                   \
  return SimpleReflection::StaticTypeId<ClassType>::GetBoundType(); \
}

}//namespace SimpleReflection
