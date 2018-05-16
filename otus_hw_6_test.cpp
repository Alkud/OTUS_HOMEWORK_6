// otus_hw_6.cpp in OTUS home work #6 project

#define BOOST_TEST_MODULE OTUS_HW_4_TEST

#include <boost/test/unit_test.hpp>
#include "homework_6.h"
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
    "[1,1]=1\n"
    "[1,8]=1\n"
    "[2,2]=2\n"
    "[2,7]=2\n"
    "[3,3]=3\n"
    "[3,6]=3\n"
    "[4,4]=4\n"
    "[4,5]=4\n"
    "[5,4]=5\n"
    "[5,5]=5\n"
    "[6,3]=6\n"
    "[6,6]=6\n"
    "[7,2]=7\n"
    "[7,7]=7\n"
    "[8,1]=8\n"
    "[8,8]=8\n"
    "[9,0]=9\n"
    "[9,9]=9\n"
    "size: 18\n"
  };
  std::stringstream testStream{};

  try
  {
    homework_6(testStream);
  }
  catch(std::exception& ex)
  {
    BOOST_CHECK(false);
    std::cout << ex.what();
  }

  /* check output correctness */
  BOOST_CHECK(testStream.str() == testString);
}


BOOST_AUTO_TEST_CASE(check_5_dimensions)
{
  LazyMatrix<double, -2, 5> testMatrix{};
  std::stringstream testStrings{};

  const size_t i {67};
  const size_t j {582};
  const size_t k {235};
  const size_t l {1};
  const size_t m {369};

  const double testValue1 = -79.654;
  const double testValue2 = 476.345;

  try
  {
    /* Build test matrix and test string */
    testMatrix[i][j][k][l][m] = testValue1;
    testStrings << '[' << i << ',' << j << ',' << k
                << ',' << l << ',' << m << "]=" << testValue1 << '\n';

    testMatrix[m][l][k][j][i] = testValue2;
    testStrings << '[' << m << ',' << l << ',' << k
                << ',' << j << ',' << i << "]=" << testValue2 << '\n';

    std::stringstream testStream{};
    for (const auto& cell : testMatrix)
    {
      testStream << cell;
    }

    /* Check matrix size */
    BOOST_CHECK(testMatrix.size() == 2);

    /* Check assignment corrresctness */
    BOOST_CHECK(testMatrix[i][j][k][l][m] == testValue1);
    BOOST_CHECK(testMatrix[m][l][k][j][i] == testValue2);
    BOOST_CHECK(testMatrix[10][11][12][13][14] == -2);
    testMatrix[i][j][k][l][m] = -2;
    BOOST_CHECK(testMatrix.size() == 1);
    BOOST_CHECK(testMatrix[i][j][k][l][m] == -2);
    testMatrix[m][l][k][j][i] = -2;
    BOOST_CHECK(testMatrix.size() == 0);
    BOOST_CHECK(testMatrix[m][l][k][j][i] == -2);

    /* Check content output correctness */
    std::string str1{testStrings.str()};
    std::string str2{testStream.str()};
    BOOST_CHECK(str1 == str2);
  }
  catch(std::exception& ex)
  {
    BOOST_CHECK(false);
    std::cout << ex.what();
  }
}

BOOST_AUTO_TEST_CASE(check_canonical_equal_operator)
{
  try
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
  catch(std::exception& ex)
  {
    BOOST_CHECK(false);
    std::cout << ex.what();
  }
}

BOOST_AUTO_TEST_CASE(indexing_failure)
{
  LazyMatrix<float, 0, 4> testMatrix;
  BOOST_CHECK_THROW((testMatrix[1][2][3][4][5] == 0), std::out_of_range);
  BOOST_CHECK_THROW((testMatrix[1][2][3] == 0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(cell_reassignment_effect_on_matrix_size)
{
  LazyMatrix<int, 0, 2> testMatrix;
  testMatrix[0][1] = 11;
  testMatrix[0][1] = 21;
  testMatrix[0][1] = 31;
  BOOST_CHECK(testMatrix[0][1] == 31);
  BOOST_CHECK(testMatrix.size() == 1);
}

BOOST_AUTO_TEST_CASE(const_matrix_operations_test)
{
  LazyMatrix<int, 0, 2> testMatrix;
  testMatrix[0][1] = 1;
  testMatrix[1][0] = 2;
  const LazyMatrix <int, 0, 2> constTestMatrix{testMatrix};
  BOOST_CHECK(constTestMatrix[0][1] == 1);
  BOOST_CHECK(constTestMatrix[1][0] == 2);
  BOOST_CHECK(constTestMatrix.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()



