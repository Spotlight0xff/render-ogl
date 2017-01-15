#include "gtest/gtest.h"
#include "Util.h"

#include <string>
#include <vector>

// util::split
TEST(unit_test_util, split)
{
  std::string str = "a/b";
  std::vector<std::string> splits;
  util::split(str, '/', splits);


	EXPECT_EQ(splits[0], "a");
	EXPECT_EQ(splits[1], "b");
  EXPECT_EQ(splits.size(), 2);
}

TEST(unit_test_util, split2) {
  std::string str = "";
  std::vector<std::string> splits;
  util::split(str, '/', splits);

	EXPECT_EQ(splits.size(), 0);
}
