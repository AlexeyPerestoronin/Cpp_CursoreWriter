#include "StandardFileManager.hpp"

#include <type_traits>

StandardFileManager::StandardFileManager(const std::string& targetFileName)
  : _targetFileName(targetFileName)
  , BaseFile(_targetFileName, BaseFile::binary | BaseFile::in | BaseFile::out)
{
}

StandardFileManager::~StandardFileManager()
{
  BaseFile::flush();
  BaseFile::close();
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
