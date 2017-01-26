#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include <random>
#include <functional>

namespace utility 
{
  inline double round_to_decimal(double input, int decimal_places)
  {
    return round(input*pow(10.0, decimal_places))/pow(10.0, decimal_places);
  }

  namespace time_tools 
  {
    inline std::chrono::time_point<std::chrono::high_resolution_clock> now()
    {
      return std::chrono::high_resolution_clock::now();
    }
    
    inline double dt(std::chrono::time_point<std::chrono::high_resolution_clock> t0, 
                     std::chrono::time_point<std::chrono::high_resolution_clock> t1)
    {
      std::chrono::duration<double> dt_ = std::chrono::duration_cast< std::chrono::duration<double> >(t1-t0);
      return dt_.count();
    }

    inline long long seconds_since_epoch ()
    {
      std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
      return std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count();
    }

    inline long milliseconds_since_epoch ()
    {
      std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
      return std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
    }
  }

  namespace random_numbers 
  {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<> distribution(0., 1.0);

    inline double rand()
    {
      return distribution(generator);
    }

    inline double rand(double min, double max)
    {
      return distribution(generator)*(max - min) + min;
    }

    inline int rand_int(int min, int max)
    {
      // NOTE: max is INCLUDED, hence the max + 1 on the next line
      return int(rand(double(min), double(max + 1)));
    }

    inline std::vector<double> rand(int size)
    {
      std::vector<double> result;
      for (int i = 0; i < size; i++)
      {
        result.push_back(distribution(generator));
      }
      return result;
    }

    inline std::vector<double> rand(int size, double min, double max)
    {
      std::vector<double> result;
      for (int i = 0; i < size; i++)
      {
        result.push_back(distribution(generator)*(max - min) + min);
      }
      return result;    
    }
  }

  namespace string_tools
  {
    inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) 
        {
            elems.push_back(item);
        }
        return elems;
    }

    inline std::vector<std::string> split(const std::string &s, char delim) 
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    inline void remove_quotes(std::string &s) 
    {
        if (s.substr(s.length()-1, 1).compare("\"") == 0) 
        {
            s.erase(s.length()-1, 1);
        }
        if (s.substr(0, 1).compare("\"") == 0) 
        {
            s.erase(0, 1);
        }    
    }

    inline std::string random_string(int len)
    {
      std::string s;
      s.resize(len);
      static const char alphanum[] =
          "0123456789"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "abcdefghijklmnopqrstuvwxyz";

      for (int i = 0; i < len; ++i) {
          s.at(i) = alphanum[utility::random_numbers::rand_int(0, sizeof(alphanum) - 1)];
      }

      return s;
    }

    inline std::string dvector_to_string(std::vector<double> v, int precision)
    {
      if (v.size() == 0)
      {
        return std::string("");
      }
      std::ostringstream ss;
      ss << "[  ";
      for (int i = 0; i < v.size(); i++)
      {
        ss << utility::round_to_decimal(v.at(i), precision) << "  ";
      }
      ss << "]";

      return ss.str();
    }

    inline std::string svector_to_string(std::vector<std::string> v, std::string title)
    {
      std::ostringstream ss;
      ss << title << ":\n";
      if (v.size() == 0)
      {
        return ss.str();
      }      
      for (int i = 0; i < v.size(); i++)
      {
        ss << "    " << v.at(i) << "\n";
      }

      return ss.str();
    }
  }  

}

#endif // UTILITY_H
