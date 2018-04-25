// lazy_matrix.h in OTUS home work #6 project

#pragma once

#include <map>
#include <list>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iterator>

template <typename T, int defaultvalue, size_t dimension = 2, typename Enable = void>
class LazyMatrix;

#include "smart_cell.h"

/*--------------------------------------------------------------------------------*/

template <typename T, int defaultValue>
class LazyMatrix<
                  T,
                  defaultValue,
                  1,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type
                >
{
public:

  using CellType = SmartCell<T, defaultValue>;

  using iterator_value_type = std::tuple<std::list<size_t>, T>;

  using iterator_pointer_type = T*;

  using iterator_reference_type = T&;

  class Iterator : public std::iterator<
                                        std::forward_iterator_tag,
                                        iterator_value_type,
                                        std::ptrdiff_t,
                                        iterator_pointer_type,
                                        iterator_reference_type
                                       >
  {
  public:

    using value_type = iterator_value_type;
    using pointer = iterator_pointer_type;
    using reference = iterator_reference_type;

    using MapIterator = typename std::map<size_t, SmartCell<T, defaultValue>>::iterator;
    using MapConstIterator = typename std::map<size_t, SmartCell<T, defaultValue>>::const_iterator;

    Iterator(){}
    Iterator(MapIterator _mapIterator) : mapIterator{_mapIterator}{}
    Iterator(const Iterator& other) : mapIterator{other.mapIterator} {}
    Iterator(Iterator&& other) {std::swap(mapIterator, other.mapIterator);}
    Iterator& operator=(const Iterator& other) {mapIterator = other.mapIterator; return *this;}
    Iterator& operator=(Iterator&& other){std::swap(mapIterator, other.mapIterator); return *this;}

    Iterator operator++()                                 // prefix increment
    {
      ++mapIterator;
      return *this;
    }

    Iterator operator++(int)                              // prefix increment
    {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    value_type operator*() { return mapIterator->second.makeTuple();}
    reference operator->() { return mapIterator->second.value;}
    bool operator==(const Iterator& other) { return mapIterator == other.mapIterator; }
    bool operator!=(const Iterator& other) { return mapIterator != other.mapIterator; }

  private:
    typename std::map<size_t, SmartCell<T, defaultValue>>::iterator mapIterator;
  };

  using iterator = Iterator;

  Iterator begin()
  {
    return Iterator{data.begin()};
  }

  Iterator end()
  {
    return Iterator{data.end()};
  }

  SmartCell<T, defaultValue>& operator[](const size_t& idx)
  {
    if (data.find(idx) != data.end()) // cell exists
    {
      return data[idx];
    }
    else                              // no such index - use the temporary cell
    {
      tmpCell.value        = defaultValue;
      tmpCell.index        = idx;
      tmpCell.parent       = this;
      tmpCell.upperIndices = upperIndices;
      return tmpCell;
    }
  }

  size_t size() const
  {
    return data.size();
  }

  void removeItem(size_t index)
  {
    data.erase(index);
  }

  friend class SmartCell<T, defaultValue>;

  friend class LazyMatrix<T, defaultValue, 2>;

  friend class Iterator;

private:
  std::list<size_t> upperIndices;
  SmartCell<T, defaultValue> tmpCell {0,defaultValue, this};
  std::map<size_t, SmartCell<T, defaultValue>> data;
};

template <typename T, int defaultValue>
std::ostream&
operator<< (std::ostream& outputStream,
            const std::pair<const size_t, LazyMatrix<T, defaultValue, 1>> matrixPair)
{
  if (matrixPair.second.size() != 0)
  {
    for (const auto& cellPair : matrixPair.second)
    {
      outputStream << cellPair << " ";
    }
    outputStream << std::endl;
  }
  return outputStream;
}


/*--------------------------------------------------------------------------------*/

template <typename T, int defaultValue, size_t dimension>
class LazyMatrix<
                  T,
                  defaultValue,
                  dimension,
                  typename std::enable_if<std::is_arithmetic<T>::value>::type
                >
{
public:

  using iterator_value_type = LazyMatrix<T, defaultValue, dimension - 1>;
  using iterator_pointer_type = LazyMatrix<T, defaultValue, dimension - 1>*;
  using iterator_reference_type = LazyMatrix<T, defaultValue, dimension - 1>&;

  class Iterator : public std::iterator<
                                        std::forward_iterator_tag,
                                        iterator_value_type,
                                        std::ptrdiff_t,
                                        iterator_pointer_type,
                                        iterator_reference_type
                                       >
  {
  public:

    using MapIterator = typename std::map<
                                          size_t,
                                          LazyMatrix<T, defaultValue, dimension - 1>
                                         >::iterator;

    using CellIterator = typename LazyMatrix<T, defaultValue, dimension - 1>::iterator;


    Iterator(){}

    Iterator(MapIterator _mapIterator, MapIterator _mapEnd) :
      mapIterator{_mapIterator}, mapEnd{_mapEnd}
    {
      if (mapIterator == mapEnd)
      {
        return;
      }

      cellIterator = mapIterator->second.begin();
      skipEmptyContainers();
    }

    Iterator(const Iterator& other) :
      mapIterator{other.mapIterator}, mapEnd{other.mapEnd}, cellIterator{other.cellIterator} {}

    Iterator(Iterator&& other)
    {
      std::swap(mapIterator, other.mapIterator);
      std::swap(mapEnd, other.mapEnd);
      std::swap(cellIterator, other.cellIterator);
    }

    Iterator& operator=(const Iterator& other)
    {
      mapIterator = other.mapIterator;
      mapEnd = other.mapEnd;
      cellIterator = other.cellIterator;
      return *this;
    }

    Iterator& operator=(Iterator&& other)
    {
      std::swap(mapIterator, other.mapIterator);
      std::swap(mapEnd, other.mapEnd);
      std::swap(cellIterator, other.cellIterator);
      return *this;
    }

    Iterator operator++()                                 // prefix increment
    {
      ++cellIterator;
      if (cellIterator == mapIterator->second.end())
          skipEmptyContainers();
      return *this;
    }

    Iterator operator++(int)                              // prefix increment
    {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }



    auto operator*() { return *cellIterator; }

    auto operator->() { return cellIterator; }

    bool operator==(const Iterator& other)
    {
      if (mapIterator != other.mapIterator)
      {
        return false;
      }

      if (mapIterator != mapEnd &&
          other.mapIterator != other.mapEnd &&
          cellIterator != other.cellIterator)
      {
        return false;
      }

      return true;
    }

    bool operator!=(const Iterator& other) { return !(mapIterator == other.mapIterator); }

    void skipEmptyContainers()
    {
      while (mapIterator != mapEnd && cellIterator == mapIterator->second.end()) // while map end is not reached and cell is empty
      {
        ++mapIterator;
        if (mapIterator != mapEnd) // if map is not empty
          cellIterator = mapIterator->second.begin();
      }
    }

  private:
    MapIterator mapIterator;
    MapIterator mapEnd;
    CellIterator cellIterator;
  };

  using iterator = Iterator;


  auto begin()
  {
    return Iterator(data.begin(), data.end());
  }

  auto end()
  {
    return Iterator(data.end(), data.end());
  }

  LazyMatrix<T, defaultValue, dimension - 1>&
  operator[](size_t idx)
  {
    if (data.find(idx) == data.end())       // submatrix doesn't exist
    {
      data[idx].upperIndices = upperIndices;
      data[idx].upperIndices.push_front(idx);
    }
    return data[idx];
  }

  size_t size() const
  {
    size_t result{};
    for (auto const& matrix : data)
    {
         result += matrix.second.size();
    }
    return result;
  }

  friend class LazyMatrix<T, defaultValue, dimension + 1>;

private:
  std::list<size_t> upperIndices{};
  std::map<
            size_t,
            LazyMatrix<T, defaultValue, dimension - 1>
          > data;
};

template <typename T, int defaultValue, size_t dimension>
std::ostream&
operator << (std::ostream& outputStream,
             LazyMatrix<T, defaultValue, dimension>& matrix)
{
  if (matrix.size() == 0)
  {
    return outputStream;
  }

  for (const auto& cell : matrix)
  {
    std::list<size_t> indices;
    T v{};
    std::tie(indices,v) = cell;
    std::cout << '[';
    bool first{true};
    for (const auto& idx : indices)
    {
      if (!first)
      {
         std::cout << ',';
      }
      first = false;
      std::cout << idx;
    }
    std::cout << "]:" << v << std::endl;
  }

  return outputStream;
}

