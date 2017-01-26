#ifndef _ZYRE_ROBOT_H_
#define _ZYRE_ROBOT_H_

#include <string>
#include <memory>
#include <iostream>

#include "/home/jason/projects/zyrecpp/zyrecpp.hpp"

#include "utility.h"

#define ROBOT_STATE_SIZE 3

class Robot
{
public:
  Robot();
  Robot(std::string name, int discovery_interval_usec = 999, bool verbose = false);
  void print();  
  void start();
  void stop();
  void join_group(std::string group);
  void list_peers();

private:
  std::unique_ptr<zyre::node_t> m_pNode;
  std::string m_name;  
  std::vector<double> m_state;
};




#endif // _ZYRE_ROBOT_H_