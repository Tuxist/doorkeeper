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

#include "worker.h"
 
pidoorkeepder::ProccessManager::Process::Process(libhttppp::Connection* connection){
  _Connection=connection;  
  _nextProcess=NULL;
   Thread=NULL;
};
      
pidoorkeepder::ProccessManager::Process::~Process(){
  delete Thread;
  delete _nextProcess;  
};
      
pidoorkeepder::ProccessManager::Process 
*pidoorkeepder::ProccessManager::Process::nextProcess(){
  return _nextProcess;
};
      
libhttppp::Connection *pidoorkeepder::ProccessManager::Process::getConnection(){
  return _Connection;
};
    
pidoorkeepder::ProccessManager::ProccessManager(){
  _firstProcess=NULL;
  _lastProcess=NULL;
};

pidoorkeepder::ProccessManager::~ProccessManager(){
  delete _firstProcess;
  _lastProcess=NULL;
};

    
pidoorkeepder::ProccessManager::Process 
*pidoorkeepder::ProccessManager::getProcess(libhttppp::Connection *connection){
      for(Process *curproc=_firstProcess; curproc; curproc=curproc->nextProcess()){
         if(connection==curproc->getConnection()){
            return curproc;
         }
      }
      return NULL;
    };
    
pidoorkeepder::ProccessManager::Process 
*pidoorkeepder::ProccessManager::addProcess(libhttppp::Connection *connection){
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
    
void pidoorkeepder::ProccessManager::delProcess(libhttppp::Connection *connection){
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
    
