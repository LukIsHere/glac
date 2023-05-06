#pragma once
#include <stdint.h>

class vector2D{
    public:
        int32_t x;
        int32_t y;
        vector2D(){}
        vector2D(int32_t x, int32_t y){
            this->x = x;
            this->y = y;
        }
        void operator=(vector2D v){
            this->x = v.x;
            this->y = v.y;
        }
        void operator+=(vector2D v){
            this->x += v.x;
            this->y += v.y;
        }
        void operator-=(vector2D v){
            this->x -= v.x;
            this->y -= v.y;
        }
};

struct nextMove{
    vector2D pos;
    uint8_t reason;
};
class moveQueue{
    private:
        nextMove next[20];
        uint8_t taken = 0;
        uint8_t writePointer = 0;
        uint8_t readPointer = 0;
    public:
        moveQueue(){
            push(nextMove{vector2D(0,0),0});
        }
        void push(nextMove v){
            next[writePointer] = v;
            writePointer = (writePointer+1)%20;
            taken++;
        }
        nextMove getNow(){
            nextMove v = next[readPointer];
            return v;
        }
        void goNext(){
            if(taken == 0){
                return;
            }
            readPointer = (readPointer+1)%20;
            taken--;
        }
        uint8_t Size(){
            return taken;
        }
};

enum direction{
    still,
    left,
    right,
    up,
    down,
    upleft,
    downleft,
    downright,
    upright
};

enum animation{
    stand,
    sit,
    whahapen,
    sad,
    happy,
    slep,
    laydown
};

char* text;

struct state{
    int32_t x;
    int32_t y;
    uint8_t frame;
    uint8_t speed;
    direction dir;
    animation anim;
};

