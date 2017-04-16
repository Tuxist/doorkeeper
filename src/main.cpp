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

#include <iostream>
#include <thread>
#include <mutex>

#include <httppp/config.h>
#include <httppp/httpd.h>
#include <httppp/event.h>

#include "controller.h"
#include "modules.h"

namespace pidoorkeepder {
  
  class ProccessManager {
  public:
    class Process {
    public:
      Process(libhttppp::Connection *connection){
        _Connection=connection;  
        _nextProcess=NULL;
        Thread=NULL;
        Mutex=new std::mutex;
      };
      
      ~Process(){
        delete Mutex;
        delete _nextProcess;  
      };
      
      Process *nextProcess(){
        return _nextProcess;
      };
      
      libhttppp::Connection *getConnection(){
        return _Connection;
      };
      
      std::thread           *Thread;
      std::mutex            *Mutex;
      
    private:
      libhttppp::Connection *_Connection;

      Process               *_nextProcess;
      friend class           ProccessManager;
    };
    
    ProccessManager(){
      _firstProcess=NULL;
      _lastProcess=NULL;
    };
    
    Process *getProcess(libhttppp::Connection *connection){
      for(Process *curproc=_firstProcess; curproc; curproc=curproc->nextProcess()){
         if(connection==curproc->getConnection()){
            return curproc;
         }
      }
      return NULL;
    };
    
    Process *addProcess(libhttppp::Connection *connection){
      Process *checkprc=getProcess(connection);
      if(checkprc!=NULL)
        return checkprc;
      if(_firstProcess==NULL){
        _firstProcess=new Process(connection);
        _lastProcess=_firstProcess;
      }else{
        _lastProcess->_nextProcess=new Process(connection);
        _lastProcess=_lastProcess->_nextProcess;
      }
      return _lastProcess;
    };
    
    void delProcess(libhttppp::Connection *connection){
      Process *prevproc=NULL;
      for(Process *curproc=_firstProcess; curproc; curproc=curproc->nextProcess()){
        if(curproc->getConnection()==connection){
          if(prevproc){
            prevproc->_nextProcess=curproc->_nextProcess;
            if(_lastProcess==curproc)
              _lastProcess=prevproc;
          }else{
            _firstProcess=curproc->_nextProcess;
            if(_lastProcess==curproc)
              _lastProcess=_firstProcess;
          }
          curproc->_nextProcess=NULL;
          delete curproc;
          break;
        }
        prevproc=curproc;
      }
    };
    
  private:
     Process *_firstProcess;
     Process *_lastProcess;
  };
  
  class EventD : public libhttppp::Queue{
  public:
    EventD(libhttppp::ServerSocket* serversocket) : libhttppp::Queue(serversocket){
      _Modules = new Modules(MODULPATH);
      _ProccessManager = new ProccessManager();
    };
    
    ~EventD(){
      delete _ProccessManager;
      delete _Modules;
    };
    
    void RequestEvent(libhttppp::Connection *curcon){
      try{
        ProccessManager::Process *curproz=_ProccessManager->addProcess(curcon);
        if(curproz->Mutex->try_lock()){
          curproz->Thread = new std::thread ([=]{
            curproz->Mutex->lock();
            Controller(curcon,_Modules);
          });
          curproz->Thread->detach();
        };
      }catch(libhttppp::HTTPException &e){
        std::cerr << e.what() << "\n";
        throw e;
      }
    };
    
    void DisconnectEvent(libhttppp::Connection *curcon){
      _ProccessManager->delProcess(curcon);
    };
   
  private:
    ProccessManager *_ProccessManager;
    Modules    *_Modules;
  };
  
  class HttpPiDoorkeeperD : public libhttppp::HttpD {
  public:
    HttpPiDoorkeeperD(int argc, char** argv) : libhttppp::HttpD(argc,argv){
      libhttppp::HTTPException httpexception;
      try {
        EventD eventd(getServerSocket());
        eventd.runEventloop();
      }catch(libhttppp::HTTPException &e){
        std::cerr << e.what() << "\n";
      }
    };
  private:
  };
}

int main(int argc, char** argv){
  pidoorkeepder::HttpPiDoorkeeperD(argc,argv);
} 
