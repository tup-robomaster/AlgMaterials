#include "robot_nodes/area.hpp"

#include <stdexcept>

namespace robot_nodes
{

double rectangle_area(double width, double height)
{
  if (width < 0.0 || height < 0.0) {
    throw std::invalid_argument("width and height must be non-negative");
  }
  return width * height;
}

}  // namespace robot_nodes
