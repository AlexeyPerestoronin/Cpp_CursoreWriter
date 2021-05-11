#pragma once

#include "Ptr.hpp"
#include "FixedQueue.hpp"

class FileContentChangerI
{
  Ptr(FileContentChangerI)

public:
  using BufferType = char;
  using FileSizeType = uint64_t;

#pragma region native errors
public:
  class ErrorMain : public std::exception
  {
    Ptr(ErrorMain)

  public:
    virtual char const* what() const override
    {
      return "some error was be raised";
    }
  };

  class ErrorFileCannotBeOpen : public ErrorMain
  {
    Ptr(ErrorFileCannotBeOpen)

  public:
    virtual char const* what() const override
    {
      return "file cannot be open";
    }
  }; 

  class ErrorFileCannotBeRemove : public ErrorMain
  {
    Ptr(ErrorFileCannotBeRemove)

  public:
    virtual char const* what() const override
    {
      return "file cannot be remove";
    }
  }; 
  
  class ErrorCannotSetCursoreToEndOfFile : public ErrorMain
  {
    Ptr(ErrorCannotSetCursoreToEndOfFile)

  public:
    virtual char const* what() const override
    {
      return "cannot set cursor to end of file";
    }
  };

  class ErrorCannotSetCursoreToBeginOfFile : public ErrorMain
  {
    Ptr(ErrorCannotSetCursoreToBeginOfFile)

  public:
    virtual char const* what() const override
    {
      return "cannot set cursor to begin of file";
    }
  };

  class ErrorCannotSetCursoreInfile : public ErrorMain
  {
    Ptr(ErrorCannotSetCursoreInfile)

  public:
    virtual char const* what() const override
    {
      return "cannot set cursor to target positino in file";
    }
  }; 

  class ErrorOutOfFileSize : public ErrorMain
  {
    Ptr(ErrorOutOfFileSize)

  public:
    virtual char const* what() const override
    {
      return "requested cursor position is out of file size";
    }
  };

  class ErrorGetCurrentCursorPositionFail : public ErrorMain
  {
    Ptr(ErrorGetCurrentCursorPositionFail)

  public:
    virtual char const* what() const override
    {
      return "cannot get current cursor position";
    }
  };

  class ErrorWriteDataFail : public ErrorMain
  {
    Ptr(ErrorWriteDataFail)

  public:
    virtual char const* what() const override
    {
      return "write data operation was fail";
    }
  };

  class ErrorReadDataFail : public ErrorMain
  {
    Ptr(ErrorReadDataFail)

  public:
    virtual char const* what() const override
    {
      return "read data operation was fail";
    }
  };

#pragma endregion

public:
  static FixedQueue<ErrorMain::UniPtr, 10> Errors;

public:
  virtual FileSizeType GetFileSize() const = 0;

  virtual void SetCursorToEnd() = 0;
  virtual void SetCursorToBegin() = 0;
  virtual void SetCursorPosition(FileSizeType position) = 0;
  virtual FileSizeType GetCursorPosition() const = 0;

  virtual void WriteData(const BufferType* buffer, FileSizeType size) = 0;
  virtual std::vector<BufferType> ReadData(FileSizeType size) = 0;
};
