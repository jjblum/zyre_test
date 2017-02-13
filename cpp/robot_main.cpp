#include <thread>
#include <chrono>

#include "robot.hpp"
#include "utility.h"

#define DEFAULT_RUNTIME_S  5.0
#define DEFAULT_RUNWAIT_MS  200

int main(int argc, char **argv)
{

  Robot robot = Robot();
  robot.start();

  robot.join_group("Robot Lottery News");

  robot.print();

  double runtime_s;
  int runwait_ms;
  double dt = 0.;
  runtime_s = (argc > 1 ? atof(argv[1]) : DEFAULT_RUNTIME_S);
  runwait_ms = (argc > 2 ? atoi(argv[2]) : DEFAULT_RUNWAIT_MS);
  std::chrono::time_point<std::chrono::high_resolution_clock> t0 = utility::time_tools::now();
  std::chrono::time_point<std::chrono::high_resolution_clock> t1 = utility::time_tools::now();

  //std::cout << "runtime_s = " << runtime_s << std::endl;
  while (dt < runtime_s)
  {

    //std::cout << "t = " << dt << " s,  time remaining = " << runtime_s - dt << std::endl;

    if (utility::random_numbers::rand() > 0.9)
    {
      std::cout << "won the lottery, shout the good news!" << std::endl;
      robot.simple_shout();
    }

    // TODO: whisper congratulations back to any peer that wins the lottery
    
    std::this_thread::sleep_for (std::chrono::milliseconds(runwait_ms));
    t1 = utility::time_tools::now();
    dt = utility::time_tools::dt(t0, t1);
  }  
  
  robot.stop();
  std::this_thread::sleep_for (std::chrono::milliseconds(1000)); // wait a second to make sure everything wraps up

  return 0;
}