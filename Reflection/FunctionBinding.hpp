#pragma once

#include "StaticTypeId.hpp"

#include "Function.hpp"
#include "Call.hpp"

namespace SimpleReflection
{

struct BoundType;

// Ideally these would be replaced with a bunch of variadics, but the unpack is very complicated for that.
// Additionally, there's a lot of issues where const functions need to be implemented (since they're separate)
// and there may be some issues with qualified types as well that were missed.
#include "Functions.inl"
#include "ConstructorsDestructors.inl"

#define CreateMethod(Fn) SimpleReflection::FromMethod<decltype(Fn), Fn>(Fn)

}//namespace SimpleReflection
