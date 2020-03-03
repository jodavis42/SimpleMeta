#include "Field.hpp"

namespace SimpleReflection
{

char* Field::GetFieldData(char* instanceData) const
{
  return instanceData + mOffset;
}

}//namespace SimpleReflection
