#include "FunctionGeneration.hpp"

#include <filesystem>
#include <fstream>

template <typename ... Params>
std::ofstream& Print(std::ofstream& stream, const char* format, Params &&... params)
{
  char buffer[500];
  _snprintf_s(buffer, sizeof(buffer), format, params...);
  stream << buffer;
  return stream;
}

std::ofstream& AppendArgs(std::ofstream& stream, size_t argCount)
{
  for(size_t i = 0; i < argCount; ++i)
  {
    stream << "Arg" << i << "Type";
    if(i != argCount - 1)
      stream << ", ";
  }
  return stream;
}

std::ofstream& AppendTypes(std::ofstream& stream, bool isStatic, bool hasReturn, size_t argCount)
{
  size_t totalArgs = isStatic + hasReturn + argCount;
  if(!isStatic)
    stream << ", ClassType";
  if(hasReturn)
    stream << ", ReturnType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", Arg" << i << "Type";

  return stream;
}

void GenerateTemplateArgs(std::ofstream& stream, bool isStatic, bool hasReturn, size_t argCount)
{
  stream << "template <typename FunctionType, FunctionType FunctionToBind";
  if(!isStatic)
    stream << ", typename ClassType";
  if(hasReturn)
    stream << ", typename ReturnType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", typename Arg" << i << "Type";
  stream << ">" << std::endl;
}

std::string GenerateFunctionName(bool isStatic, bool isConst, bool hasReturn)
{
  std::string fnName = "Bound";
  if(isStatic)
    fnName += "Static";
  else
    fnName += "Instance";
  if(hasReturn)
    fnName += "Return";
  return fnName;
}

void GenerateBoundFunction(std::ofstream& stream, bool isStatic, bool isConst, bool hasReturn, size_t argCount)
{
  std::string fnName = GenerateFunctionName(isStatic, isConst, hasReturn);

  GenerateTemplateArgs(stream, isStatic, hasReturn, argCount);

  Print(stream, "void %s(Call& call)\n", fnName.c_str());
  stream << "{" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  Arg%dType arg%d = call.Get<Arg%dType>(%d);\n", i, i, i, i);
  if(!isStatic)
  {
    Print(stream, "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());\n");
    Print(stream, "  ClassType* self = *reinterpret_cast<ClassType**>(location);\n");
  }

  stream << "  ";
  if(hasReturn)
    stream << "ReturnType ret = ";
  stream << "(";
  if(!isStatic)
    stream << "self->";
  stream << "*FunctionToBind)(";
  for(size_t i = 0; i < argCount; ++i)
  {
    stream << "arg" << i;
    if(i != argCount - 1)
      stream << ", ";
  }
  stream << ");" << std::endl;

  if(hasReturn)
  {
    stream << "  call.Set<ReturnType>(Call::Return, ret);" << std::endl;
  }
  stream << "}" << std::endl << std::endl;
}

