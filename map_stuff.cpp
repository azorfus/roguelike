#include <iostream>

int map[5][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                 };

void xgen(){
for(int x = 0; x<5; x++){
        for(int y = 0; y<10; y++){
            map[x][y] = 0;
        }
    }
}

int placementx;
int placementy;
int P;

void printMAP()
{
    int count = 0;
    for(int x = 0; x<5; x++){
        for(int y = 0; y<10; y++){
            switch(map[x][y]){
                case 0:
                    std::cout << "\u001b[30;1m.\u001b[0m";
                    count++;    
                  if(count>=10){
                    std::cout<<std::endl;
                  count = 0;
                  }  
                    break;
                case 1:
                    std::cout << '#';
                    count++;    
                    if(count>=10){
                      std::cout<<std::endl;
                    count = 0;
                   }
                    break;
                case 2:
                    std::cout << "\u001b[34;1m@\u001b[0m";
                    count++;    
                    if(count>=10){
                      std::cout<<std::endl;
                    count = 0;
                   }
                    break;
                
                count++;    
                if(count>=10){
                std::cout<<std::endl;
                count = 0;
                }
            }
        }
}
}


void gen(){
    int c = 0;
    while(c!=10){
            placementx=rand()%5;
            placementy=rand()%10;
            P = rand()%2+1;

        map[placementx][placementy] = 1;
        c++;
    }
    if(P == 1){
    map[0][placementy] = 2;
    }else if(P == 2){
    map[4][placementy] = 2;
    }
     
}

int main()
{
    srand(time(0));
    gen();
    while(true){   
    printMAP();
    std::string ui;
    std::getline(std::cin, ui);
    if(ui == "qwerty"){
        xgen();
        gen();
    }
    if(ui == "a"){
        printMAP();
    }

   }

}