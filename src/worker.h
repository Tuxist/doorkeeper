 /**********************************************************************
pidoorkeepder is program to make a rasperry to a doorstation

Copyright (C) 2017  <Jan Koester> jan.koester@gmx.net

This file is part of Foobar.

Foobar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <thread>
#include <mutex>
 
#include <httppp/config.h>
#include <httppp/httpd.h>

#ifndef WORKER_H
#define WORKER_H

namespace pidoorkeepder { 
 class ProccessManager {
  public:
    class Process {
    public:
      Process *nextProcess();
      libhttppp::Connection *getConnection();
      std::thread           *Thread;
    private:
      Process(libhttppp::Connection *connection);
      ~Process();
      libhttppp::Connection *_Connection;
      Process               *_nextProcess;
      friend class           ProccessManager;
    };
    
    ProccessManager();
    ~ProccessManager();
    
    Process *getProcess(libhttppp::Connection *connection);
    Process *addProcess(libhttppp::Connection *connection); 
    void     delProcess(libhttppp::Connection *connection);
    
  private:
     Process *_firstProcess;
     Process *_lastProcess;
  };
};
#endif