void GenerateBoundConstructor(std::ofstream& stream, size_t argCount)
{
  stream << "template <typename ClassType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", typename Arg" << i << "Type";
  stream << ">" << std::endl;

  Print(stream, "void BoundConstructor(Call& call)\n");
  stream << "{" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  Arg%dType arg%d = call.Get<Arg%dType>(%d);\n", i, i, i, i);
  Print(stream, "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());\n");
  Print(stream, "  ClassType* self = *reinterpret_cast<ClassType**>(location);\n");

  Print(stream, "  new(self) ClassType(");
  for(size_t i = 0; i < argCount; ++i)
  {
    stream << "arg" << i;
    if(i != argCount - 1)
      stream << ", ";
  }
  stream << ");" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateBoundCopyConstructor(std::ofstream& stream)
{
  stream << "template <typename ClassType>" << std::endl;
  stream << "void BoundCopyConstructor(Call& call)" << std::endl;
  stream << "{" << std::endl;
  stream << "  ClassType& arg0 = call.Get<ClassType&>(0);" << std::endl;
  stream << "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  stream << "  char* self = *(char**)location;" << std::endl;
  stream << "  new (self) ClassType(arg0);" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateBoundDestructor(std::ofstream& stream)
{
  stream << "template <typename ClassType>" << std::endl;
  stream << "void BoundDestructor(Call& call)" << std::endl;
  stream << "{" << std::endl;
  stream << "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  stream << "  ClassType* self = *reinterpret_cast<ClassType**>(location);" << std::endl;
  stream << "  self->~ClassType();" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateFromMethod(std::ofstream& stream, bool isStatic, bool isConst, bool hasReturn, size_t argCount)
{
  std::string fnName = GenerateFunctionName(isStatic, isConst, hasReturn);
  std::string returnType = "void";
  if(hasReturn)
    returnType = "ReturnType";
  std::string classType = "";
  std::string classTypeScope = "";
  if(!isStatic)
  {
    classType = "ClassType";
    classTypeScope = "::";
  }

  GenerateTemplateArgs(stream, isStatic, hasReturn, argCount);
  Print(stream, "Function* FromMethod(%s(%s%s*)(", returnType.c_str(), classType.c_str(), classTypeScope.c_str());
  AppendArgs(stream, argCount);
  Print(stream, ")%s)\n", isConst ? "const" : "");
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  Print(stream, "  fn->mBoundFunction = &%s<FunctionType, FunctionToBind", fnName.c_str());
  AppendTypes(stream, isStatic, hasReturn, argCount);
  Print(stream, ">;\n");
  Print(stream, "  fn->SetReturnType(StaticTypeId<%s>::GetBoundType());\n", returnType.c_str());
  if(!isStatic)
    stream << "  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  fn->SetParamType(%d, StaticTypeId<Arg%dType>::GetBoundType());\n", i, i);
  stream << "  return fn;" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateFromConstructor(std::ofstream& stream, size_t argCount)
{
  stream << "template <typename ClassType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", typename Arg" << i << "Type";
  stream << "> " << std::endl;

  stream << "Function* FromConstructor()" << std::endl;
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  stream << "  fn->mBoundFunction = &BoundConstructor<ClassType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", Arg" << i << "Type";
  stream << ">; " << std::endl;
  stream << "  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  fn->SetParamType(%d, StaticTypeId<Arg%dType>::GetBoundType());\n", i, i);
  stream << "  return fn;" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateFromCopyConstructor(std::ofstream& stream)
{
  stream << "template <typename ClassType>" << std::endl;
  stream << "Function* FromCopyConstructor()" << std::endl;
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  stream << "  fn->mBoundFunction = &BoundCopyConstructor<ClassType>;" << std::endl;
  stream << "  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  stream << "  fn->SetParamType(0, StaticTypeId<const ClassType&>::GetBoundType());" << std::endl;
  stream << "  return fn;" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateFromDestructor(std::ofstream& stream)
{
  stream << "template <typename ClassType>" << std::endl;
  stream << "Function* FromDestructor()" << std::endl;
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  stream << "  fn->mBoundFunction = &BoundDestructor<ClassType>;" << std::endl;
  stream << "  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  stream << "  return fn;" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateBoundFunctions(std::ofstream& stream)
{
  for(size_t i = 0; i < 3; ++i)
  {
    GenerateBoundFunction(stream, true, false, false, i);
    GenerateFromMethod(stream, true, false, false, i);

    GenerateBoundFunction(stream, true, false, true, i);
    GenerateFromMethod(stream, true, false, true, i);

    GenerateBoundFunction(stream, false, false, false, i);
    GenerateFromMethod(stream, false, false, false, i);
    GenerateFromMethod(stream, false, true, false, i);

    GenerateBoundFunction(stream, false, false, true, i);
    GenerateFromMethod(stream, false, false, true, i);
    GenerateFromMethod(stream, false, true, true, i);
  }
}

void GenerateConstructorsDestructors(std::ofstream& stream)
{
  for(size_t i = 0; i < 3; ++i)
  {
    GenerateBoundConstructor(stream, i);
    GenerateFromConstructor(stream, i);
  }
  GenerateBoundCopyConstructor(stream);
  GenerateFromCopyConstructor(stream);
  GenerateBoundDestructor(stream);
  GenerateFromDestructor(stream);
}

std::ofstream CreateStreamTo(const std::string& folder, const std::string& fileName)
{
  std::filesystem::path path = folder;
  path.append(fileName);
  std::filesystem::create_directories(folder);
  std::string fullFilePath = path.string();

  std::ofstream stream;
  stream.open(fullFilePath.c_str(), std::ofstream::out);
  return stream;
}

void GenerateFunctions(const std::string& folder)
{
  std::ofstream stream = CreateStreamTo(folder, "Functions.inl");
  GenerateBoundFunctions(stream);

  stream = CreateStreamTo(folder, "ConstructorsDestructors.inl");
  GenerateConstructorsDestructors(stream);
}
