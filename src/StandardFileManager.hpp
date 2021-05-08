#pragma once

#include "FileContentChangerI.hpp"

#include <string>
#include <fstream>

class StandardFileManager final
  : public FileContentChangerI
  , protected std::fstream
{
  Ptr(StandardFileManager)

private:
  using BaseFile = std::fstream;

private:
  std::string _targetFileName;
  bool _isCreateFileIfNotExist;
  bool _isDeleteFileOnClosing;
  mutable struct
  {
    FileSizeType size;
    bool isCached;
  } _fileSize;

public:
  StandardFileManager(std::string targetFileName, bool isCreateFileIfNotExist = true, bool isDeleteFileOnClosing = false);
  ~StandardFileManager();

public: // FileControllerI
  virtual FileSizeType GetFileSize() const override;

  virtual void SetCursorToEnd() override;
  virtual void SetCursorToBegin() override;
  virtual void SetCursorPosition(FileSizeType position) override;
  virtual FileSizeType GetCursorPosition() const override;

  virtual void WriteData(const BufferType* buffer, FileSizeType size) override;
  virtual std::vector<BufferType> ReadData(FileSizeType size) override;

private:
  FileSizeType GetRemainingFileSize();
};
