#ifndef ROBOT_NODES__AREA_HPP_
#define ROBOT_NODES__AREA_HPP_

namespace robot_nodes
{

// 计算矩形面积。宽或高为负数时抛出 std::invalid_argument。
double rectangle_area(double width, double height);

}  // namespace robot_nodes

#endif  // ROBOT_NODES__AREA_HPP_
