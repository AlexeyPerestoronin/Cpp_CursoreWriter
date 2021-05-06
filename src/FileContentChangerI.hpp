#pragma once

#include "Ptr.hpp"

#include <vector>
#include <queue>

class FileContentChangerI
{
  Ptr(FileContentChangerI)

public:
  using BufferType = char;
  using FileSizeType = uint64_t;

public:
  static std::queue<std::exception> Errors;

public:
  virtual FileSizeType GetFileSize() const = 0;

  virtual void SetCourseToEnd() = 0;
  virtual void SetCourseToBegin() = 0;
  virtual void SetCoursePosition(FileSizeType position) = 0;
  virtual FileSizeType GetCoursePosition() const = 0;

  virtual void WriteData(const BufferType* buffer, FileSizeType size) = 0;
  virtual std::vector<BufferType> ReadData(FileSizeType size) = 0;
};
