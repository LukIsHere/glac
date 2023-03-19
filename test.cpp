#include <iostream>
#include "scf.hpp"


int main(){
    dsl::dataArray serce("serce.pbi");

    scf test(64,64,[&](scf& scf, ctx& ctx, int32_t x, int32_t y){
        ctx.drawSprite(0,0,serce,1);
        scf.move(x+10,y+10);
    });
    return 0;
}