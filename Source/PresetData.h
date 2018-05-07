#pragma once

struct PresetData
{
    double ampAttack, ampDecay, ampSustain, ampRelease;
    double filterCutoff;
    long ampHoldTime;
    
    PresetData()
    {
        ampAttack = 0.1;
        ampDecay = 0.1;
        ampSustain = 0.5;
        ampRelease = 0.999;
        ampHoldTime = 7000;
        filterCutoff = 20000.0;
    }
};
