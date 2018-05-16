// process.cpp in OTUS home work #6 project

#include "homework_6.h"
#include <cassert>

void homework_6(std::ostream& outputStream)
{
  LazyMatrix<int, 0, 2> matrix{};
  for (size_t i {0}; i < 10; i++)
  {
    matrix[i][i] = i;
    matrix[9 - i][i] = 9 - i;
  }

  for (size_t i {1}; i <= 8; i++)
  {
    for (size_t j {1}; j <= 8; j++)
    {
      outputStream << matrix[i][j] << " ";
    }
    outputStream << '\n';
  }
  for (const auto& cell : matrix)
  {
    int x{}, y{};
    int v{};
    std::tie(x, y ,v) = cell;
    outputStream << '[' << x    << ',' <<
                  y  << "]=" << v   <<
                 '\n';    
  }
  outputStream << "size: " << matrix.size() << std:: endl;
}
