#pragma once

#include <set>
#include <map>
#include <tuple>
#include <utility>
#include <cassert>
#include <iostream>

/* Array to tuple conversion function */
template <typename T, size_t N, size_t... Indices>
auto arrayToTupleHelper(std::array<T, N> const& array,
                        std::index_sequence<Indices...>)
{
    return std::make_tuple(array[Indices]...);
}

template <typename T, size_t N>
auto arrayToTuple(std::array<T, N> const& array)
{
    return arrayToTupleHelper(array, std::make_index_sequence<N>{});
}

/* Tuple of size_t indices type declaration */
template<size_t... Indices>
auto indexTupleHelper(std::index_sequence<Indices...>)
{
  return std::make_tuple(Indices...);
}

template<size_t N, typename Indices = std::make_index_sequence<N>>
using indexTuple = decltype(indexTupleHelper(Indices{}));


/* Matrix element type declaration */
template<size_t dimension, typename T>
using LazyMatrixElement = decltype(std::tuple_cat(indexTuple<dimension>{}, std::make_tuple(T{})));


/* Matrix element operator<< overloading */
template<typename... Types, size_t... Indices>
auto
extractIndicesHelper(const std::tuple<Types...>& tuple, std::index_sequence<Indices...>)
{
  return std::array<size_t, sizeof...(Types) - 1> {std::get<Indices>(tuple)...};
}

template<typename... Types>
auto extractIndices(const std::tuple<Types...>& tuple)
{
  return extractIndicesHelper(tuple, std::make_index_sequence<sizeof...(Types) - 1>{});
}

template<typename... Types>
std::ostream&
operator<<(std::ostream& outputStream,
           const std::tuple<Types...> cell)
{
  auto indexArray{extractIndices(cell)};
  auto iter(std::cbegin(indexArray));
  outputStream << '[' << *iter;
  ++iter;
  for (;iter != std::cend(indexArray); ++iter)
  {
    outputStream << ',' << *iter;
  }
  outputStream << "]=" << std::get<sizeof...(Types) - 1>(cell) << '\n';
  return outputStream;
}

/*---------------------------------------------------------------------------------------------*/
/*                       Matrix class                                                          */
/*---------------------------------------------------------------------------------------------*/

template <typename T, int defaultvalue, size_t dimension = 2, typename Enable = void>
class LazyMatrix;

template <typename T, int defaultValue, size_t dimension>
class LazyMatrix<
                  T,
                  defaultValue,
                  dimension,
                  typename std::enable_if<
                                          std::is_arithmetic<T>::value &&
                                          dimension >= 1
                                         >::type
                >
{

  /* Helper class for correct assignment opeartions */
  struct SmartCell
  {
    explicit SmartCell(LazyMatrix<T, defaultValue, dimension>* _matrix) :
      matrix{_matrix}
    {
      if(nullptr == matrix)
      {
        throw std::invalid_argument("Parent matrix pointer not defined");
      }
    }

    SmartCell& operator[](size_t index) noexcept(false)
    {
      address[nextIndex] = index;
      if (nextIndex < dimension - 1)       // address not complete
      {
        ++nextIndex;
      }
      else if (dimension - 1 == nextIndex) // address complete
      {
        if (matrix->serviceData.find(arrayToTuple(address))
            != matrix->serviceData.end())
        {
          value = matrix->serviceData[arrayToTuple(address)];
        }
        else
        {
          value = defaultValue;
        }
        ++nextIndex;
      }
      else                                   // wrong number in indices
      {
        throw std::out_of_range{"Index beyond matrix dimension"};
        /* If you have reached this point, you try to index the matrix
         * beyond its dimension. E.g. use M[0][0][0] for a
         * 2-dimensional one*/
      }
      return *this;
    }

    const SmartCell& operator[](size_t index) const noexcept(false)
    {
      address[nextIndex] = index;
      if (nextIndex < dimension - 1)       // address not complete
      {
        ++nextIndex;
      }
      else if (dimension - 1 == nextIndex) // address complete
      {
        if (matrix->serviceData.find(arrayToTuple(address))
            != matrix->serviceData.end())
        {
          value = matrix->serviceData[arrayToTuple(address)];
        }
        else
        {
          value = defaultValue;
        }
        ++nextIndex;
      }
      else                                   // wrong number in indices
      {
        throw std::out_of_range{"Index beyond matrix dimension"};
        /* If you have reached this point, you try to index the matrix
         * beyond its dimension. E.g. use M[0][0][0] for a
         * 2-dimensional one*/
      }
      return *this;
    }


    template <typename U>
    typename std::enable_if<std::is_arithmetic<U>::value, SmartCell&>::type
    operator=(const U& newValue) noexcept(false)
    {      

      if (dimension != nextIndex)
      {
        throw(std::out_of_range{"Wrong index range"});
      }

      if(nullptr == matrix)
      {
        throw std::invalid_argument("Parent matrix pointer not defined");
      }

      if (defaultValue == newValue)
      {
        matrix->serviceData.erase(arrayToTuple(address));
        matrix->userData.erase(std::tuple_cat(address, std::tie(value)));
      }
      else
      {
        matrix->serviceData[arrayToTuple(address)] = newValue;
        matrix->userData.erase(std::tuple_cat(address, std::tie(value)));
        matrix->userData.insert(std::tuple_cat(arrayToTuple(address), std::tie(newValue)));
      }
      value = newValue;
      return *this;
    }

    template <typename U, class = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
    operator U() noexcept(false)
    {

      if (dimension != nextIndex)
      {
        throw(std::out_of_range{"Wrong index range"});
      }

      return static_cast<U>(value);
    }

    template<typename U>
    typename std::enable_if<std::is_arithmetic<U>::value, bool>::type
    operator==(const U& otherValue) const noexcept(false)
    {
      if (dimension != nextIndex)
      {
        throw(std::out_of_range{"Wrong index range"});
      }

      return (value == static_cast<T>(otherValue));
    }

    friend std::ostream&
    operator<< (std::ostream& outputStream, const SmartCell& cell)
    {
      outputStream << cell.value;
      return outputStream;
    }

    mutable std::array<size_t, dimension> address{};
    mutable size_t nextIndex{};
    mutable T value{defaultValue};
    LazyMatrix<T, defaultValue, dimension>* const matrix{nullptr};
  };


public:
  LazyMatrix(){}

  SmartCell& operator[](const size_t& index) noexcept(true)
  {
    dummyCell.address[0] = index;
    dummyCell.nextIndex = 1;
    return dummyCell;
  }

  const SmartCell& operator[](const size_t& index) const noexcept(true)
  {
    dummyCell.address[0] = index;
    dummyCell.nextIndex = 1;
    return dummyCell;
  }

  size_t size() const noexcept(true)
  {
    return userData.size();
  }

  auto begin() noexcept(true)
  {
    return userData.begin();
  }

  auto end() noexcept(true)
  {
    return userData.end();
  }

  auto cbegin() const noexcept(true)
  {
    return userData.cbegin();
  }

  auto cend() const noexcept(true)
  {
    return userData.cend();
  }


private:
  std::set<LazyMatrixElement<dimension, T>> userData;
  std::map<indexTuple<dimension>, T> serviceData;

  SmartCell dummyCell{this};
};

