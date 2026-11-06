#include <gtest/gtest.h>

#include "robot_nodes/area.hpp"

TEST(AreaTest, ComputesProduct)
{
  EXPECT_DOUBLE_EQ(robot_nodes::rectangle_area(2.0, 3.0), 6.0);
}

TEST(AreaTest, ZeroSideGivesZero)
{
  EXPECT_DOUBLE_EQ(robot_nodes::rectangle_area(0.0, 5.0), 0.0);
}
