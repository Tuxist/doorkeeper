 
/**********************************************************************
pidoorkeepder is program to make a rasperry to a doorstation

Copyright (C) 2017  <Jan Koester> jan.koester@gmx.net

This file is part of pidoorkeepder.

pidoorkeepder is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

pidoorkeepder is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with pidoorkeepder.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <httppp/config.h>
#include <httppp/httpd.h>
#include <httppp/event.h>
#include "httppp/http.h"

#ifndef CAMEVENT_H
#define CAMEVENT_H

namespace pidoorkeepder {
  class CamEvent : public libhttppp::Queue{
  public:
    CamEvent(libhttppp::ServerSocket* serversocket);
    ~CamEvent();
    virtual void RequestEvent(libhttppp::Connection* curcon);
    virtual void ResponseEvent(libhttppp::Connection* curcon);
    virtual void ConnectEvent(libhttppp::Connection* curcon);
    virtual void DisconnectEvent(libhttppp::Connection* curcon);
  private:
    libhttppp::HttpResponse _CurrentHttpResponse;
  };
};

#endif
