#include <gtest/gtest.h>

#include "StandardFileManager.hpp"

struct StandardFileManagerTest : public testing::Test
{
  const std::string fileName = "test.txt";
  std::string fileContent = "-/content\-";

  void SetUp() override
  {
    // try remove file before deal with it
    std::remove(fileName.c_str());
  }
};


TEST_F(StandardFileManagerTest, CheckConstructorOptions)
{
  // note: cannot open unexistant file
  {
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    StandardFileManager sfm(fileName, /*isCreateFileIfNotExist*/ false);
    ASSERT_EQ(FileContentChangerI::Errors.size(), 1);
    FileContentChangerI::Errors.pop();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }

  // note: open unexistant file and not delete it after closing
  {
    StandardFileManager sfm(fileName);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }

  // note: can open existant file
  {
    StandardFileManager sfm(fileName, /*isCreateFileIfNotExist*/ false);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }

  // note: can open existant file and delete it after closing
  {
    StandardFileManager sfm(fileName, /*isCreateFileIfNotExist*/ false, /*isDeleteFileOnClosing*/ true);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }

  // note: cannot open unexistant file
  {
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    StandardFileManager sfm(fileName, /*isCreateFileIfNotExist*/ false);
    ASSERT_EQ(FileContentChangerI::Errors.size(), 1);
    FileContentChangerI::Errors.pop();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }
}


TEST_F(StandardFileManagerTest, CheckWriteAndRead)
{
  // write data to empty file
  {
    StandardFileManager sfm(fileName, true);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetFileSize(), 0);
    // write first portion of data
    sfm.WriteData(fileContent.data(), fileContent.size());
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size());
    ASSERT_EQ(sfm.GetFileSize(), fileContent.size());
    // write second portion of data
    sfm.WriteData(fileContent.data(), fileContent.size());
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    ASSERT_EQ(sfm.GetFileSize(), fileContent.size() * 2);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
  }

  // read data from not empty file:
  {
    // open file and set cursore position
    StandardFileManager sfm(fileName);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetFileSize(), fileContent.size() * 2);
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    sfm.SetCursorToBegin();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), 0);
    // read first portion of data
    std::vector<FileContentChangerI::BufferType> buffer = sfm.ReadData(fileContent.size());
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(buffer.size(), fileContent.size());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size());
    // read second portion of data
    buffer = sfm.ReadData(fileContent.size());
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(buffer.size(), fileContent.size());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    // modifing cursore position
    sfm.SetCursorToEnd();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    sfm.SetCursorToBegin();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), 0);
  }
  
  // read data from not empty file: more than there is in the file and not from begin cursor position
  {
    // open file and set cursore position
    StandardFileManager sfm(fileName);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetFileSize(), fileContent.size() * 2);
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    sfm.SetCursorPosition(10);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), 10);
    // read data
    std::vector<FileContentChangerI::BufferType> buffer = sfm.ReadData(fileContent.size() * 3);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(buffer.size(), fileContent.size());
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
  }
  
  // read data from not empty file: more than there is in the file and from begin cursor position
  {
    // open file and set cursore position
    StandardFileManager sfm(fileName);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetFileSize(), fileContent.size() * 2);
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
    sfm.SetCursorToBegin();
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(sfm.GetCursorPosition(), 0);
    // read data
    std::vector<FileContentChangerI::BufferType> buffer = sfm.ReadData(fileContent.size() * 3);
    ASSERT_TRUE(FileContentChangerI::Errors.empty());
    ASSERT_EQ(buffer.size(), fileContent.size() * 2);
    ASSERT_EQ(sfm.GetCursorPosition(), fileContent.size() * 2);
  }
}
