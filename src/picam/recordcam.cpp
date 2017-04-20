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
#include <unistd.h>
#include <sys/wait.h>

#include "recordcam.h"
#include "camevent.h"


pid_t pidoorkeepder::RecordCamera::Pid=0;
std::mutex pidoorkeepder::RecordCamera::lockBuf;
char pidoorkeepder::RecordCamera::Buffer[BLOCKSIZE]={0};
ssize_t pidoorkeepder::RecordCamera::BufferSize=0;

pidoorkeepder::RecordCamera::RecordCamera(){
};
    
void pidoorkeepder::RecordCamera::startRecording(){
      int fd[2];
      if(pipe(fd) < 0){
        std::cerr << "Cannot create pipe\n";
        return;
      }
      
      Pid = fork();
      if(Pid < 0){ /* failure */
        std::cerr << "forkfailed\n";
        return;
      }
      if(Pid > 0) { /* parent */
        close(fd[1]);
	char buf[BLOCKSIZE];
        int len = 0;
        while((len = read(fd[0], &buf, BLOCKSIZE)) > 0) {
	  if(lockBuf.try_lock()){
            std::copy(buf,buf+len,RecordCamera::Buffer);
	    RecordCamera::BufferSize=len;
	    RecordCamera::lockBuf.unlock();
	  }
        }
      } else { /* child */
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        close(fd[1]);
	if(execle("/bin/cat","cat","/dev/urandom",NULL)<0)
	  perror("exec");
//         if(execle("raspivid","raspivid","-t","10","-o","-",NULL,NULL) < 0)
//           perror("exec");
        quick_exit(EXIT_SUCCESS);
      }
      int status;
      waitpid(-1, &status, 0);
    };
    
void pidoorkeepder::RecordCamera::stopRecording(){
  kill(Pid, SIGTERM);
};

ssize_t pidoorkeepder::RecordCamera::getCameraBuffer(char **Buffer){
  while(!RecordCamera::lockBuf.try_lock()){};
  ssize_t len=RecordCamera::BufferSize;
  *Buffer=new char[len];
  std::copy(RecordCamera::Buffer,RecordCamera::Buffer+len,*Buffer);
  RecordCamera::lockBuf.unlock();
  return len;
}



