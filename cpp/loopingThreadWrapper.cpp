#include "loopingThreadWrapper.hpp"


loopingThreadWrapper::loopingThreadWrapper()
: loopingThreadWrapper(250, utility::string_tools::random_string(8), nullptr)
{
}


loopingThreadWrapper::loopingThreadWrapper(int msleep, std::string name, std::shared_ptr<zyre::node_t> pNode)
: m_stopped(false), m_msleep(msleep), m_name(name), m_pNode(pNode)
{
    ++m_threadCount;
}


loopingThreadWrapper::~loopingThreadWrapper()
{
	--m_threadCount;
	m_stopped = true;
}


void loopingThreadWrapper::start()
{
    //t = std::thread(&loopingThreadWrapper::execute,this);
    m_pThread = std::unique_ptr<std::thread>(new std::thread(&loopingThreadWrapper::loop,this));
}


void loopingThreadWrapper::stop()
{
	m_stopped = true;
    std::cout << "Thread \"" << m_name << "\": join()'ing...!" << std::endl;
	m_pThread->join();
}


bool loopingThreadWrapper::isStopped()
{
	return m_stopped;
}


void loopingThreadWrapper::loop() 
{
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;  
    while(1) 
    {
        t0 = utility::time_tools::now();
        if (this->isStopped()) break;
        execute();
        t1 = utility::time_tools::now();
        double dt_ = utility::time_tools::dt(t0, t1); // seconds
        double shortfall_to_deadline = m_msleep/1000. - dt_; // seconds        
        if (shortfall_to_deadline > 0)
        {
          //std::cout << "sleeping for " << int(shortfall_to_deadline*1000) << " milliseconds" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(int(shortfall_to_deadline*1000)));
        }
        else 
        {
          //std::cout << "WARNING: behind by " << int(-shortfall_to_deadline*1000) << " milliseconds" << std::endl;
        }       
    }
}

//void loopingThreadWrapper::execute(){/* virtual */}


void loopingThreadWrapper::execute()
{
    //std::cout << "Thread \"" << m_name << "\": listening for zmsg's..." << std::endl;
    int msec_deadline = 1000;
    zyre::zmsg msg(m_pNode->recv_deadline(msec_deadline));
    if (!msg.is_zmsg())
    {
        //std::cout << "recv_deadline did not return a zmsg..." << std::endl;        
        if (m_pNode->poller_expired())
        {
            std::cout << "Deadline of " << msec_deadline << " msec expired w/o a zmsg..." << std::endl;
        }
        return;
    }
    std::cout << "Thread \"" << m_name << "\": received a zmsg!" << std::endl;
    std::cout << "   zmsg.size() = " << msg.size() << std::endl;
    msg.print();
}

// static variables
int loopingThreadWrapper::m_threadCount = 0; //initialize the static variable
