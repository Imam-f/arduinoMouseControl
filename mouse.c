#include<stdio.h>
#include<windows.h>
#include<time.h>

void delay(int x){
    int starttime = clock();
    while (clock() < starttime + x*CLOCKS_PER_SEC/100) {
    }
}

int main(void) {
    printf("Start\n");
    POINT mousepos;
    int state;
    
    mousepos.x = 0;
    mousepos.y = 0;

    while(!(GetAsyncKeyState(VK_SPACE) & 0x01)){
        printf("Loop\n");
        delay(1);

        GetCursorPos(&mousepos);
        state = ((GetAsyncKeyState(VK_UP) && 1) << 3) | ((GetAsyncKeyState(VK_DOWN) && 1) << 2) | 
            ((GetAsyncKeyState(VK_LEFT) && 1) << 1) | ((GetAsyncKeyState(VK_RIGHT) && 1) << 0);
        SetCursorPos(mousepos.x+(((state >> 0) & 1))*10-(((state >> 1) & 1))*10,
                     mousepos.y+(((state >> 2) & 1))*10-(((state >> 3) & 1))*10);
        
        printf("Loop4\n"); 
    }

    printf("Finish"); 
    return 0;
} 