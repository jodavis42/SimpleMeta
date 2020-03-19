#include "FunctionGeneration.hpp"

#include <filesystem>
#include <fstream>

enum class StaticMode
{
  Instance, IsStatic
};

enum class ConstMode
{
  None, IsConst
};

enum class ExtensionMode
{
  None, IsExtension
};

enum class ReturnMode
{
  None, HasReturn
};


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

std::ofstream& AppendTypes(std::ofstream& stream, StaticMode staticMode, ReturnMode returnMode, size_t argCount)
{
  if(staticMode == StaticMode::Instance)
    stream << ", ClassType";
  if(returnMode == ReturnMode::HasReturn)
    stream << ", ReturnType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", Arg" << i << "Type";

  return stream;
}

void GenerateTemplateArgs(std::ofstream& stream, StaticMode staticMode, ReturnMode returnMode, size_t argCount)
{
  stream << "template <typename FunctionType, FunctionType FunctionToBind";
  if(staticMode == StaticMode::Instance)
    stream << ", typename ClassType";
  if(returnMode == ReturnMode::HasReturn)
    stream << ", typename ReturnType";
  for(size_t i = 0; i < argCount; ++i)
    stream << ", typename Arg" << i << "Type";
  stream << ">" << std::endl;
}

std::string GenerateFunctionName(StaticMode staticMode, ConstMode constMode, ReturnMode returnMode, ExtensionMode extensionMode)
{
  std::string fnName = "Bound";
  if(extensionMode == ExtensionMode::IsExtension)
    fnName += "Extension";
  if(staticMode == StaticMode::IsStatic)
    fnName += "Static";
  else
    fnName += "Instance";
  if(returnMode == ReturnMode::HasReturn)
    fnName += "Return";
  return fnName;
}

void GenerateBoundFunction(std::ofstream& stream, StaticMode staticMode, ConstMode constMode, ReturnMode returnMode, size_t argCount)
{
  std::string fnName = GenerateFunctionName(staticMode, constMode, returnMode, ExtensionMode::None);

  GenerateTemplateArgs(stream, staticMode, returnMode, argCount);

  Print(stream, "void %s(Call& call)\n", fnName.c_str());
  stream << "{" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  Arg%dType arg%d = call.Get<Arg%dType>(%d);\n", i, i, i, i);
  if(staticMode == StaticMode::Instance)
  {
    Print(stream, "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());\n");
    Print(stream, "  ClassType* self = *reinterpret_cast<ClassType**>(location);\n");
  }

  stream << "  ";
  if(returnMode == ReturnMode::HasReturn)
    stream << "ReturnType ret = ";
  stream << "(";
  if(staticMode == StaticMode::Instance)
    stream << "self->";
  stream << "*FunctionToBind)(";
  for(size_t i = 0; i < argCount; ++i)
  {
    stream << "arg" << i;
    if(i != argCount - 1)
      stream << ", ";
  }
  stream << ");" << std::endl;

  if(returnMode == ReturnMode::HasReturn)
  {
    stream << "  call.Set<ReturnType>(Call::Return, ret);" << std::endl;
  }
  stream << "}" << std::endl << std::endl;
}

