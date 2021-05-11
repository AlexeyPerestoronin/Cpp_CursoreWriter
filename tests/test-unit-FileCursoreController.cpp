#include <gtest/gtest.h>

#include "StandardFileManager.hpp"
#include "FileCursoresController.hpp"


struct FileCursoresControllerTest : public testing::Test
{
  const std::string fileName = "test.txt";
  std::string fileContent = "-/content\-";
  FileContentChangerI::ShaPtr fileContentChanger;

  void SetUp() override
  {
    // try remove file before deal with it
    std::remove(fileName.c_str());
    fileContentChanger = std::make_shared<StandardFileManager>(fileName, true, false);
  }
};



TEST_F(FileCursoresControllerTest, ConstructorTest)
{
  {
    //FileCursor<int>::ShaPtr fc;
    FileCursoresController<int>::CursorClass::ShaPtr fc2;
    FileCursoresController<int> fcc(fileContentChanger);
    auto fc = fcc.AddCursore(1, { 0, 0, 0 });
    fc.lock()->WriteData(fileContent.data(), fileContent.size());
  }
}