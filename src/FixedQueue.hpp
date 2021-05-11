#pragma once

#include <queue>

template<class Type, size_t Capacity>
class FixedQueue : public std::queue<Type>
{
private:
  using BaseType = std::queue<Type>;

public:
  template<class T>
  void push(T&& value)
  {
    if (this->size() == Capacity)
      BaseType::pop();
    BaseType::push(std::forward<T>(value));
  }

  template<class... T>
  void emplace(T&&... value)
  {
    if (this->size() == Capacity)
      BaseType::pop();
    BaseType::emplace(std::forward<T>(value)...);
  }

  size_t capacity()
  {
    return Capacity;
  }
};
