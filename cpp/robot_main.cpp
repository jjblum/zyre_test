#include <thread>
#include <chrono>

#include "robot.hpp"



int main(int argc, char **argv)
{

  Robot robot_1 = Robot();
  Robot robot_2 = Robot("Chad the party node", 100, true);

  robot_1.start();
  robot_2.start();

  robot_1.join_group("HEYGUY");
  robot_2.join_group("HEYGUY");

  // idle/work for X seconds
  std::this_thread::sleep_for (std::chrono::milliseconds(1000));
  robot_2.list_peers();
  


  robot_1.stop();
  robot_2.stop();

  return 0;
}