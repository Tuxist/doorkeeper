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
#include <sstream>
#include <httppp/http.h>
#include <dlfcn.h>
#include <dirent.h>

#include "modules.h"

pidoorkeepder::ModuleAPI::~ModuleAPI(){
}


const char *pidoorkeepder::ModuleAPI::getName(){
  return NULL;  
}

const char *pidoorkeepder::ModuleAPI::getControllerPath(){
  return NULL;  
}

void pidoorkeepder::ModuleAPI::runModul(libhttppp::Connection* curcon,libhttppp::HttpRequest *cureq){
  return;
}

void pidoorkeepder::ModuleAPI::stopModul(libhttppp::Connection* curcon,libhttppp::HttpRequest *cureq){
  return;
}

pidoorkeepder::Module::Module(){
  _nextModul=NULL;
}

pidoorkeepder::Module::~Module(){
  delete _nextModul;
}

pidoorkeepder::Module * pidoorkeepder::Module::nextModul(){
  return _nextModul;
}

pidoorkeepder::ModuleAPI *pidoorkeepder::Module::createSymbols(){
  create_t* create_modul = (create_t*) dlsym(_ModuleAPI, "create");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    return NULL;
  }
  ModuleAPI* modulsym = create_modul();
  return modulsym;    
}


void pidoorkeepder::Module::deleteSymbols(pidoorkeepder::ModuleAPI *delapi){
  destroy_t* destroy_module = (destroy_t*) dlsym(_ModuleAPI, "destroy");
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
    return;
  }
  destroy_module(delapi); 
}


pidoorkeepder::Modules::Modules(const char *path){
    /*load modules from patch */
   _firstModule=NULL;
   _lastModule=NULL;
   DIR *dpdf;
   struct dirent *epdf;
   dpdf = opendir(path);
   if (dpdf != NULL){
     while ((epdf = readdir(dpdf))){
        std::string mpath=path;
        mpath+=epdf->d_name;
        if(epdf->d_name[0]!='.'){
          printf("Load Modul: %s \n",mpath.c_str());
          loadModul(mpath.c_str());
        }
     }
   }
   closedir(dpdf);
}

pidoorkeepder::Modules::~Modules(){
  for(Module *curmod=getfirstModule(); curmod; curmod=curmod->nextModul()){
    dlclose(curmod->_ModuleAPI);
  }
  delete _firstModule;
  _lastModule=NULL;
}

pidoorkeepder::Module *pidoorkeepder::Modules::getfirstModule(){
  return _firstModule;  
}

void pidoorkeepder::Modules::loadModul(const char *path){
    void *modul = dlopen(path, RTLD_LAZY);
    if (!modul) {
        std::cerr << "Cannot load library: " << dlerror() << '\n';
        return;
    }
    
    dlerror();
    
    if(!_firstModule){
      _firstModule= new Module;
      _lastModule=_firstModule;
    }else{
      _lastModule->_nextModul=new Module;
      _lastModule=_lastModule->_nextModul;
    }
    if(_lastModule)
      _lastModule->_ModuleAPI=modul;
}
