#include "wave_func.hpp"
#include <cmath>
const float PI2 = 6.283185f; // PI * 2

///////////////////////////////////////////////////////////////////////////////
// compute the position at the current time(sec)
// Equation: amp * FUNC(freq*(t - phase)) + offset
///////////////////////////////////////////////////////////////////////////////
float CWaveFunc::Update(float time)
{
    // compute time factor between 0 and 1 from (freq*(time - phase))
    float timeFact = freq * (time - phase);
    timeFact -= (int)timeFact;

    switch(func)
    {
    case FUNC_SIN:
        output = sinf(PI2 * timeFact);
        break;

    case FUNC_TRIANGLE:
        if(timeFact < 0.25f)            // 0 ~ 0.25
            output = 4 * timeFact;
        else if(timeFact < 0.75f)       // 0.25 ~ 0.75
            output = 2 - (4 * timeFact);
        else                            // 0.75 ~ 1
            output = 4 * timeFact - 4;
        break;

    case FUNC_SQUARE:
        if(timeFact < 0.5f)
            output = 1;
        else
            output = -1;
        break;

    case FUNC_SAWTOOTH:
        output = 0.5 * timeFact - 1;
        break;

    default:
        output = 1; // no function defined
    }

    // apply amplitude and offset
    output = amp * output + offset;

    return output;
}
