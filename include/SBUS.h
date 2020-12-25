#ifndef RPISBUS_SBUS_H
#define RPISBUS_SBUS_H

#include <cstdint>
#include "/home/ubuntu/workspace/spi_driver/Utilities/PeriodicTask.h"

#include <atomic>

#include "sbus_driver.h"

struct sbus_packet_t
{
    uint16_t *channels;
    bool ch17, ch18;
    bool failsafe;
    bool frameLost;
};

typedef void (*sbus_packet_cb)(sbus_packet_t);

class RC_controller
{
public:
    RC_controller(PeriodicTaskManager* taskManager,std::string devPath,double frenquency); 
   ~RC_controller();

    uint16_t channel(int num);
    uint16_t channel_remaped(int num) ;

    sbus_err_t onPacket(sbus_packet_cb cb);
    sbus_err_t write(sbus_packet_t packet);
    void start();
    void stop();
private:
PeriodicTaskManager* _taskManager;
PeriodicMemberFunction<RC_controller> sbus_flush_task;
std::string _devPath;
    enum class State
    {
        WAIT_FOR_HEADER,
        PACKET,
    } _state = State::WAIT_FOR_HEADER;

    uint8_t _packet[SBUS_PACKET_SIZE] = { 0 };
    int _packetPos = 0;
    int _nextRead;
    int _fd = -1;

    std::atomic_uint16_t _channels[16] ;
    std::atomic_uint16_t _channels_remap[16] ;
    sbus_packet_cb _packetCb = nullptr;

    void install(const char *path, bool blocking, uint8_t timeout = 0);
    void read();
    bool verifyPacket();
    void decodePacket();
    void remap();
    
};


#endif //RPISBUS_SBUS_H
