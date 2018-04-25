// process.cpp in OTUS home work #6 project

#include "process.h"
#include <cassert>

void process(std::ostream& outputStream)
{
  LazyMatrix<int, 0, 2> matrix{};
  for (size_t i {0}; i < 10; i++)
  {
    matrix[i][i] = i;
    matrix[i][9 - i] = 9 - i;
  }

  for (size_t i {1}; i <= 8; i++)
  {
    for (size_t j {1}; j <= 8; j++)
    {
      outputStream << matrix[j][i] << " ";
    }
    outputStream << std::endl;
  }
  for (const auto& cell : matrix)
  {
    std::list<size_t> indices;
    int v{};
    std::tie(indices,v) = cell;
    outputStream << '[';
    bool first{true};
    for (const auto& idx : indices)
    {
      if (!first)
      {
        outputStream << ',';
      }
      first = false;
      outputStream << idx;
    }
    outputStream<< "]:" << v << std::endl;
  }
  outputStream << "size: " << matrix.size() << std:: endl;
}
