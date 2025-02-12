//-----------------------------------------------------------------------------------------------------
// #name clock.h
// #description ���Ԍo�ߊϑ�
// #make �@�@Juan Abadia�i�ȉ���URL�������j
// https://github.com/yabadabu/dx11_video_texture
// #update 
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifndef CLOCK_H
#define CLOCK_H

class Clock {
    uint64_t time_stamp;

public:

    Clock() {
        time_stamp = timeStamp();
    }

    static uint64_t timeStamp() {
        uint64_t now;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&now);
        return now;
    }

    float elapsed() {
        uint64_t now = timeStamp();
        uint64_t delta_ticks = now - time_stamp;

        LARGE_INTEGER freq;
        if (::QueryPerformanceFrequency(&freq)) {
            float delta_secs = (float)(delta_ticks) / (float)freq.QuadPart;
            time_stamp = now;
            return delta_secs;
        }
        return 0.f;
    }
};
#endif // !CLOCK_H

