#pragma once
#include <stdint.h>
#include <string.h>
struct EnergyPolicy {
  bool doorOpen=true, rawOpen=true, displayOn=true, overrideActive=false;
  uint32_t changedAt=0, closedAt=0, wakeAt=0;
  static constexpr uint32_t debounceMs=50, idleMs=15000, wakeMs=30000;
  void update(bool open,uint32_t now) {
    if(open!=rawOpen){rawOpen=open;changedAt=now;}
    if(rawOpen!=doorOpen && uint32_t(now-changedAt)>=debounceMs){
      doorOpen=rawOpen;if(!doorOpen)closedAt=now;
    }
    if(overrideActive && uint32_t(now-wakeAt)>=wakeMs)overrideActive=false;
    displayOn=doorOpen||overrideActive||uint32_t(now-closedAt)<idleMs;
  }
  bool command(const char* cmd,uint32_t now){
    if(strcmp(cmd,"STATUS")==0)return true;
    if(strcmp(cmd,"WAKE")==0){overrideActive=true;wakeAt=now;displayOn=true;return true;}
    return false;
  }
};
struct LineParser {
 char buf[17]{};uint8_t count=0;bool overflow=false;
 // Returns 1 for complete command, -1 for rejected overlong line, 0 otherwise.
 int feed(char c){
   if(c=='\r')return 0;
   if(c=='\n'){buf[count]=0;int result=overflow?-1:1;count=0;overflow=false;return result;}
   if(count<16&&!overflow)buf[count++]=c;else overflow=true;
   return 0;
 }
};
