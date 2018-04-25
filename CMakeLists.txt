#CmakeList.txt for OTUS Homework #6

#Check CMake version
cmake_minimum_required(VERSION 3.2)

#Set project name, set PROJECT_VERSION_MINOR value equal to TRAVIS_BUILD NUMBER
if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
  set (VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
else ()
  set (VERSION_PATCH 1)
endif()

project(matrix VERSION 0.0.${VERSION_PATCH})

set (BOOST_MIN_VERSION "1.54.0")

#Find Boost UTF component, get BOOST_LIBRARIES and BOOST_INCLUDE_DIR values
find_package(Boost COMPONENTS unit_test_framework REQUIRED)


#Set library target
add_library(lazy_matrix INTERFACE)
target_sources(lazy_matrix INTERFACE ${PROJECT_SOURCE_DIR}/lazy_matrix.h)
target_include_directories(lazy_matrix INTERFACE ${PROJECT_SOURCE_DIR})

add_library(smart_cell INTERFACE)
target_sources(smart_cell INTERFACE ${PROJECT_SOURCE_DIR}/smart_cell.h)
target_include_directories(smart_cell INTERFACE ${PROJECT_SOURCE_DIR})


add_library(process process.cpp)

#Set executable target
set(HEADERS ${PROJECT_SOURCE_DIR}/lazy_matrix.h ${PROJECT_SOURCE_DIR}/smart_cell.h)
add_executable(matrix matrix.cpp ${HEADERS})


#Set test target_link_libraries
add_executable(otus_hw_6_test otus_hw_6_test.cpp)

#Enable tests
enable_testing()
add_test(
NAME test_hw_6
COMMAND otus_hw_6_test)

#Set properties and compile options for executable target
set_target_properties(matrix
  PROPERTIES CXX_STANDARD 14
             CXX_STANDARD_REQUIRED ON
             COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

#Set properties for test target
set_target_properties(otus_hw_6_test PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

#Link
target_link_libraries(matrix process)
target_link_libraries(process INTERFACE lazy_matrix)
target_link_libraries(lazy_matrix INTERFACE smart_cell)
target_link_libraries(otus_hw_6_test
  ${Boost_LIBRARIES}
  process
  lazy_matrix
)

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT alexandr_kudinov@mail.ru)

include(CPack)
