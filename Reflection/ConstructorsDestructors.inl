template <typename ClassType>
void BoundConstructor(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  new(self) ClassType();
}

template <typename ClassType> 
Function* FromConstructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundConstructor<ClassType>; 
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename ClassType, typename Arg0>
void BoundConstructor(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  new(self) ClassType(arg0);
}

template <typename ClassType, typename Arg0Type> 
Function* FromConstructor()
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundConstructor<ClassType, Arg0Type>; 
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  return fn;
}

template <typename ClassType, typename Arg0, typename Arg1>
void BoundConstructor(Call& call)
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Arg0Type arg0 = call.Get<Arg0Type>(0);
  Arg1Type arg1 = call.Get<Arg1Type>(1);
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  new(self) ClassType(arg0, arg1);
}

template <typename ClassType, typename Arg0Type, typename Arg1Type> 
Function* FromConstructor()
{
  typedef UnqualifiedType<Arg0>::type Arg0Type;
  typedef UnqualifiedType<Arg1>::type Arg1Type;
  Function* fn = new Function();
  fn->mBoundFunction = &BoundConstructor<ClassType, Arg0Type, Arg1Type>; 
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<Arg0Type>::GetBoundType());
  fn->SetParamType(1, StaticTypeId<Arg1Type>::GetBoundType());
  return fn;
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
Function* FromCopyConstructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundCopyConstructor<ClassType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  fn->SetParamType(0, StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

template <typename ClassType>
void BoundDestructor(Call& call)
{
  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType>::GetBoundType());
  ClassType* self = *reinterpret_cast<ClassType**>(location);
  self->~ClassType();
}

template <typename ClassType>
Function* FromDestructor()
{
  Function* fn = new Function();
  fn->mBoundFunction = &BoundDestructor<ClassType>;
  fn->SetThisType(StaticTypeId<ClassType>::GetBoundType());
  return fn;
}

