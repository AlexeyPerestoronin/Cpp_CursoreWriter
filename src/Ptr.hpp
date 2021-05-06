#pragma once
#include <memory>

#define Ptr(Type) \
public: \
  using ShaPtr = std::shared_ptr<Type>; \
  using UniPtr = std::unique_ptr<Type>; \
private:
