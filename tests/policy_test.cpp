#include <cassert>
#include "../firmware/room-climate-hub-energy-saver/policy.h"
int main(){
 EnergyPolicy p;p.update(false,100);assert(p.doorOpen);p.update(false,149);assert(p.doorOpen);
 p.update(false,150);assert(!p.doorOpen&&p.displayOn);p.update(false,15150);assert(!p.displayOn);
 p.update(true,16000);p.update(false,16020);p.update(false,16100);assert(!p.doorOpen&&!p.displayOn);
 p.update(true,17000);p.update(true,17050);assert(p.doorOpen&&p.displayOn);
 p.update(false,18000);p.update(false,18050);p.update(false,40000);assert(!p.displayOn);
 assert(p.command("WAKE",40000));p.update(false,69999);assert(p.displayOn);
 p.update(false,70000);assert(!p.displayOn);assert(!p.command("INVALID",70000));
 EnergyPolicy wrap;wrap.doorOpen=false;wrap.rawOpen=false;wrap.closedAt=0xfffffff0u;
 wrap.update(false,0x20u);assert(wrap.displayOn);wrap.update(false,15000u);assert(!wrap.displayOn);
 LineParser l;for(char c: "STATUS"){if(c)assert(l.feed(c)==0);}assert(l.feed('\n')==1);assert(!strcmp(l.buf,"STATUS"));
 for(int i=0;i<17;i++){l.feed('X');}assert(l.feed('\n')==-1);l.feed('W');assert(l.feed('\r')==0);assert(l.feed('\n')==1);assert(!strcmp(l.buf,"W"));
}
