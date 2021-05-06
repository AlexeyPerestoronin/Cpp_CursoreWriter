#include "StandardFileManager.hpp"

#include <type_traits>

StandardFileManager::StandardFileManager(std::string targetFileName, bool isCreateFileIfNotExist, bool isDeleteFileOnClosing)
  : _targetFileName(std::move(targetFileName))
  , _isCreateFileIfNotExist(isCreateFileIfNotExist)
  , _isDeleteFileOnClosing(isDeleteFileOnClosing)
{
  auto openFileMode = BaseFile::binary | BaseFile::in | BaseFile::out | BaseFile::ate;
  if (isCreateFileIfNotExist)
    openFileMode |= BaseFile::trunc;
  BaseFile::open(_targetFileName, openFileMode);
  if (!BaseFile::is_open())
     FileContentChangerI::Errors.emplace("target file cannot be opening");
}

StandardFileManager::~StandardFileManager()
{
  BaseFile::close();
  if (_isDeleteFileOnClosing)
    if (std::remove(_targetFileName.c_str()) != NULL)
      FileContentChangerI::Errors.emplace("target file cannot be removing");
}

FileContentChangerI::FileSizeType StandardFileManager::GetFileSize() const
{
  // note: nc_this <=> non const this
  StandardFileManager* nc_this = const_cast<StandardFileManager*>(this);
  FileSizeType currentPosition = GetCoursePosition();
  nc_this->SetCourseToEnd();
  std::streampos lastPosition = GetCoursePosition();
  nc_this->SetCoursePosition(currentPosition);
  return static_cast<FileSizeType>(lastPosition);
}

void StandardFileManager::SetCourseToEnd()
{
  BaseFile::seekp(0, std::ios_base::end);
}

void StandardFileManager::SetCourseToBegin()
{
  BaseFile::seekp(0, std::ios_base::beg);
}

void StandardFileManager::SetCoursePosition(FileSizeType position)
{
  BaseFile::seekp(static_cast<std::streamoff>(position), std::ios_base::beg);
}

FileContentChangerI::FileSizeType StandardFileManager::GetCoursePosition() const
{
  // note: nc_this <=> non const this
  StandardFileManager* nc_this = const_cast<StandardFileManager*>(this);
  std::streampos currentPosition = nc_this->tellp();
  return static_cast<FileSizeType>(currentPosition);
}

void StandardFileManager::WriteData(const BufferType* buffer, FileSizeType size)
{
  BaseFile::write(buffer, static_cast<std::streamsize>(size));
}

std::vector<StandardFileManager::BufferType> StandardFileManager::ReadData(FileSizeType size)
{
  std::vector<BufferType> result;
  result.reserve(static_cast<size_t>(size));
  BaseFile::write(result.data(), static_cast<std::streamsize>(result.size()));
  return std::vector<BufferType>();
}
