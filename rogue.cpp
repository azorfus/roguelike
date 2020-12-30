#include <iostream>
#include "map.h"
#include <time.h>
#include <vector>
#include <regex>
#include <cmath>

// money
double cash = 0.0;
// life
int life = 1000;
// items in inventory/cart..
int itemINV = 0;
int itemCART = 0;

// damange done every move
int dmg=1;
// record of dmg
int recdmg=0;

//for visible coords
int recX = 0;
int recY = 0;

// to ensure that the player is holding only one cart
int cartCOUNT=0;

// items on the map
std::vector <std::string> itemsM = {};
// items in cart. if player is using a cart   
std::vector <std::string> cart = {};
// items in inventory
std::vector <std::string> inventory = {};

// for color switch
bool colorMODE;

// if a tile is empty
bool empty;

// if player is holding a cart
bool cartHold;

// if player at a cart tile
bool cartShow;

// tracking inventory items 
int fur, silk_cloth, silver_watch, gold_watch, iron_nug, gold_nug;

static std::regex operator ""_r(const char* pattern, std::size_t length)
{
    return std::regex(pattern,length);
}

namespace{
    std::regex take ("((take|t) +)?(a|all)"_r);
    std::regex drop ("((drop|d) +)?(a|all)"_r);
    std::regex pull ("((pull|pl) +)?(c|cart)"_r);
    std::regex put ("((((put|pt) +)?(a|all) +?)(i|in)) +?(c|cart)"_r);
    std::regex takeC ("((((take|tk) +)?(a|all) +?)(f|from)) +?(c|cart)"_r);
    std::regex leave ("((leave|l) +)?(c|cart)"_r);
}

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

void printItems(){
    int size = itemsM.size();

    if(!itemsM.empty()){
        if(colorMODE==true){
            for(int j=4;j-4<size;j++){
                std::cout << "\033[1J\033[H";
                std::cout<<"\033[s\033["<<j<<";20f\u001b[32;1m"<<itemsM[j-4]<<"\u001b[0m\033[u\n";
            }
        } else {
            for(int j=4;j-4<size;j++){
                std::cout << "\033[1J\033[H";
                std::cout<<"\033[s\033["<<j<<";20f"<<itemsM[j-4]<<"\033[u\n";
            }
        }
    }
}

void showItems()
{
    for(int i=0;i<5;i++){
        int choose=rand()%25+1;
        switch(choose){
            case 2:case 4:itemsM.push_back("fur");break;
            case 6:case 3:itemsM.push_back("silk cloth");break;  
            case 13:case 11:itemsM.push_back("silver watch");break;
            case 17:case 23:itemsM.push_back("gold watch");break;
            case 10:case 19:itemsM.push_back("iron nugget");break;
            case 14:case 25:itemsM.push_back("gold nugget");break;
            case 8:itemsM.push_back("diamond ring");break;
            case 7:case 24:itemsM.push_back("pearl necklace");break;
            
        }
    }
    printItems();
}

void ItemCount()
{
    if(!itemsM.empty()){
        for(int i=0;i<round(itemsM.size());i++){
            if(itemsM[i]=="fur"){cash+=10.25;dmg+=1;}
            else if(itemsM[i]=="silk cloth"){cash+=20;dmg+=1;}
            else if(itemsM[i]=="silver watch"){cash+=100.95;dmg+=1;}
            else if(itemsM[i]=="gold watch"){cash+=300.50;dmg+=2;}
            else if(itemsM[i]=="iron nugget"){cash+=80;dmg+=2;}
            else if(itemsM[i]=="gold nugget"){cash+=140.25;dmg+=2;}
            else if(itemsM[i]=="diamond ring"){cash+=730.95;dmg+=3;}
            else if(itemsM[i]=="pearl necklace"){cash+=400.85;dmg+=3;}
        }            
    }
}

// for tracking dmg in stats()
int u=0;


