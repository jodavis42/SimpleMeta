template <typename FunctionType, FunctionType FunctionToBind>
void BoundStatic(Call& call)
{
  (*FunctionToBind)();
}

template <typename FunctionType, FunctionType FunctionToBind>
Function* FromMethod(void(*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, FunctionToBind>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType>
void BoundStaticReturn(Call& call)
{
  ReturnType ret = (*FunctionToBind)();
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType>
Function* FromMethod(ReturnType(*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, FunctionToBind, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
void BoundInstance(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)();
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromMethod(void(ClassType::*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromMethod(void(ClassType::*)()const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
void BoundInstanceReturn(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*FunctionToBind)();
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
Function* FromMethod(ReturnType(ClassType::*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
Function* FromMethod(ReturnType(ClassType::*)()const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0>
void BoundStatic(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  (*FunctionToBind)(arg0);
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0>
Function* FromMethod(void(*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, FunctionToBind, Arg0>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0>
void BoundStaticReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  ReturnType ret = (*FunctionToBind)(arg0);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0>
Function* FromMethod(ReturnType(*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, FunctionToBind, ReturnType, Arg0>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0>
void BoundInstance(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0 arg0 = call.Get<Arg0>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)(arg0);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0>
Function* FromMethod(void(ClassType::*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0>
Function* FromMethod(void(ClassType::*)(Arg0)const)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0>
void BoundInstanceReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*FunctionToBind)(arg0);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0>
Function* FromMethod(ReturnType(ClassType::*)(Arg0))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0>
Function* FromMethod(ReturnType(ClassType::*)(Arg0)const)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0, typename Arg1>
void BoundStatic(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  (*FunctionToBind)(arg0, arg1);
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0, typename Arg1>
Function* FromMethod(void(*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, FunctionToBind, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0, typename Arg1>
void BoundStaticReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  ReturnType ret = (*FunctionToBind)(arg0, arg1);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0, typename Arg1>
Function* FromMethod(ReturnType(*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, FunctionToBind, ReturnType, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0, typename Arg1>
void BoundInstance(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)(arg0, arg1);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0, typename Arg1>
Function* FromMethod(void(ClassType::*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0, typename Arg1>
Function* FromMethod(void(ClassType::*)(Arg0, Arg1)const)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0, typename Arg1>
void BoundInstanceReturn(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*FunctionToBind)(arg0, arg1);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0, typename Arg1>
Function* FromMethod(ReturnType(ClassType::*)(Arg0, Arg1))
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0, typename Arg1>
Function* FromMethod(ReturnType(ClassType::*)(Arg0, Arg1)const)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0, Arg1>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

