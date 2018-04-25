// smart_cell.h in in OTUS home work #6 project

#pragma once

template <typename Container, size_t... Indices>
auto containerToTupleHelper(Container source, std::index_sequence<Indices...> ) {
    return std::make_tuple(source[Indices]...);
}

template <size_t N, typename Container>
auto containerToTuple(Container source) {
    return containerToTupleHelper(source, std::make_index_sequence<N>{} );
}

template<typename T, int defaultValue>
class SmartCell
{
public:
  SmartCell() :
    index{0}, value{defaultValue}, parent{nullptr}{}

  SmartCell(const size_t& _index,
            const T& _value,
            LazyMatrix<T, defaultValue, 1>* _parent) :
    index{_index}, value{_value}, parent{_parent}
  {}

  template <typename U>
  typename std::enable_if<std::is_arithmetic<U>::value, SmartCell<T, defaultValue>&>::type
  operator=(const U& newValue)
  {
    if (defaultValue == newValue && parent != nullptr)  // ask to remove this cell
    {
      parent->removeItem(index);
      parent->tmpCell.index = index;
      return parent->tmpCell;
    }
    else
    {
      if (value == defaultValue && parent != nullptr)   // this is a temporary cell
      {
        parent->data[index].value        = static_cast<T>(newValue);
        parent->data[index].index        = index;
        parent->data[index].parent       = parent;
        parent->data[index].upperIndices = upperIndices;
        return parent->data[index];
      }
      else                                               // this is an existing cell
      {
        value = static_cast<T>(newValue);
        return *this;
      }
    }
  }

  template <typename U, class = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
  operator U ()
  {
    return static_cast<U>(value);
  }

  operator std::tuple<std::list<size_t>, T> ()
  {
    std::list<T> indexList;
    indexList.push_back(index);
    for(auto idx : upperIndices)
    {
      indexList.push_back(idx);
    }
    return std::make_tuple(indexList, value);
  }

  template<typename U>
  typename std::enable_if<std::is_arithmetic<U>::value, bool>::type
  operator== (const U& otherValue)
  {
    return (value == static_cast<T>(otherValue));
  }

  auto makeTuple()
  {
    std::list<size_t> indexList;
    indexList.push_back(index);
    for(auto idx : upperIndices)
    {
      indexList.push_back(idx);
    }
    return std::make_tuple(indexList, value);
  }

  size_t index{};
  T value{};
  std::list<size_t> upperIndices{};
  LazyMatrix<T, defaultValue, 1>* parent;
};

template <typename T, T defaultValue>
bool operator< (const SmartCell<T, defaultValue>& a,
                const SmartCell<T, defaultValue>& b )
{
  return a.index < b.index;
}

template <typename T, T defaultValue>
typename std::enable_if<std::is_arithmetic<T>::value, std::ostream&>::type
operator<< (std::ostream& outputStream, const std::pair<const size_t, SmartCell<T, defaultValue>>& cellPair)
{
  outputStream << cellPair.second.value;
  return outputStream;
}


template<typename T, T defaultValue>
std::ostream& operator<< (std::ostream& outputStream, const SmartCell<T, defaultValue>& cell)
{
  outputStream << cell.value;
  return outputStream;
}

template <typename T>
std::ostream&
operator<< (std::ostream& outputStream,
            const std::tuple<std::list<size_t>, T>& cell)
{
  std::list<size_t> indices;
  T v{};
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
  outputStream << "]:" << v << std::endl;

  return outputStream;
}