void stats()
{
    std::cout<<"\033[s\033[2;20HX: "<<recX<<" Y: "<<recY<<"\033[u";
    if(cartHold==true){
        if(u==0){dmg=4; u++;}
        std::cout << "\033[1J\033[H";
        std::cout<<"\033[3;20Ha cart with ["<<cart.size()<<"] items\033[u";
    }
    else if(cartShow==true){
        std::cout << "\033[1J\033[H";
        std::cout<<"\033[4;20Ha cart\033[u";
    }
}

int main()
{
    srand(time(0));
    
    int ran = rand()%8+1;
    int ran0 = rand()%8+1;
    char colorOP;
    bool Take;
    bool ERROR;

    std::cout<<"Do you want colors? (If you see random text but no colors type 'n') [y/n]: ";
    std::cin>>colorOP;
    switch(colorOP){
        case 'y': case 'Y': colorMODE=true;break;
        case 'n': case 'N': colorMODE=false;break;
        default: std::cout<<"Not a valid input\n";exit(0);
    }

    std::string user_input;
    mapGen(map);
    map[ran][ran0] = 4;
    bool ich, cch;
    ich=cch=false;        
    std::smatch part;
    std::cout<<"You are in a dungeon starving to death... find items...\nthe more items you find the more money you get for food\n";
    std::cin.ignore();
    

    while(life>0){
//        system("clear");
        std::cout << "\033[1J\033[H";
        empty=false;
        stats();
        printMAP(map, colorMODE);
        if(colorMODE==true){std::cout << "\u001b[37;1m[life:" << life << "]$ \u001b[0m";}
        if(colorMODE==false){std::cout << "[life:"<<life<<"]$ ";}
        std::getline(std::cin, user_input);
        for(int x=0;x<10;x++){
            for(int y=0;y<10;y++){
            // for the movement of the character and a tile checking code
                switch(map[x][y]){
                    case 4:
                        
                       if(user_input=="w"){
                            life-=dmg;
                            cartShow=false;
                            ERROR=false;
                            int x0 = x-1;
                            Take=false;
                            switch(map[x0][y]){
                                case 0:case 2:case 3:
                                    if(x<=0 && x0<=-1){
                                        recX--;
                                        mapRem(); // map clearing
                                        mapGen(map); // new map generation
                                        map[9][y] = 4;
                                    }else{
                                        if(ich==true){
                                            ich=false;
                           		    itemsM.clear();
                                            map[x][y]=2;
                                        }if(cch==true){
                                            cch=false;
                           		    itemsM.clear();
                                            map[x][y]=3;
                                        }else{   
                                            if(map[x0][y]==2){
                                              if(empty==false){
                                                showItems();
                                                }
                                              } else if(map[x0][y]==3){
                                                 cartShow=true;
                                                 cch=true;
                                              }
                                                ich=true;
                                                Take=true;
                                                empty=false;
                                            
                                            map[x][y] = 0;
                                        }
                                        x--;
                                        recX--;
                                        map[x0][y] = 4;
                                    }
                                    break;

                            }
                        }//ifw

                        else if(user_input=="s"){
                            life-=dmg;
                            cartShow=false;
                            ERROR=false;
                            int x0 = x+1;
                            Take=false;
                            switch(map[x0][y]){
                                case 0:case 2:case 3:
                                    if(x>=9 && x0<=10){
                                        recX++;
                                        mapRem();
                                        mapGen(map);
                                        map[0][y] = 4;
                                    }else{
                                        if(ich==true){
                                            ich=false;
                           		    itemsM.clear();
                                            map[x][y]=2;
                                        }if(cch==true){
                                            cch=false;
                           		    itemsM.clear();
                                            map[x][y]=3;
                                        }else{   
                                            if(map[x0][y]==2){
                                              if(empty==false){
                                                showItems();
                                                }
                                              } else if(map[x0][y]==3){
                                                 cartShow=true;
                                                 cch=true;
                                              }
                                                ich=true;
                                                Take=true;
                                                empty=false;
                                            
                                            map[x][y] = 0;
                                        }
                                        x++;
                                        recX++;
                                        map[x0][y] = 4;
                                    }
                                    
                                    break;

                            }
                        }//ifs

                        else if(user_input=="a"){
                            life-=dmg;
                            cartShow=false;
                            ERROR=false;
                            int y0 = y-1;
                            Take=false;
                            switch(map[x][y0]){
                                case 0:case 2:case 3:
                                    if(y<=0 && y0<=11){
                                        recY--;
                                        mapRem();
                                        mapGen(map);
                                        map[x][9] = 4;
                                    }else{
                                       if(ich==true){
                                            ich=false;
                           		    itemsM.clear();
                                            map[x][y]=2;
                                        }if(cch==true){
                                            cch=false;
                           		    itemsM.clear();
                                            map[x][y]=3;
                                        }else{   
                                            if(map[x][y0]==2){
                                              if(empty==false){
                                                showItems();
                                                }
                                              } else if(map[x][y0]==3){
                                                 cartShow=true;
                                                 cch=true;
                                              }
                                                ich=true;
                                                Take=true;
                                                empty=false;
                                            
                                            map[x][y] = 0;
                                        }
                                        y++;
                                        recY++;
                                        map[x][y0] = 4;
                                    }
             
                                    break;

                            }
                        }//ifd

                        else if(user_input=="d"){
                            life-=dmg;
                            cartShow=false;
                            ERROR=false;
                            int y0 = y+1;
                            Take=false;
                            switch(map[x][y0]){
                                case 0:case 2:case 3:
                                    if(y>=9 && y0<=10){
                                        recY++;
                                        mapRem();
                                        mapGen(map);
                                        map[x][0] = 4;
                                    }else{

                                        if(ich==true){
                                            ich=false;
                           		    itemsM.clear();
                                            map[x][y]=2;
                                        }if(cch==true){
                                            cch=false;
                           		    itemsM.clear();
                                            map[x][y]=3;
                                        }else{   
                                            if(map[x][y0]==2){
                                              if(empty==false){
                                                showItems();
                                                }
                                              } else if(map[x][y0]==3){
                                                 cartShow=true;
                                                 cch=true;
                                              }
                                                ich=true;
                                                Take=true;
                                                empty=false;
                                            
                                            map[x][y] = 0;
                                        }
                                        y++;
                                        recY++;
                                        map[x][y0] = 4;
                                    }
                                    break;

                            }
                        }//ifd
                }//switch,x,y
                
            }//fy
        }//fx
                        
            
                        if(std::regex_match(user_input, part, pull)){
                            if(cartShow==true&&cartCOUNT==0){
                                cartHold=true;
                                cch=false;
                                cartCOUNT=1;
                            }else{std::cout<<"\nThere is no cart here! <Enter>";std::cin.ignore();}
                        }
                        else if(std::regex_match(user_input, part, leave)){
                            if(cartCOUNT>=1){
                                u=0;
                                cartHold=false;
                                cch=true;
                                cartCOUNT=0;
                            }else{std::cout<<"\nYou have no cart! <Enter>";std::cin.ignore();}
                        }
                        else if(std::regex_match(user_input, part, put)){
                            if(!inventory.empty()){
                                recdmg+=dmg;
                                if(dmg>=10){dmg=6;}
                                else if(dmg>=20){dmg=8;}
                                else if(dmg<10){dmg=5;}
                                dmg++;
                                for(int i=0;i<round(inventory.size());i++){
                                    cart.push_back(inventory[i]);
                                }
                                inventory.clear();
                            }
                        }
                        else if(std::regex_match(user_input, part, takeC)){
                            if(!cart.empty()){
                                for(int i=0;i<round(cart.size());i++){
                                    inventory.push_back(cart[i]);
                                }
                                cart.clear();
                                dmg=recdmg+4;
                                u=1;
                            }
                        }

                        else if(std::regex_match(user_input, part, take)){
                            ERROR=false;
                            if(!itemsM.empty()){
                                ItemCount();
                                for(int i=0;i<round(itemsM.size());i++){
                                    inventory.push_back(itemsM[i]);
                                }                                
                            ich=false;
                            empty=true;
                            itemsM.clear();
                            }
                        }
                        else if(std::regex_match(user_input, part, drop)){
                            ERROR=false;
                            if(!inventory.empty()){
                                ich=true;
                                inventory.clear();
                                cash=0;
                                dmg=1;
                            }
                        }
                        else if(std::regex_match(user_input, "i|inv|inventory"_r)){
                            ERROR=false;
                            if(!inventory.empty()){
                                std::cout << "\033[1J\033[H";
                                for(int i=0;i<round(inventory.size());i++){
                                    std::cout<<inventory[i]<<", ";
                                }
                                std::cout<<"\n\nLife damaged for every step taken: "<<dmg<<"\n\n[Cash:"<<cash<<"]\n\n<Enter>";
                                std::cin.ignore();
                            }else{
                                std::cout << "\033[1J\033[H";
                                std::cout<<"\033[s\033[5;20f\u001b[32;1mYou have nothing!\u001b[0m\033[u\n";
                                
                            }
                        }
                        else if(user_input == "help"){
                            std::cout<< "\033[1J\033[H";
                            std::cout<<"Movement commands:\n\n'w' - move north\n's' - move south\n'a' - move west\n'd' - move east\n\n";
                            std::cout<<"Interactive controls:\n\n'drop/d all/a' - to drop all of your items [WARNING: you will not get them back]\n";
                            std::cout<<"'take/t all/a' - to take all items on the floor\n";
                            std::cout<<"'put/pt a/all i/in c/cart' - put everthing you have in your inventory to a cart\n";
                            std::cout<<"'take/tk a/all f/from c/cart' - take everthing you have in your cart to your inventory\n";
                            std::cout<<"'pull/pl c/cart' - pull a cart\n";
                            std::cout<<"'leave/l c/cart' - leave your cart [Note: Items in your cart will stay in the cart and can be taken from any cart";
                            std::cout<<"\n\n<Enter>";
                            std::cin.ignore();
                        }
                        else if(user_input == "quit"){
                            break;
                        }                        
                        
    }// main while
    
    if(colorMODE==true){
    std::cout << "\033[1J\033[H\u001b[31;1m";
    std::cout << "\n[Life: "<<life<<"]\n"; 
    if(cash>=10000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou won! With all the money you collected you bought yourself a Meal of luxury steak, chocolate cake and coffe!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=5000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of chicken stew, vanilla ice-cream and orange juice!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=2000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of vegetable soup, porridge and some tea!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=1000||cash<=1000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of canned beans, pale apples and warm water!\n";
        std::cout<<"\ncash: "<<cash;
    }else{
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself some milk!\n";
        std::cout<<"\ncash: "<<cash;
    }
        std::cout<<"\n\n\u001b[0m";
    }else{
    std::cout << "\n[Life: "<<life<<"]\n";
    if(cash>=10000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou won! With all the money you collected you bought yourself a Meal of luxury steak, chocolate cake and coffe!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=5000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of chicken stew, vanilla ice-cream and orange juice!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=2000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of vegetable soup, porridge and some tea!\n";
        std::cout<<"\ncash: "<<cash;
    }else if(cash>=1000||cash<=1000){
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself a Meal of canned beans, pale apples and warm water!\n";
        std::cout<<"\ncash: "<<cash;
    }else{
        std::cout<<"You had...\n";
        for(int i=0;i<round(inventory.size());i++){
            std::cout<<inventory[i]<<", ";
        }
        std::cout<<"\n\nYou lost! With all the money you collected you bought yourself some milk!\n";
        std::cout<<"\ncash: "<<cash;
    }
        std::cout<<"\n\n\u001b[0m";
    
    }    
}



