#pragma once

struct PresetData
{
    double attack, decay, sustain, release;
    long holdTime;
    
    PresetData()
    {
        attack = 0.1;
        decay = 0.1;
        sustain = 0.5;
        release = 0.999;
        holdTime = 1;
    }
};
