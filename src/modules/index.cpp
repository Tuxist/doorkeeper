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

#include <cstdio>
#include <sstream>
#include <httppp/config.h>
#include <httppp/connections.h>
#include <httppp/http.h>
#include "../modules.h"

namespace pidoorkeepder {
  class Index : public pidoorkeepder::ModuleAPI {
  public:
    virtual const char *getName(){
      return "IndexController";
    }
    
    virtual const char *getControllerPath(){
      return "/";
    }
    
    virtual void runModul(libhttppp::Connection *curcon,libhttppp::HttpRequest *cureq){
      std::stringstream idxstream;
      idxstream << "<!DOCTYPE HTML>"
                << " <html>"
                << "  <head>"
                << "    <title>PIDoorkeeper</title>"
                << "    <meta content=\"\">"
                << "    <meta charset=\"utf-8\">"
                << "    <style></style>"
                << "  </head>"
                << "<body>"
                << "<h1>PIDoorkeeper</h1>"
                << " <video width=\"640\" height=\"480\" controls>"
                << "<source src=\"/pidoorkeepder/camera.mp4\" type=\"video/mp4\">"
                << "Your browser does not support the video tag."
                << "</video>"
                << "</body></html>";
      std::string data = idxstream.str();
      libhttppp::HttpResponse curres;
      curres.setState(HTTP200);
      curres.setVersion(HTTPVERSION(1.1));
      curres.setContentType("text/html");
      curres.send(curcon,data.c_str(),data.length());
    }
    
    virtual ~Index(){
        
    }
    
};

// the class factories
extern "C" ModuleAPI* create() {
    return new Index;
}

extern "C" void destroy(ModuleAPI* p) {
    delete p;
}
};
