#include <iostream>
#include "scfLin.hpp"
#include <iostream>
#include <thread>
#include "mutex.hpp"
#include <unistd.h>
#include "parser.hpp"



state glac;
moveQueue queue;

void animationManager(ctx& ctx){
    
}

void movementManager(scf& scf, ctx& ctx, int32_t x, int32_t y){
    if(queue.Size()==0){

    }else{
        nextMove mvnow = queue.getNow();
        int8_t xl;
        int8_t yl;
        if(mvnow.pos.x>glac.x)xl = 1;
        if(mvnow.pos.x<glac.x)xl = -1;
        if(mvnow.pos.y>glac.y)yl = 1;
        if(mvnow.pos.y<glac.y)yl = -1;
        for(int i = 0;i<glac.speed;i++){
            if(mvnow.pos.x==glac.x)xl=0;
            if(mvnow.pos.y==glac.y)yl=0;
            glac.x+=xl;
            glac.y+=yl;
        }
        if(mvnow.pos.x==glac.x&&mvnow.pos.y==glac.y)queue.goNext();
    }
    animationManager(ctx);
    scf.move(glac.x,glac.y);
}

Lock comandLock;

int main(){

    //daemon(0,0);

    std::thread([&](){
        while(true){
            std::cout << std::endl << ">";
            std::string cmd;
            std::getline(std::cin,cmd);
            WriteLock lock(comandLock);
            std::string id;
            dsl::parser p(cmd);
            id = p.getWord();
            std::cout << id;
            if(id=="go"){
                nextMove mv;
                mv.pos.x = p.getNumber();
                mv.pos.y = p.getNumber();
                mv.reason = p.getNumber();
                queue.push(mv);
            }else if(id=="speed"){
                glac.speed = p.getNumber();
            }else if(id=="quit"){
                abort();
            }

        }
    }).detach();

    scf test(64,64,[&](scf& scf, ctx& ctx, int32_t x, int32_t y){
        WriteLock lock(comandLock);
        ctx.fillRect(0,0,64,64,dsl::getRGB888(x,255,255));
        movementManager(scf,ctx,x,y);
    });
    return 0;
}