void GenerateBoundExtensionFunction(std::ofstream& stream, StaticMode staticMode, ConstMode constMode, ReturnMode returnMode, size_t argCount)
{
  std::string fnName = GenerateFunctionName(staticMode, constMode, returnMode, ExtensionMode::IsExtension);

  GenerateTemplateArgs(stream, staticMode, returnMode, argCount);

  Print(stream, "void %s(Call& call)\n", fnName.c_str());
  stream << "{" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  Arg%dType arg%d = call.Get<Arg%dType>(%d);\n", i, i, i, i);
  if(staticMode == StaticMode::Instance)
  {
    Print(stream, "  char* location = call.GetLocationChecked(Call::This, StaticTypeId<ClassType*>::GetBoundType());\n");
    Print(stream, "  ClassType* self = *reinterpret_cast<ClassType**>(location);\n");
  }

  stream << "  ";
  if(returnMode == ReturnMode::HasReturn)
    stream << "ReturnType ret = ";
  stream << "(*FunctionToBind)(";
  if(staticMode == StaticMode::Instance)
  {
    stream << "self";
    if(argCount != 0)
      stream << ", ";
  }
  for(size_t i = 0; i < argCount; ++i)
  {
    stream << "arg" << i;
    if(i != argCount - 1)
      stream << ", ";
  }
  stream << ");" << std::endl;

  if(returnMode == ReturnMode::HasReturn)
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

void GenerateFromMethod(std::ofstream& stream, StaticMode staticMode, ConstMode constMode, ReturnMode returnMode, ExtensionMode extensionMode, size_t argCount)
{
  std::string fnName = "FromMethod";
  if(extensionMode == ExtensionMode::IsExtension)
    fnName = "FromExtensionMethod";

  std::string boundFnName = GenerateFunctionName(staticMode, constMode, returnMode, extensionMode);
  std::string returnType = "void";
  if(returnMode == ReturnMode::HasReturn)
    returnType = "ReturnType";
  std::string classType = "";
  std::string classTypeScope = "";
  if(staticMode == StaticMode::Instance)
  {
    classType = "ClassType";
    classTypeScope = "::";
  }

  GenerateTemplateArgs(stream, staticMode, returnMode, argCount);
  Print(stream, "Function* %s(%s(%s%s*)(", fnName.c_str(), returnType.c_str(), classType.c_str(), classTypeScope.c_str());
  AppendArgs(stream, argCount);
  Print(stream, ")%s)\n", constMode == ConstMode::IsConst ? "const" : "");
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  Print(stream, "  fn->mBoundFunction = &%s<FunctionType, FunctionToBind", boundFnName.c_str());
  AppendTypes(stream, staticMode, returnMode, argCount);
  Print(stream, ">;\n");
  Print(stream, "  fn->SetReturnType(StaticTypeId<%s>::GetBoundType());\n", returnType.c_str());
  if(staticMode == StaticMode::Instance)
    stream << "  fn->SetThisType(StaticTypeId<ClassType*>::GetBoundType());" << std::endl;
  for(size_t i = 0; i < argCount; ++i)
    Print(stream, "  fn->SetParamType(%d, StaticTypeId<Arg%dType>::GetBoundType());\n", i, i);
  stream << "  return fn;" << std::endl;
  stream << "}" << std::endl << std::endl;
}

void GenerateFromExtensionMethod(std::ofstream& stream, StaticMode staticMode, ConstMode constMode, ReturnMode returnMode, size_t argCount)
{
  std::string fnName = "FromExtensionMethod";

  std::string boundFnName = GenerateFunctionName(staticMode, constMode, returnMode, ExtensionMode::IsExtension);
  std::string returnType = "void";
  if(returnMode == ReturnMode::HasReturn)
    returnType = "ReturnType";
  std::string classType = "ClassType";

  GenerateTemplateArgs(stream, staticMode, returnMode, argCount);
  Print(stream, "Function* %s(%s(*)(%s %s*", fnName.c_str(), returnType.c_str(), constMode == ConstMode::IsConst ? "const" : "", classType.c_str());
  if(argCount != 0)
    stream << ", ";
  AppendArgs(stream, argCount);
  Print(stream, "))\n");
  stream << "{" << std::endl;
  stream << "  Function* fn = new Function();" << std::endl;
  Print(stream, "  fn->mBoundFunction = &%s<FunctionType, FunctionToBind", boundFnName.c_str());
  AppendTypes(stream, staticMode, returnMode, argCount);
  Print(stream, ">;\n");
  Print(stream, "  fn->SetReturnType(StaticTypeId<%s>::GetBoundType());\n", returnType.c_str());
  if(staticMode == StaticMode::Instance)
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
    GenerateBoundFunction(stream, StaticMode::IsStatic, ConstMode::None, ReturnMode::None, i);
    GenerateFromMethod(stream, StaticMode::IsStatic, ConstMode::None, ReturnMode::None, ExtensionMode::None, i);

    GenerateBoundFunction(stream, StaticMode::IsStatic, ConstMode::None, ReturnMode::HasReturn, i);
    GenerateFromMethod(stream, StaticMode::IsStatic, ConstMode::None, ReturnMode::HasReturn, ExtensionMode::None, i);

    GenerateBoundFunction(stream, StaticMode::Instance, ConstMode::None, ReturnMode::None, i);
    GenerateFromMethod(stream, StaticMode::Instance, ConstMode::None, ReturnMode::None, ExtensionMode::None, i);
    GenerateFromMethod(stream, StaticMode::Instance, ConstMode::IsConst, ReturnMode::None, ExtensionMode::None, i);

    GenerateBoundFunction(stream, StaticMode::Instance, ConstMode::None, ReturnMode::HasReturn, i);
    GenerateFromMethod(stream, StaticMode::Instance, ConstMode::None, ReturnMode::HasReturn, ExtensionMode::None, i);
    GenerateFromMethod(stream, StaticMode::Instance, ConstMode::IsConst, ReturnMode::HasReturn, ExtensionMode::None, i);

    GenerateBoundExtensionFunction(stream, StaticMode::Instance, ConstMode::None, ReturnMode::None, i);
    GenerateFromExtensionMethod(stream, StaticMode::Instance, ConstMode::None, ReturnMode::None, i);
    GenerateFromExtensionMethod(stream, StaticMode::Instance, ConstMode::IsConst, ReturnMode::None, i);
    GenerateBoundExtensionFunction(stream, StaticMode::Instance, ConstMode::None, ReturnMode::HasReturn, i);
    GenerateFromExtensionMethod(stream, StaticMode::Instance, ConstMode::None, ReturnMode::HasReturn, i);
    GenerateFromExtensionMethod(stream, StaticMode::Instance, ConstMode::IsConst, ReturnMode::HasReturn, i);
    
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
