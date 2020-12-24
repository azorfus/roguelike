#include <iostream>
#include "map.h"
#include <time.h>

int pfs = 0;

//void mapRem(int (&map)[6][10])
//{
//    map[6][10] = {0};
//}

//int spawn(int (&map)[10][10], int x, int y, int p){
 //   if (x<=0) map[9][y] = 4;
   // else if (x>=10) map[1][y] = 4;
    //else if (y>=10) map[x][1] = 4;
   // else if (y<=0) map[x][9] = 4;
   // else return false;
   // return 0;
//}

void mapGen(int (&map)[10][10])
{
    int walls = 0;
    int placex = 0;
    int placey = 0;
    int which = 0;
    while(walls<22){
        which = rand()%6+1;
        placex = rand()%9+1;
        placey = rand()%9+1;
        
        map[placex][placey] = 1;
        if(which%3 == 0){
            map[placex][placey] = 2;
        }if(which%6 == 0){
            map[placex][placey] = 3;
        }
        walls++;
    }
//    map[placex][placey] = 4;
    
}

int printMAP(int map[10][10])
{
    int count = 0;
    for(int x=0;x<10;x++){
        for(int y=0;y<10;y++){
            switch(map[x][y]){
                case 0:
                    std::cout << "\u001b[30;1m.\u001b[0m";
                    if(++count>=10){
                        std::cout<<"\u001b[30;1m |\u001b[0m\n";
                        count = 0;
                    }
                    break;
                
                case 1:
                    std::cout << '#';
                    if(++count>=10){
                        std::cout<<"\u001b[30;1m |\u001b[0m\n";
                        count = 0;
                    }
                    break;
               case 2:
                    std::cout << "\u001b[32;1mi\u001b[0m";
                    if(++count>=10){
                        std::cout<<"\u001b[30;1m |\u001b[0m\n";
                        count = 0;
                    }
                    break;
               
               case 3:
                    std::cout << "\u001b[33;1mc\u001b[0m";
                    if(++count>=10){
                        std::cout<<"\u001b[30;1m |\u001b[0m\n";
                        count = 0;
                    }
                    break;
                
               case 4:
                    std::cout << "\u001b[36;1m@\u001b[0m";
                    if(++count>=10){
                        std::cout<<"\u001b[30;1m |\u001b[0m\n";
                        count = 0;
                    }
                    break;

            }
    
        }//fy
    }//fx
    std::cout<<std::endl;
    return 0;    
}