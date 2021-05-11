#pragma once
#include <memory>

#define Ptr(Type) \
public: \
  using Ptr = typename Type*; \
  using UniPtr = typename std::unique_ptr<Type>; \
  using ShaPtr = typename std::shared_ptr<Type>; \
  using WekPtr = typename std::weak_ptr<Type>; \
private:
