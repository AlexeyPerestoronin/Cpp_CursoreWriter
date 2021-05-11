#include <gtest/gtest.h>

#include "FixedQueue.hpp"

TEST(FixedQueueTest, TestPush)
{
  FixedQueue<std::string, 3> fq;
  ASSERT_TRUE(fq.empty());
  fq.push("str1");
  fq.push("str2");
  const std::string str3{ "str3" };
  fq.push(str3);
  std::string str4{ "str4" };
  fq.push(std::move(str4));
  ASSERT_TRUE(fq.size(), 3);
  ASSERT_EQ(fq.back(), "str4");
  ASSERT_EQ(fq.front(), "str2");
}


TEST(FixedQueueTest, TestEmpalce)
{
  FixedQueue<std::string, 3> fq;
  ASSERT_TRUE(fq.empty());
  fq.emplace("str1");
  fq.emplace("str2");
  const std::string str3{ "str3" };
  fq.emplace(str3);
  std::string str4{ "str4" };
  fq.emplace(std::move(str4));
  ASSERT_TRUE(fq.size(), 3);
  ASSERT_EQ(fq.back(), "str4");
  ASSERT_EQ(fq.front(), "str2");
}
