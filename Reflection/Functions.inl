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
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)();
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromMethod(void(ClassType::*)())
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromMethod(void(ClassType::*)()const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
void BoundInstanceReturn(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
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
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
Function* FromMethod(ReturnType(ClassType::*)()const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
void BoundExtensionInstance(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (*FunctionToBind)(self);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromExtensionMethod(void(*)( ClassType*))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType>
Function* FromExtensionMethod(void(*)(const ClassType*))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
void BoundExtensionInstanceReturn(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (*FunctionToBind)(self);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
Function* FromExtensionMethod(ReturnType(*)( ClassType*))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType>
Function* FromExtensionMethod(ReturnType(*)(const ClassType*))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0Type>
void BoundStatic(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  (*FunctionToBind)(arg0);
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0Type>
Function* FromMethod(void(*)(Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, FunctionToBind, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0Type>
void BoundStaticReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  ReturnType ret = (*FunctionToBind)(arg0);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0Type>
Function* FromMethod(ReturnType(*)(Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, FunctionToBind, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
void BoundInstance(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)(arg0);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
Function* FromMethod(void(ClassType::*)(Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
Function* FromMethod(void(ClassType::*)(Arg0Type)const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
void BoundInstanceReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*FunctionToBind)(arg0);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
Function* FromMethod(ReturnType(ClassType::*)(Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
Function* FromMethod(ReturnType(ClassType::*)(Arg0Type)const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
void BoundExtensionInstance(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (*FunctionToBind)(self, arg0);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
Function* FromExtensionMethod(void(*)( ClassType*, Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type>
Function* FromExtensionMethod(void(*)(const ClassType*, Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
void BoundExtensionInstanceReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (*FunctionToBind)(self, arg0);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
Function* FromExtensionMethod(ReturnType(*)( ClassType*, Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type>
Function* FromExtensionMethod(ReturnType(*)(const ClassType*, Arg0Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0Type, typename Arg1Type>
void BoundStatic(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  (*FunctionToBind)(arg0, arg1);
}

template <typename FunctionType, FunctionType FunctionToBind, typename Arg0Type, typename Arg1Type>
Function* FromMethod(void(*)(Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStatic<FunctionType, FunctionToBind, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0Type, typename Arg1Type>
void BoundStaticReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  ReturnType ret = (*FunctionToBind)(arg0, arg1);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ReturnType, typename Arg0Type, typename Arg1Type>
Function* FromMethod(ReturnType(*)(Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundStaticReturn<FunctionType, FunctionToBind, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
void BoundInstance(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (self->*FunctionToBind)(arg0, arg1);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
Function* FromMethod(void(ClassType::*)(Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
Function* FromMethod(void(ClassType::*)(Arg0Type, Arg1Type)const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstance<FunctionType, FunctionToBind, ClassType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
void BoundInstanceReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (self->*FunctionToBind)(arg0, arg1);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
Function* FromMethod(ReturnType(ClassType::*)(Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
Function* FromMethod(ReturnType(ClassType::*)(Arg0Type, Arg1Type)const)
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
void BoundExtensionInstance(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  (*FunctionToBind)(self, arg0, arg1);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
Function* FromExtensionMethod(void(*)( ClassType*, Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename Arg0Type, typename Arg1Type>
Function* FromExtensionMethod(void(*)(const ClassType*, Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstance<FunctionType, FunctionToBind, ClassType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<void>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
void BoundExtensionInstanceReturn(Call& call)
{
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  ReturnType ret = (*FunctionToBind)(self, arg0, arg1);
  call.Set<ReturnType>(Call::Return, ret);
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
Function* FromExtensionMethod(ReturnType(*)( ClassType*, Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

template <typename FunctionType, FunctionType FunctionToBind, typename ClassType, typename ReturnType, typename Arg0Type, typename Arg1Type>
Function* FromExtensionMethod(ReturnType(*)(const ClassType*, Arg0Type, Arg1Type))
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundExtensionInstanceReturn<FunctionType, FunctionToBind, ClassType, ReturnType, Arg0Type, Arg1Type>;
  fn->SetReturnType(StaticTypeId<ReturnType>::GetBoundType());
  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
}

