#include "StandardFileManager.hpp"

#include <type_traits>

#define SFM_ADD_ERROR(ErrorType) \
  FileContentChangerI::Errors.push(std::make_unique<ErrorMain>(ErrorType{}))

#define SFM_REMOVE_CONST_CV \
  const_cast<StandardFileManager*>(this)

#define SFM_ACCESS_GOOD(expression, error) \
  { \
    expression; \
    if (!BaseFile::good()) \
    { \
      SFM_ADD_ERROR(error); \
      return; \
    } \
  }

#define SFM_EXCEPT_GOOD(expression, error) \
  { \
    expression; \
    if (!BaseFile::good()) \
    { \
      SFM_ADD_ERROR(error); \
    } \
  }

StandardFileManager::StandardFileManager(std::string targetFileName, bool isCreateFileIfNotExist, bool isDeleteFileOnClosing)
  : _targetFileName(std::move(targetFileName))
  , _isCreateFileIfNotExist(isCreateFileIfNotExist)
  , _isDeleteFileOnClosing(isDeleteFileOnClosing)
  , _fileSize{ /*size*/ 0, /*isCached*/ false }
{
  auto openFileMode = BaseFile::binary | BaseFile::in | BaseFile::out | BaseFile::ate;
  if (isCreateFileIfNotExist)
    openFileMode |= BaseFile::app;
  BaseFile::open(_targetFileName, openFileMode);
  if (!BaseFile::is_open())
    SFM_ADD_ERROR(ErrorFileCannotBeOpen);
  else
    GetFileSize();
}

StandardFileManager::~StandardFileManager()
{
  BaseFile::close();
  if (_isDeleteFileOnClosing)
    if (std::remove(_targetFileName.c_str()) != NULL)
      SFM_ADD_ERROR(ErrorFileCannotBeRemove);
}

FileContentChangerI::FileSizeType StandardFileManager::GetFileSize() const
{
  if (!_fileSize.isCached)
  {
    FileSizeType currentPosition = GetCursorPosition();
    SFM_REMOVE_CONST_CV->SetCursorToEnd();
    std::streampos lastPosition = GetCursorPosition();
    SFM_EXCEPT_GOOD(SFM_REMOVE_CONST_CV->seekp(static_cast<std::streamoff>(currentPosition), std::ios_base::beg), ErrorCannotSetCursoreInfile);
    _fileSize.size = static_cast<FileSizeType>(lastPosition);
    _fileSize.isCached = true;
  }
  return _fileSize.size;
}

void StandardFileManager::SetCursorToEnd()
{
  SFM_ACCESS_GOOD(BaseFile::seekp(0, std::ios_base::end), ErrorCannotSetCursoreToEndOfFile);
}

void StandardFileManager::SetCursorToBegin()
{
  SFM_ACCESS_GOOD(BaseFile::seekp(0, std::ios_base::beg), ErrorCannotSetCursoreToBeginOfFile);
}

void StandardFileManager::SetCursorPosition(FileSizeType position)
{
  if (position > GetFileSize())
    SFM_ADD_ERROR(ErrorOutOfFileSize);
  else
    SFM_ACCESS_GOOD(BaseFile::seekp(static_cast<std::streamoff>(position), std::ios_base::beg), ErrorCannotSetCursoreInfile);
}

FileContentChangerI::FileSizeType StandardFileManager::GetCursorPosition() const
{
  std::streampos currentPosition;
  SFM_EXCEPT_GOOD(currentPosition = SFM_REMOVE_CONST_CV->tellp(), ErrorGetCurrentCursorPositionFail);
  return static_cast<FileSizeType>(currentPosition);
}

void StandardFileManager::WriteData(const BufferType* buffer, FileSizeType size)
{
  _fileSize.isCached = false;
  SFM_ACCESS_GOOD(BaseFile::write(buffer, static_cast<std::streamsize>(size)), ErrorWriteDataFail);
}

std::vector<StandardFileManager::BufferType> StandardFileManager::ReadData(FileSizeType size)
{
  std::vector<BufferType> result(std::min<size_t>(size, GetRemainingFileSize()), '\0');
  SFM_EXCEPT_GOOD(BaseFile::read(result.data(), static_cast<std::streamsize>(result.size())), ErrorReadDataFail);
  return result;
}

FileContentChangerI::FileSizeType StandardFileManager::GetRemainingFileSize()
{
  return GetFileSize() - GetCursorPosition();
}

#undef SFM_EXCEPT_GOOD
#undef SFM_ACCESS_GOOD
#undef SFM_REMOVE_CONST_CV
#undef SFM_ADD_ERROR
