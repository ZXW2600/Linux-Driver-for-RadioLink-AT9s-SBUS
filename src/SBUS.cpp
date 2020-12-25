#include "SBUS.h"
#include <cstdio>
#include <termios.h>

#define SBUS_DESYNC_END 2
#define SBUS_DESYNC_HDR 1
#define READ_BUF_SIZE (SBUS_PACKET_SIZE * 2)

/* define range mapping here, -+100% -> 1000..2000 */
#define SBUS_RANGE_MIN 306.0f
#define SBUS_RANGE_MAX 1694.0f
#define SBUS_TARGET_MIN 2000.0f
#define SBUS_TARGET_MAX 0.0f
/* pre-calculate the floating point stuff as far as possible at compile time */
#define SBUS_SCALE_FACTOR ((SBUS_TARGET_MAX - SBUS_TARGET_MIN) / (SBUS_RANGE_MAX - SBUS_RANGE_MIN))
#define SBUS_SCALE_OFFSET (int)(SBUS_TARGET_MIN - (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f))
void RC_controller::remap()
{
    for(size_t i=0;i<16;i++)
        _channels_remap[i]=_channels[i]*SBUS_SCALE_FACTOR+SBUS_SCALE_OFFSET;
}


RC_controller::RC_controller(PeriodicTaskManager* taskManager,std::string devPath,double frenquency)
    : _nextRead(READ_BUF_SIZE),
    _taskManager(taskManager),
    _devPath(devPath),
    sbus_flush_task {taskManager,1/frenquency,"sbus flush task",&RC_controller::read,this}
{
    //初始化SBUS
    install(_devPath.c_str(),true);
}

RC_controller::~RC_controller()
{
    sbus_uninstall(&_fd);
}

void RC_controller::install(const char *path, bool blocking, uint8_t timeout)
{
    sbus_err_t err= sbus_install(&_fd, path, blocking, timeout);
       if (err != SBUS_OK)
    {
        fprintf(stderr, "SBUS install error: %d\n\r", err);
    }
}

uint16_t RC_controller::channel(int num) 
{
    if (num >= 0 & num < 16)
        return (uint16_t)(_channels[num]);
    else
        return 0;
}

uint16_t RC_controller::channel_remaped(int num) 
{
    if (num >= 0 & num < 16)
        return (uint16_t)(_channels_remap[num]);
    else
        return 0;
}


sbus_err_t RC_controller::onPacket(sbus_packet_cb cb)
{
    _packetCb = cb;
    return SBUS_OK;
}

void RC_controller::read()
{
    uint8_t readBuf[READ_BUF_SIZE] = {0};
    int nRead = sbus_read(&_fd, readBuf, _nextRead);

        //if ok, do nothing
    // if (nRead <= 0)
        // return SBUS_OK;

    bool hadDesync = false;

    for (int i = 0; i < nRead; i++)
    {
        switch (_state)
        {
            case State::WAIT_FOR_HEADER:
                if (readBuf[i] == SBUS_HEADER)
                {
                    _packet[0] = SBUS_HEADER;
                    _packetPos = 1;
                    _state = State::PACKET;
                }
                break;

            case State::PACKET:
                _packet[_packetPos] = readBuf[i];
                _packetPos++;
                if (_packetPos >= SBUS_PACKET_SIZE)
                {
                    if (verifyPacket())
                    {
                        decodePacket();
                        remap();
                    }
                    else
                    {
                        hadDesync = true;
                    }
                    _state = State::WAIT_FOR_HEADER;
                    _packetPos = 0;
                }
                break;
        }
    }

    if (_state == State::PACKET)
    {
        _nextRead = SBUS_PACKET_SIZE - _packetPos;
    }
    else
    {
        _nextRead = READ_BUF_SIZE;
    }

    if (hadDesync)
    {
        
        fprintf(stderr, "SBUS desync\n\r");
        tcflush(_fd, TCIFLUSH);

    }
    //去掉返回值后如何处理报错
    // return hadDesync ? SBUS_ERR_DESYNC : SBUS_OK;
}

sbus_err_t RC_controller::write(sbus_packet_t packet)
{
    uint8_t opt = 0;
    opt |= packet.ch17 * SBUS_OPT_C17;
    opt |= packet.ch18 * SBUS_OPT_C18;
    opt |= packet.failsafe * SBUS_OPT_FS;
    opt |= packet.frameLost * SBUS_OPT_FL;
    sbus_write(&_fd, packet.channels, opt);
    return SBUS_OK;
}

bool RC_controller::verifyPacket()
{
    return (_packet[0] == SBUS_HEADER) && (_packet[SBUS_PACKET_SIZE - 1] == SBUS_END);
}

void RC_controller::decodePacket()
{

    uint8_t opt = 0;
    uint16_t temp[16];
    sbus_err_t ret = sbus_decode(_packet, temp, &opt);
    for (size_t i = 0; i < 16; i++)
    {
        _channels[i]=temp[i];
    }
    
    // if (ret == SBUS_OK && _packetCb != nullptr)
    // {
    //     sbus_packet_t packet = {
    //             _channels,
    //             (bool) (opt & SBUS_OPT_C17),
    //             (bool) (opt & SBUS_OPT_C18),
    //             (bool) (opt & SBUS_OPT_FS),
    //             (bool) (opt & SBUS_OPT_FL)
    //     };
    //     _packetCb(packet);
    // }
}

    void RC_controller:: start()
    {
sbus_flush_task.start();
    }
    void RC_controller::stop()
    {
sbus_flush_task.stop();

    }