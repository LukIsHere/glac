#pragma once
#include "dsl/ctx.hpp"
#include "string.h"

struct character{
    char c;
    dsl::argb8888 color;
    void operator=(char c){
        this->c = c;
    }
};

class terminal{
        character* buffer;
        uint32_t height;
        uint32_t width;
        uint32_t line;
        uint32_t cursor;
        dsl::ctx8888 ctx;
        uint8_t scale;
        dsl::argb8888 color;
        dsl::argb8888 bg;
        character& getChar(uint32_t x, uint32_t y){
            if(x>width||y>height){
                return buffer[0];
            }
            return buffer[(y*width) + x];
        };
        void moveDown(){
            for(uint32_t y = 1; y < height; y++){
                for(uint32_t x = 0; x < width; x++){
                    getChar(x,y-1).color = getChar(x,y).color;
                }
            }
        }
    public:
        terminal(uint32_t width, uint32_t height, uint8_t scale, dsl::argb8888 color = dsl::RGB(50,255,50),dsl::argb8888 bg = dsl::ARGB(1,1,1,255)):ctx(width*scale*6, height*scale*6){
            this->width = width;
            this->height = height;
            this->scale = scale;
            this->color = color;
            this->bg = bg;
            this->line = 0;
            this->cursor = 0;
            this->buffer = new character[width*height];
        };
        ~terminal(){
            delete[] buffer;
        }
        dsl::ctx8888& draw(){
            ctx.fill(bg);
            for(uint32_t y = 0; y < height; y++){
                for(uint32_t x = 0; x < width; x++){
                    ctx.drawLetter(getChar(x,y).c,x*scale*6,y*scale*6,getChar(x,y).color,scale);
                }
            }
            return ctx;
        };
        void clear(){
            line = 0;
            cursor = 0;
            for(uint32_t i = 0; i < width*height; i++){
                buffer[i].c = ' ';
                buffer[i].color = dsl::RGB(0,0,0);
            }
        };
        void print(const char* str){
            print(str, color);
        };
        void print(const char* str, dsl::argb8888 color){
            for(uint32_t i = 0; i < strlen(str); i++){
                print(str[i], color);
            }
        };
        void print(char c){
            print(c, color);
        };
        void print(char c,dsl::argb8888 color){
            if(char(c) == '\n'){
                line++;
                cursor = 0;
                if(line >= height){
                    line--;
                    moveDown();
                }
                return;
            }
            getChar(cursor, line).color = color;
            getChar(cursor, line).c = c;
            cursor++;
            if(cursor >= width){
                cursor = 0;
                line++;
            }
            if(line >= height){
                line--;
                moveDown();
            }
        };
};