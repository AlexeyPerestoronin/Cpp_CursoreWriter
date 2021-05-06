#include <gtest/gtest.h>

#include "StandardFileManager.hpp"

TEST(StandardFileManager, ConstructorOptions) {
  std::string fileName = "test.txt";

  // try remove file before deal with it
  std::remove(fileName.c_str());

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
