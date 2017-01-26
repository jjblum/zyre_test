#include "robot.hpp"

int main(int argc, char **argv)
{

  Robot robot_1 = Robot();
  Robot robot_2 = Robot("Chad the party node");

  robot_1.print();

  return 0;
}