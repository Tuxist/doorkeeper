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
#include <iostream>
#include <sstream>
#include <sys/wait.h>

#include <httppp/config.h>
#include <httppp/connections.h>
#include <httppp/http.h>
#include "../modules.h"

namespace pidoorkeepder {
  class RecordCamera {
  public:
    void startRecording(libhttppp::Connection *curcon){
      char buf[BLOCKSIZE];
      int fd[2], len = 0;

      if(pipe(fd) < 0){
        std::cerr << "Cannot create pipe\n";
        return;
      }
      
      _Pid = fork();
      if(_Pid < 0){ /* failure */
        std::cerr << "forkfailed\n";
        return;
      }
      if(_Pid > 0) { /* parent */
        close(fd[1]);
        while((len = read(fd[0], &buf, BLOCKSIZE)) > 0) {
          curcon->addSendQueue((const char*)&buf,BLOCKSIZE);
        }
      } else { /* child */
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        close(fd[1]);
	if(execle("/bin/cat","cat","/dev/urandom",NULL)<0)
	  perror("exec");
//         if(execle("raspivid","raspivid","-t 10 -o -",NULL) < 0)
//           perror("exec");
        quick_exit(EXIT_SUCCESS);
      }
      int status;
      waitpid(-1, &status, 0);
    };
    
    void stopRecording(){
      kill(_Pid, SIGTERM);
    };
    
  private:
    pid_t _Pid;
  };

  class Camera : public pidoorkeepder::ModuleAPI {
  public:
    virtual const char *getName(){
      return "CameraController";
    }

    virtual const char *getControllerPath(){
      return "/pidoorkeepder/camera.mp4";
    }

    virtual void runModul(libhttppp::Connection *curcon,libhttppp::HttpRequest *cureq){
      std::stringstream idxstream;
      libhttppp::HttpResponse curres;
      curres.setState(HTTP200);
      curres.setVersion(HTTPVERSION(1.1));
      curres.setContentType("video/mp4");
      curres.send(curcon,NULL,-1);
      recordcam.startRecording(curcon);
    }

    virtual void stopModul(libhttppp::Connection *curcon,libhttppp::HttpRequest *cureq){
      recordcam.stopRecording();
    }
    
    virtual ~Camera(){
    }
  private:
    RecordCamera recordcam;
};

// the class factories
extern "C" ModuleAPI* create() {
    return new Camera;
}

extern "C" void destroy(ModuleAPI* p) {
    delete p;
}
}; 
