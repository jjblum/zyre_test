#include "robot.hpp"


Robot::Robot()
: Robot(utility::string_tools::random_string(8))
{
}


Robot::Robot(std::string name, int discovery_interval_usec, bool verbose)
: m_name(name)
{
  m_pNode = std::unique_ptr<zyre::node_t>(new zyre::node_t(name));
  if (verbose) m_pNode->set_verbose();
  m_pNode->set_port(utility::random_numbers::rand_int(11400, 11499));
  m_pNode->set_interval(discovery_interval_usec);

  m_state = utility::random_numbers::rand(ROBOT_STATE_SIZE, 0., 1.);
}


void Robot::print()
{
  m_pNode->print();
  std::cout << "STATE = " << utility::string_tools::dvector_to_string(m_state, 3) << std::endl;
}

void Robot::start()
{
  m_pNode->start();
}

void Robot::stop()
{
  m_pNode->stop();
}

void Robot::join_group(std::string group)
{
  m_pNode->join(group);
}

void Robot::list_peers()
{
  std::vector<std::string> peer_list = m_pNode->peers();
  std::cout << utility::string_tools::svector_to_string(peer_list, "PEERS") << std::endl;
}