#include <cstdio>
#include <ctime>
#include "SBUS.h"
#include "/home/ubuntu/workspace/spi_driver/Utilities/PeriodicTask.h"

PeriodicTaskManager demomanager;

RC_controller sbus(&demomanager,"/dev/ttyUSB0",100);

int main()
{
    printf("SBUS nonblocking receiver example\n\r");

    sbus.start();
    printf("SBUS  receiver start\n\r");


size_t inter=0;
while(1)
{
   
    if(++inter==100000)
    {
        inter=0;
            printf("inter  enter\n\r");

        for(size_t i=0;i<16;i++)
        {
            printf("channels %d: %d",i,(int)(sbus.channel_remaped(i)));
            // printf("channels %d: %d",i,(int)(sbus.channel(i)));
        }
            printf("\n\r");

    }
}

    return 0;
}