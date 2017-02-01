#include "robot.hpp"


Robot::Robot()
: Robot(utility::string_tools::random_string(8), 1000, false)
{
}


Robot::Robot(std::string name, int discovery_interval_msec, bool verbose)
: m_name(name)
{
  m_pNode = std::shared_ptr<zyre::node_t>(new zyre::node_t(name));
  if (verbose) m_pNode->set_verbose();
  m_pNode->set_interval(discovery_interval_msec);

  m_pRecvThread = std::unique_ptr<loopingThreadWrapper>(new loopingThreadWrapper(0, "zyre_recv", m_pNode));

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
  m_pRecvThread->start();
}


void Robot::stop()
{
  m_pNode->stop();
  m_pRecvThread->stop();
}


void Robot::join_group(std::string group)
{
  m_pNode->join(group);
}


void Robot::list_peers()
{
  std::vector<std::string> peer_list = m_pNode->peers(); // TODO: use node names rather than peer hashes
  std::cout << utility::string_tools::svector_to_string(peer_list, "PEERS") << std::endl;
}


void Robot::simple_shout()
{
  // shout to group 0
  std::vector<std::string> groups = m_pNode->own_groups();
  if (groups.size() < 1)
  {
    std::cout << "No groups to shout to. So lonely...";
    return;
  }
  std::cout << "Shouting to group " << groups.at(0) << std::endl;

  
  // construct a basic message
  zyre::zmsg hey;
  hey.add_str("LOUD NOISES!!!");

  // shout the message
  if (m_pNode)
  {   
    shout(groups.at(0), hey);   
  }
}


void Robot::shout(const std::string& group, zyre::zmsg &msg)
{
  m_pNode->shout(group, msg.msg_ptr());
  msg.nullify(); // required because zyre destroys the message!
}