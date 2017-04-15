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

#ifndef MODULES_H
#define MODULES_H

namespace pidoorkeepder {
  class ModuleAPI {
    public:
      virtual ~ModuleAPI();
      virtual const char *getName()=0;
      virtual const char *getControllerPath()=0;
      virtual void        runModul(libhttppp::Connection *curcon)=0;
  };
  
 typedef ModuleAPI* create_t();
 typedef void destroy_t(ModuleAPI*);
  
  class Module {
  public:
      ModuleAPI *createSymbols();
      void       deleteSymbols(ModuleAPI *delapi);
      Module    *nextModul();
      ~Module();
  private:
      Module();
      void   *_ModuleAPI;
      Module *_nextModul;
      friend class Modules;  
  };
 
  
  class Modules {
  public:
    Modules(const char *path);
    ~Modules();
    Module *getfirstModule();
  private:
    void   loadModul(const char *path);
    Module *_firstModule;
    Module *_lastModule;
  };
};

#endif
