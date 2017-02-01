#ifndef LOOPING_THREAD_WRAPPER_H
#define LOOPING_THREAD_WRAPPER_H

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <memory>

#include "utility.h"
#include "zyrecpp.hpp"

class loopingThreadWrapper 
{
public:
	loopingThreadWrapper();
  loopingThreadWrapper(int msleep, std::string name, std::shared_ptr<zyre::node_t> pNode);
	~loopingThreadWrapper();
	static int m_threadCount;
	void stop();
	void start();
	bool isStopped();
	void loop();
	// virtual void execute();  

protected:
  void execute();
	bool m_stopped;
	std::string m_name;
  std::unique_ptr<std::thread> m_pThread = nullptr;
  std::shared_ptr<zyre::node_t> m_pNode = nullptr; // must be a shared_ptr, because a unique_ptr is MOVE ONLY, it can't be copied
  int m_msleep; // number of milliseconds slept every iteration in execute
  
};

// IMPORTANT: THIS TYPE OF THREAD IS ONLY APPROPRIATE FOR THREADS THAT ARE
// 			  MEANT TO LOOP CONTINUOUSLY UNTIL SOME CONDITION IS MET

#endif //LOOPING_THREAD_WRAPPER_H
