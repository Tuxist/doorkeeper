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


#include "controller.h"

pidoorkeepder::Controller::Controller(libhttppp::Connection* curcon,Modules *modules){
  bool found=false;
  libhttppp::HttpRequest curreq;
  curreq.parse(curcon);
  for(Module *curmod=modules->getfirstModule(); curmod; curmod=curmod->nextModul()){
    ModuleAPI *mapi = curmod->createSymbols();
    if(strncmp(mapi->getControllerPath(),curreq.getRequestURL(),strlen(curreq.getRequestURL()))==0){
       printf("Controller run Modul: %s \n",mapi->getName());
       mapi->runModul(curcon,&curreq);
       found=true;
       curmod->deleteSymbols(mapi);
       break;
    }
    curmod->deleteSymbols(mapi);
  }
  if(!found){
    printf("Rurl not unknown\n");
    libhttppp::HttpResponse curres;
    curres.setState(HTTP404);
    curres.setVersion(HTTPVERSION(1.1));
    curres.send(curcon,NULL,0);
  }
}

pidoorkeepder::Controller::~Controller(){
    
}
