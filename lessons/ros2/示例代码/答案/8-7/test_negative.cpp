#include <gtest/gtest.h>

#include <stdexcept>

#include "robot_nodes/area.hpp"

TEST(AreaTest, NegativeSideThrows)
{
  EXPECT_THROW(robot_nodes::rectangle_area(-1.0, 2.0), std::invalid_argument);
}
