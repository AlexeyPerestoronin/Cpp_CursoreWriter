#pragma once

#include "FileContentChangerI.hpp"

#include <string>
#include <fstream>

class StandardFileManager
  : public FileContentChangerI
  , public std::fstream
{
  Ptr(StandardFileManager)

private:
  using BaseFile = std::fstream;

private:
  std::string _targetFileName;

public:
  StandardFileManager(const std::string& targetFileName);
  ~StandardFileManager();

public: // FileControllerI
  virtual FileSizeType GetFileSize() const override;

  virtual void SetCourseToEnd() override;
  virtual void SetCourseToBegin() override;
  virtual void SetCoursePosition(FileSizeType position) override;
  virtual FileSizeType GetCoursePosition() const override;

  virtual void WriteData(const BufferType* buffer, FileSizeType size) override;
  virtual std::vector<BufferType> ReadData(FileSizeType size) override;
};
