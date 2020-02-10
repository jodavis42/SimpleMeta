#include "MetaLibrary.hpp"


std::vector<BoundType*> MetaLibrary::mBoundTypes;
std::unordered_map<std::string, BoundType*> MetaLibrary::mBoundTypeMap;
std::unordered_map<size_t, BoundType*> MetaLibrary::mBoundTypeIdMap;
