 
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
#include <mutex>

#ifndef RECORDCAM_H
#define RECORDCAM_H
namespace pidoorkeepder {
  class RecordCamera {
  public:
    RecordCamera();
    
    static void startRecording();
    static void stopRecording();
    
    ssize_t getCameraBuffer(char **Buffer);

    //static vars
    static pid_t      Pid;
    static std::mutex lockBuf;
    static char       Buffer[BLOCKSIZE];
    static ssize_t    BufferSize;     
  private:
  };
};
#endif
