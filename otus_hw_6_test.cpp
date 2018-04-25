// otus_hw_6.cpp in OTUS home work #6 project

#define BOOST_TEST_MODULE OTUS_HW_4_TEST

#include <boost/test/unit_test.hpp>
#include "process.h"
#include "lazy_matrix.h"
#include <list>
#include <string>
#include <random>

BOOST_AUTO_TEST_SUITE(homework_6_test)

BOOST_AUTO_TEST_CASE(check_main_task)
{
  std::string testString
  {
    "1 0 0 0 0 0 0 1 \n"
    "0 2 0 0 0 0 2 0 \n"
    "0 0 3 0 0 3 0 0 \n"
    "0 0 0 4 4 0 0 0 \n"
    "0 0 0 5 5 0 0 0 \n"
    "0 0 6 0 0 6 0 0 \n"
    "0 7 0 0 0 0 7 0 \n"
    "8 0 0 0 0 0 0 8 \n"
    "[9,0]:9\n"
    "[1,1]:1\n"
    "[8,1]:8\n"
    "[2,2]:2\n"
    "[7,2]:7\n"
    "[3,3]:3\n"
    "[6,3]:6\n"
    "[4,4]:4\n"
    "[5,4]:5\n"
    "[4,5]:4\n"
    "[5,5]:5\n"
    "[3,6]:3\n"
    "[6,6]:6\n"
    "[2,7]:2\n"
    "[7,7]:7\n"
    "[1,8]:1\n"
    "[8,8]:8\n"
    "[9,9]:9\n"
    "size: 18\n"
  };
  std::stringstream testStream{};
  process(testStream);
  /* check output correctness */
  BOOST_CHECK(testStream.str() == testString);
}

BOOST_AUTO_TEST_CASE(check_5_dimensions)
{
  LazyMatrix<double, -2, 5> testMatrix{};
  std::stringstream testStrings{};
  for (size_t n{0}; n < 1000; n++)
  {
    size_t i {std::rand() % 1000};
    size_t j {std::rand() % 1000};
    size_t k {std::rand() % 1000};
    size_t l {std::rand() % 1000};
    size_t m {std::rand() % 1000};
    testMatrix[m][l][k][j][i] = i * 0.33 + j * 0.33 + k * 0.33 + l * 0.33 + m * 0.33;
    testStrings << '[' << i << ',' << j << ',' << k << ',' << l << ',' << m << "]:" << i * 0.33 + j*0.33 + k * 0.33 + l * 0.33 + m * 0.33<< '\n';
  }
  std::list<size_t> indices{};
  double v{};
  std::stringstream testStream{};
  for (const auto cell : testMatrix)
  {
    std::tie(indices,v) = cell;
    testStream << cell;
  }
  /* check matrix size */
  BOOST_CHECK(testMatrix.size() == 1000);
  /*check indexing correctness*/
  BOOST_CHECK(std::accumulate(indices.begin(), indices.end(), 0) * 0.33 == v);

  std::string str1{testStrings.str()};
  std::string str2{testStream.str()};
  auto checkSumm1 {std::accumulate(str1.begin(), str1.end(), 0,
                    [](int sum, const char& ch) {return sum + ch;})};
  auto checkSumm2  {std::accumulate(str2.begin(), str2.end(), 0,
                    [](int sum, const char& ch) {return sum + ch;})};

  /* check content output correctness */
  BOOST_CHECK(checkSumm1 == checkSumm2);
}

BOOST_AUTO_TEST_CASE(check_canonical_equal_operator)
{
  LazyMatrix<float, -200, 3> testMatrix{};
  ((((testMatrix[90][2456][789] = -5678.30007) = -200) = 1280.451) = -200) = -200.001;
  /* check matrix size */
  BOOST_CHECK(testMatrix.size() == 1);
  /* check equal operator correctness */
  BOOST_CHECK(testMatrix[90][2456][789] == -200.001);

  /* check reset to default */
  testMatrix[90][2456][789] = -200;
  BOOST_CHECK(testMatrix.size() == 0);
  BOOST_CHECK(testMatrix[90][2456][789] == -200);
}

BOOST_AUTO_TEST_SUITE_END()
