#include <thread>
#include <chrono>

#include "robot.hpp"
#include "utility.h"

int main(int argc, char **argv)
{

  Robot robot = Robot();
  robot.start();

  robot.join_group("Robot Lottery News");

  for (int i = 0; i < 20; i++)
  {
    if (utility::random_numbers::rand() > 0.9)
    {
      std::cout << "won the lottery, shout the good news!" << std::endl;
      robot.simple_shout();
    }

    // TODO: whisper congratulations back to any peer that wins the lottery
    
    std::this_thread::sleep_for (std::chrono::milliseconds(200));
  }  
  
  robot.stop();
  std::this_thread::sleep_for (std::chrono::milliseconds(1000)); // wait a second to make sure everything wraps up

  return 0;
}