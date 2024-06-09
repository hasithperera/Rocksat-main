#!/usr/bin/python3

import time
import rp

#overlay (needs testing-add image needed)
#from rp_overlay import overlay

def signal_gen():
    ''' Test signal generation '''
    channel = rp.RP_CH_2        # rp.RP_CH_2
    waveform = rp.RP_WAVEFORM_SINE
    freq = 5e6
    ampl = .2                    # One way amplitude (reference to GND)
    rp.rp_GenReset()
    rp.rp_GenWaveform(channel, waveform)

    rp.rp_GenAmp(channel, ampl)
    rp.rp_GenOutEnable(channel)
    rp.rp_GenTriggerOnly(channel)
    #print("start sin")



# Initialize the interface
rp.rp_Init()

#####! Choose one of two methods, comment the other !#####
#! METHOD 1: Interacting with Registers direclty

channel = rp.RP_CH_2        # rp.RP_CH_2
freq = 5e6
freq_tmp = 5e6;
df = 15e6/150
period = 1e-4     # period in sec


while 1:
    signal_gen()
    #rp.rp_GenWaveform(channel, waveform)
    #rp.rp_GenAmp(channel, ampl)
    #rp.rp_GenTriggerOnly(channel)
    for i in range(0,150):
        rp.rp_GenFreqDirect(channel, freq+df*i)
        #print(freq+df*i)
        rp.rp_LEDSetState(i)     # or 0b00000001
        time.sleep(period)
        rp.rp_LEDSetState(0b00000000)     # or 0
    rp.rp_GenReset()
    #time.sleep(.8)
    #rp.rp_GenReset()
    #time.sleep(800)

rp.rp_Release()

# Release resources



