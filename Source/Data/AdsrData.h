/*
  ==============================================================================

    AdsrData.h
    Created: 9 May 2022 9:30:25am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData: public ADSR {
public:
    void setADSR(const float attack, const float decay, const float sustain, const float release);
    
private:
    ADSR::Parameters adsrParams;
};
