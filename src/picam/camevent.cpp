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

#include <iostream>

#include "httppp/http.h"

#include "camevent.h"
#include "recordcam.h"


pidoorkeepder::CamEvent::CamEvent(libhttppp::ServerSocket* serversocket) : libhttppp::Queue(serversocket){

}

pidoorkeepder::CamEvent::~CamEvent(){

}

void pidoorkeepder::CamEvent::RequestEvent(libhttppp::Connection* curcon){
//   std::stringstream idxstream;
  libhttppp::HttpResponse curres;
  curres.setState(HTTP200);
  curres.setVersion(HTTPVERSION(1.1));
  curres.setContentType("video/mp4");
  curres.send(curcon,NULL,-1);
}

void pidoorkeepder::CamEvent::ResponseEvent(libhttppp::Connection* curcon){
  char *buf;
  RecordCamera rcam;
  ssize_t buflen= rcam.getCameraBuffer(&buf);
  curcon->addSendQueue(buf,buflen);
  delete[] buf;
}

void pidoorkeepder::CamEvent::ConnectEvent(libhttppp::Connection* curcon){
}

void pidoorkeepder::CamEvent::DisconnectEvent(libhttppp::Connection* curcon){
}


