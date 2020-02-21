#pragma once

#include "StaticTypeId.hpp"

#include "Function.hpp"
#include "Call.hpp"

struct BoundType;

// Ideally these would be replaced with a bunch of variadics, but the unpack is very complicated for that.
// Additionally, there's a lot of issues where const functions need to be implemented (since they're separate)
// and there may be some issues with qualified types as well that were missed.

template <typename FunctionType, FunctionType Function>
void BoundStatic(Call& call)
{
  (*Function)();
}
template <typename FunctionType, FunctionType Function, typename Arg0>
void BoundStatic(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
 (*Function)(arg0);
}
template <typename FunctionType, FunctionType Function, typename Arg0, typename Arg1>
void BoundStatic(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  (*Function)(arg0, arg1);
}
template <typename FunctionType, FunctionType Function, typename ReturnType>
void BoundStaticReturn(Call& call)
{
  ReturnType ret = (*Function)();
  call.Set(Call::Return, ret);
}
template <typename FunctionType, FunctionType Function, typename ReturnType, typename Arg0>
void BoundStaticReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  ReturnType ret = (*Function)(arg0);
  call.Set(Call::Return, ret);
}
template <typename FunctionType, FunctionType Function, typename ReturnType, typename Arg0, typename Arg1>
void BoundStaticReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  ReturnType ret = (*Function)(arg0, arg1);
  call.Set(Call::Return, ret);
}
template <typename FunctionType, FunctionType Function, typename ClassType>
void BoundInstance(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*Function)();
}
template <typename FunctionType, FunctionType Function, typename ClassType, typename Arg0>
void BoundInstance(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*Function)(arg0);
}
template <typename FunctionType, FunctionType Function, typename ClassType, typename Arg0, typename Arg1>
void BoundInstance(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*Function)(arg0, arg1);
}
template <typename FunctionType, FunctionType Function, typename ClassType, typename ReturnType>
void BoundInstanceReturn(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*Function)();
  call.Set(Call::Return, ret);
}
template <typename FunctionType, FunctionType Function, typename ClassType, typename ReturnType, typename Arg0>
void BoundInstanceReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*Function)(arg0);
  call.Set(Call::Return, ret);
}
template <typename FunctionType, FunctionType Function, typename ClassType, typename ReturnType, typename Arg0, typename Arg1>
void BoundInstanceReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*Function)(arg0, arg1);
  call.Set(Call::Return, ret);
}

template <typename ClassType>
void BoundConstructor(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  char* self = *(char**)location;
  new (self) ClassType();
}
template <typename ClassType, typename Arg0>
void BoundConstructor(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  char* self = *(char**)location;
  new (self) ClassType(arg0);
}
template <typename ClassType>
void BoundCopyConstructor(Call& call)
{
  ClassType& arg0 = call.Get<ClassType>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  char* self = *(char**)location;
  new (self) ClassType(arg0);
}

template <typename ClassType>
void BoundDestructor(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  self->~ClassType();
}

template <typename FunctionType, FunctionType Fn>
Function* FromMethod(void(*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, Fn>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename Arg0>
Function* FromMethod(void(*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, Fn, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename Arg0, typename Arg1>
Function* FromMethod(void(*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, Fn, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ReturnType>
Function* FromMethod(ReturnType(*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, Fn, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ReturnType, typename Arg0>
Function* FromMethod(ReturnType(*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, Fn, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ReturnType, typename Arg0, typename Arg1>
Function* FromMethod(ReturnType(*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, Fn, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}


template <typename FunctionType, FunctionType Fn, typename ClassType>
Function* FromMethod(void(ClassType::*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, Fn, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename Arg0>
Function* FromMethod(void(ClassType::*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, Fn, ClassType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename Arg0, typename Arg1>
Function* FromMethod(void(ClassType::*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type ArgType;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, Fn, ClassType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename ReturnType>
Function* FromMethod(ReturnType(ClassType::*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, Fn, ClassType, ReturnType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename ReturnType>
Function* FromMethod(ReturnType(ClassType::*)()const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, Fn, ClassType, ReturnType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename ReturnType, typename Arg0>
Function* FromMethod(ReturnType(ClassType::*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, Fn, ClassType, ReturnType, Arg0Type>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}
template <typename FunctionType, FunctionType Fn, typename ClassType, typename ReturnType, typename Arg0, typename Arg1>
Function* FromMethod(ReturnType(ClassType::*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, Fn, ClassType, ReturnType, Arg0Type, Arg1Type>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename ClassType>
Function* FromConstructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundConstructor<ClassType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}
template <typename ClassType>
Function* FromCopyConstructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundCopyConstructor<ClassType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename ClassType>
Function* FromDestructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundDestructor<ClassType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

#define CreateMethod(Fn) FromMethod<decltype(Fn), Fn>(Fn)
