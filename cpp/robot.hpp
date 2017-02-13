#ifndef _ZYRE_ROBOT_H_
#define _ZYRE_ROBOT_H_

#include <string>
#include <memory>
#include <iostream>
#include "nlohmann/json.hpp"
#include "zyrecpp.hpp"

#include "loopingThreadWrapper.hpp"
#include "utility.h"

#include "msg_schema_generated.h"


#define ROBOT_STATE_SIZE 3


class Robot
{
public:
  Robot();
  Robot(std::string name, int discovery_interval_msec = 100, bool verbose = false);
  void print();  
  void start();
  void stop();
  void join_group(std::string group);
  void list_peers();
  void simple_shout();
  void shout(const std::string& group, zyre::zmsg &msg);

private:
  std::shared_ptr<zyre::node_t> m_pNode = nullptr; // must be a shared_ptr, because a unique_ptr is MOVE ONLY, it can't be copied
  std::unique_ptr<loopingThreadWrapper> m_pRecvThread = nullptr;
  std::string m_name;  
  std::vector<double> m_state;  
};

#endif // _ZYRE_ROBOT_H_