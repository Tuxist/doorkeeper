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

#include <httppp/http.h>
#include <httppp/connections.h>
#include "modules.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace pidoorkeepder {
  class Controller {
  public:
    class ControllerInstance{
      
    private:
      ControllerInstance(libhttppp::Connection*,ModuleAPI *mapi);
      ~ControllerInstance();
      ModuleAPI              *_MAPI;
      libhttppp::Connection  *_Connection;
      ControllerInstance     *_nextControllerInstance;
      friend class Controller;
    };
    Controller(libhttppp::Connection* curcon,Modules* modules);
    ~Controller();
  private:
    void addControllerInstance(libhttppp::Connection*,ModuleAPI *mapi);
    void deleteControllerInstance(libhttppp::Connection);
  };
};

#endif
