#pragma once

#include "BoundType.hpp"
#include "Field.hpp"
#include "GetterSetter.hpp"
#include "MetaLibrary.hpp"
#include "MetaSerialization.hpp"
#include "StaticTypeId.hpp"
#include "Attributes.hpp"
#include "FunctionBinding.hpp"

template <typename ClassType>
void DefaultTypeSetup(ReflectionLibrary& library, BoundType& ownerType);

template <typename ClassType, typename FieldType>
void DefaultFieldSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& fieldType);

template <typename ClassType, typename GetterSetterType>
void DefaultGetterSetterSetup(ReflectionLibrary& library, BoundType& ownerType, BoundType& getterSetterType);

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
  DefaultFieldSetup<ClassType, FieldType>(library, owner, *f->mType);
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
  DefaultGetterSetterSetup<ClassType, GetType>(library, owner, *getSet->mType);
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

  derrivedType.mFields.reserve(baseType->mFields.size());
  for(size_t i = 0; i < baseType->mFields.size(); ++i)
  {
    Field* field = new Field();
    *field = *baseType->mFields[i];
    derrivedType.mFields.push_back(field);
  }
  
  derrivedType.mGetterSetters.reserve(baseType->mGetterSetters.size());
  for(size_t i = 0; i < baseType->mGetterSetters.size(); ++i)
  {
    GetterSetter* baseGetSet = baseType->mGetterSetters[i];
    GetterSetter* getSet = new GetterSetter();
    *getSet = *baseGetSet;
    getSet->mGetter = new Function();
    *getSet->mGetter = *baseGetSet->mGetter;
    getSet->mSetter = new Function();
    *getSet->mSetter = *baseGetSet->mSetter;
    derrivedType.mGetterSetters.push_back(getSet);
  }

  for(auto&& pair : baseType->mFunctionMap)
  {
    auto&& derivedFunctionList = derrivedType.mFunctionMap[pair.first];
    derivedFunctionList.reserve(pair.second.size());
    for(size_t i = 0; i < pair.second.size(); ++i)
    {
      Function* baseFunction = pair.second[i];
      Function* derivedFunction = new Function();
      *derivedFunction = *baseFunction;
      derivedFunctionList.push_back(derivedFunction);
    }
  }
}

#define BindFieldAs(Library, boundType, Owner, FieldMember, FieldName) \
  FromField<decltype(&Owner::FieldMember), &Owner::FieldMember>(library, boundType, FieldName, &Owner::FieldMember, offsetof(Owner, FieldMember))
#define BindField(Library, boundType, Owner, FieldMember) BindFieldAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindPropertyAs(Library, boundType, Owner, FieldMember, FieldName) BindFieldAs(Library, boundType, Owner, FieldMember, FieldName)->AddComponentTypeChainable<SerializedAttribute>()
#define BindProperty(Library, boundType, Owner, FieldMember) BindPropertyAs(Library, boundType, Owner, FieldMember, #FieldMember)

#define BindGetterSetterAs(Library, boundType, Owner, FieldName, Getter, Setter) \
  FromGetterSetter<decltype(&Owner::Getter), &Owner::Getter, decltype(&Owner::Setter), &Owner::Setter, Owner>(library, boundType, FieldName, &Owner::Getter, &Owner::Setter)->AddComponentTypeChainable<SerializedAttribute>()
#define BindGetterSetter(Library, boundType, Owner, FieldName) BindGetterSetterAs(Library, boundType, Owner, #FieldName, Get##FieldName, Set##FieldName)

#define BindPrimitiveTypeAs(Library, PrimitiveType, PrimitiveTypeName, Id) BindPrimitiveTypeToLibrary<PrimitiveType>(Library, PrimitiveTypeName, Id)
#define BindPrimitiveType(Library, PrimitiveType, Id) BindPrimitiveTypeAs(Library, PrimitiveType, #PrimitiveType, Id)

#define BindTypeAs(Library, ClassType, ClassTypeName, Id) BindClassType<ClassType, &ClassType::Bind>(Library, ClassTypeName, Id)
#define BindType(Library, ClassType, Id) BindTypeAs(Library, ClassType, #ClassType, Id)

#define BindTypeExternalAs(Library, ClassType, ClassTypeName, Id, BindingFn) BindClassType<ClassType, BindingFn>(Library, ClassTypeName, Id)
#define BindTypeExternal(Library, ClassType, Id, BindingFn) BindTypeExternalAs(Library, ClassType, #ClassType, Id, BindingFn)

#define BindFunctionAs(Library, BoundType, ClassType, FunctionName, Function) FromFunction<decltype(&ClassType::Function), &ClassType::Function>(Library, BoundType, FunctionName, &ClassType::Function)
#define BindFunction(Library, BoundType, ClassType, Function) BindFunctionAs(Library, BoundType, ClassType, #Function, Function)

#define BindBase(Library, BoundType, BaseType) BindBaseType<BaseType>(Library, BoundType)
