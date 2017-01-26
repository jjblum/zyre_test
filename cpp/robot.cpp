#include "robot.hpp"

Robot::Robot()
: Robot(utility::string_tools::random_string(8))
{
}

Robot::Robot(std::string name)
: m_name(name)
{
  m_pNode = std::unique_ptr<zyre::node_t>(new zyre::node_t(name));
  m_state.resize(ROBOT_STATE_SIZE);
}

void Robot::print()
{
  m_pNode->print();
}

