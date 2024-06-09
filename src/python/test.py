## Authour: Hasith Perera
## Date: 03/03/2024


## Build a python program to send commands
## reusing the 8 bit LEDs for data
## designed to run testing


import time
import rp




if __name__=='__main__':
    rp.rp_Init()
    
    #set PWM out puts
    rp.rp_DpinSetDirection(rp.RP_DIO3_N,rp.RP_OUT)
    rp.rp_DpinSetDirection(rp.RP_DIO2_N,rp.RP_OUT)
    rp.rp_DpinSetDirection(rp.RP_DIO1_N,rp.RP_OUT)
    
    rp.rp_LEDSetState(255)

    while(1):
        usr = input('>')
        try:
            num = int(usr)
            print(num)
            if (num > 256):
                print('!')
                continue
            rp.rp_LEDSetState(num)
        except:
            print('!')
            pass
        

        if usr=='q':
            print('clearing outputs')
            rp.rp_Release()
            exit()
