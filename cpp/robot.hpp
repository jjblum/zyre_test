#ifndef _ZYRE_ROBOT_H_
#define _ZYRE_ROBOT_H_

#include <string>
#include <memory>
#include <iostream>

#include "zyrecpp.hpp"

#include "utility.h"

#define ROBOT_STATE_SIZE 3


class Robot
{
public:
  Robot();
  Robot(std::string name, int discovery_interval_msec = 999, bool verbose = false);
  void print();  
  void start();
  void stop();
  void join_group(std::string group);
  void list_peers();
  void simple_shout();
  void shout(const std::string& group, zyre::zmsg &msg);

private:
  std::unique_ptr<zyre::node_t> m_pNode = nullptr;
  std::string m_name;  
  std::vector<double> m_state;
};

#endif // _ZYRE_ROBOT_H_