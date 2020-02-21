#include "Field.hpp"

char* Field::GetFieldData(char* instanceData) const
{
  return instanceData + mOffset;
}
