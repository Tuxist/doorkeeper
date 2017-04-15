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

#include <httppp/config.h>
#include <httppp/httpd.h>
#include <httppp/event.h>

#include "controller.h"
#include "modules.h"

namespace pidoorkeepder {
  class EventD : public libhttppp::Queue{
  public:
    EventD(libhttppp::ServerSocket* serversocket) : libhttppp::Queue(serversocket){
      _Modules = new Modules(MODULPATH);
      _Controller = NULL;
    };
    
    ~EventD(){
      delete _Modules;
      delete _Controller;
    };
    
    void RequestEvent(libhttppp::Connection *curcon){
      try{
        _Controller = new Controller(curcon,_Modules);
      }catch(libhttppp::HTTPException &e){
        std::cerr << e.what() << "\n";
        throw e;
      }
    };
    
    void DisconnectEvent(libhttppp::Connection *curcon){
       delete _Controller; 
    };
   
  private:
    Controller *_Controller;
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
