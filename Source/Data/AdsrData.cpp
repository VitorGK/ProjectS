/*
  ==============================================================================

    AdsrData.cpp
    Created: 9 May 2022 9:30:25am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::setADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    setParameters(adsrParams);
}
