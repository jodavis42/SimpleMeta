#pragma once

namespace SimpleReflection
{

struct ReflectionLibrary;

ReflectionLibrary* CreateReflectionLibrary(const std::vector<ReflectionLibrary*>& dependencies, bool finalize = true);
ReflectionLibrary* CreateCoreLibrary(const std::vector<ReflectionLibrary*>& dependencies, bool finalize = true);

}//namespace SimpleReflection
