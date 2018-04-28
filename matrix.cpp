// matrix.cpp in OTUS home work #6 project

#include "homework_6.h"


int main()
{
  try
  {
    homework_6(std::cout);
  }
  catch(std::exception& ex)
  {
    std::cout << ex.what();
    return 1;
  }
  return 0;
}
