#include <iostream>
#include "map.h"
#include <time.h>

// money
double gold = 0.0;
// life
int life = 1000;
// items in inventory/cart..
int itemINV = 0;
int itemCART = 0;

// map for the game
int map[10][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void mapRem(){
    for(int x=0;x<10;x++){
        for(int y=0;y<10;y++){
            map[x][y] = 0;
        }
    }
}

void genItems()
{
    int silkcloth, ironnugs, goldnugs, woodpen;
    std::cout<<"\033[s\033[5;20H"<<"hello\033[u";
}

int main()
{
    srand(time(0));
    
    int recN, recS, recE, recW, recC; // player tile records.. north south east west and current
    int ran = rand()%8+1;
    int ran0 = rand()%8+1;

    std::string user_input;
    mapGen(map);
    map[ran][ran0] = 4;
    while(life>0){
        std::cout << "\33[1J\033[H";
        genItems();
        printMAP(map);
        std::cout << "\u001b[37;1m[life:" << life << "]$ \u001b[0m";
        std::getline(std::cin, user_input);
        for(int x=0;x<10;x++){
            for(int y=0;y<10;y++){
            // for the movement of the character and a tile checking code
                switch(map[x][y]){
                    case 4:
                        recN=map[--x][y]; recS=map[++x][y];
                        recE=map[x][++y]; recW=map[x][--y];
                        recC=map[x][y];
                        
                        if(user_input=="w"){
                            int x0 = x-1;
                            switch(map[x0][y]){
                                case 0:
                                    if(x<=0 && x0<=-1){
                                        mapRem(); // map clearing
                                        mapGen(map); // new map generation
                                        map[9][y] = 4;
                                    }else{
                                        map[x][y] = 0;
                                        x--;
                                        map[x0][y] = 4;
                                    }
                                    break;
                            }
                        }//ifw

                        if(user_input=="s"){
                            int x0 = x+1;
                            switch(map[x0][y]){
                                case 0:
                                    if(x>=9 && x0<=10){
                                        mapRem();
                                        mapGen(map);
                                        map[0][y] = 4;
                                    }else{
                                        map[x][y] = 0;
                                        x++;
                                        map[x0][y] = 4;
                                    }
                                    break;
                            }
                        }//ifs

                        if(user_input=="a"){
                            int y0 = y-1;
                            switch(map[x][y0]){
                                case 0:
                                    if(y<=0 && y0>=-1){
                                        mapRem();
                                        mapGen(map);
                                        map[x][9] = 4;
                                    }else{
                                        map[x][y] = 0;
                                        y--;
                                        map[x][y0] = 4;
                                    }
                                    break;
                            }
                        }//ifa

                        if(user_input=="d"){
                            int y0 = y+1;
                            switch(map[x][y0]){
                                case 0:
                                    if(y>=9 && y0<=10){
                                        mapRem();
                                        mapGen(map);
                                        map[x][0] = 4;
                                    }else{
                                        map[x][y] = 0;
                                        y++;
                                        map[x][y0] = 4;
                                    }
                                    break;
                            }
                        }//ifd
                        
                        if(user_input == "clearMAP"){
                            mapRem();
                        }
                        
                        break;
                
                
                }//switch,x,y
            }//fy
        }//fx
    }// main while
}


