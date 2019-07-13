#ifndef WAVEFUNC_HPP
#define WAVEFUNC_HPP

enum FuncType       // various periodic functions
{
    FUNC_SIN = 0,
    FUNC_TRIANGLE,
    FUNC_SQUARE,
    FUNC_SAWTOOTH
};

// WaveFunc requires 4 params (amplitude, frequency, phase, offset).
// The equation is: amp * func(freq(t - phase)) + offset
struct CWaveFunc
{
    FuncType func;
    float    amp;       // amplitude
    float    freq;      // frequency
    float    phase;     // horizontal shift
    float    offset;    // vertical shift
    float    output;    // result at given time

    // default constructor, initialize all members
    CWaveFunc() : func(FUNC_SIN), amp(1.0f), freq(1.0f), phase(0.0f), offset(0.0f), output(0.0f) {}

    // compute the position at the current time(sec)
    float Update(float time);
};

#endif
