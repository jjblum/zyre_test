#include "robot.hpp"

Robot::Robot()
: Robot(utility::string_tools::random_string(8))
{
}

Robot::Robot(std::string name)
: m_name(name)
{
  m_pNode = std::unique_ptr<zyre::node_t>(new zyre::node_t(name));
  m_state = utility::random_numbers::rand(ROBOT_STATE_SIZE, 0., 1.);
}

void Robot::print()
{
  m_pNode->print();
  std::cout << "STATE = " << utility::string_tools::vector_to_string(m_state, 3) << std::endl;
}

