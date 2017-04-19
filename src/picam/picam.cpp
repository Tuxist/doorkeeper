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
#include <thread>

#include <httppp/config.h>
#include <httppp/httpd.h>
#include <httppp/event.h>

#include "camevent.h"
#include "recordcam.h"

namespace pidoorkeepder {
  class PICamD : public libhttppp::HttpD {
  public:
    PICamD(int argc, char** argv) : libhttppp::HttpD(argc,argv){
      libhttppp::HTTPException httpexception;
      try {
        CamEvent eventd(getServerSocket());
	eventd.runEventloop();
      }catch(libhttppp::HTTPException &e){
        std::cerr << e.what() << "\n";
      }
    };
  private:
  };
}

int main(int argc, char** argv){
  std::thread t1=std::thread([]{
    pidoorkeepder::RecordCamera::startRecording();
  });
  pidoorkeepder::PICamD(argc,argv);
  pidoorkeepder::RecordCamera::stopRecording();
  t1.join();
}
