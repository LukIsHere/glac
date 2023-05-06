#include "glac.hpp"
#include "scfLin.hpp"
#include "terminal.hpp"

int main(){
    terminal t(10,10,6);
    t.print("Helo wrld");
    scf test(360,360,[&] (scf& s,ctx8888& ctx,int32_t x,int32_t y){
        ctx.drawCtx(0,0,t.draw());
    });
}