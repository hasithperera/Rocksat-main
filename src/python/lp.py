# test program for LP mode
# author: Hasith Perera (aep00010@mix.wvu.edu)
# 19/02/2024 

import rp
import time
import numpy as np

analog_out = [rp.RP_AOUT0, rp.RP_AOUT1, rp.RP_AOUT2, rp.RP_AOUT3]
out_voltage = [1.0, 1.0, 1.0, 1.0]

PORTN = 0x00
pinN = 0x00

#####! Choose one of two methods, comment the other !#####

#! METHOD 1: Configuring specific Analog pin
#for i in range(4):
#    rp.rp_ApinSetValue(analog_out[i], out_voltage[i])
#    print (f"Set voltage on AO[{i}] to {out_voltage[i]} V")


def voltage_sweep(dt=900,bits=12,vmax=1.8):
    '''sweeps the voltage
    ADC spec
        12-bit 
        0-1.8 V
    '''
    dv = vmax / 2**bits
    rp.rp_DpinSetState(rp.RP_DIO0_N,rp.RP_HIGH)

    rp.rp_LEDSetState(0x01)
    for i in range(0,2**bits-1):
        #print(f'{time.time()} \t {i} {dv*i}')
        #rp.rp_AOpinSetValue(3, 1.2*np.mod(i,2))
        rp.rp_AOpinSetValue(3, dv*i)
        rp.rp_AOpinSetValue(2, dv*i)
        #rp.rp_AOpinSetValue(2, 1.2*(np.mod(i,10)>5))
        time.sleep((dt/1000)/(2**bits-1))

    rp.rp_DpinSetState(rp.RP_DIO0_N,rp.RP_LOW)
    rp.rp_LEDSetState(0x00)
    time.sleep((1000-dt)/1000)


def digital_IO():
    
    #E1 N
    rp.rp_DpinSetDirection(rp.RP_DIO0_N,rp.RP_OUT)

if __name__=='__main__':

    print("Testing LP mode")

    # Initialize the interface
    rp.rp_Init()
    
    digital_IO()

    # Reset analog pins
    rp.rp_ApinReset()
    i = 0
    while(1):
        i = i + 1
        vmax = 1.5 + .3*(i%2)
        print(vmax)

        voltage_sweep(vmax=vmax,bits=8)
        rp.rp_LEDSetState(0x00)     
    # Release resources
    rp.rp_Release()
