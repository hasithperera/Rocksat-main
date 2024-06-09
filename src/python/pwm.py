
import rp
import time

if __name__=='__main__':
    print('PWM code')

    rp.rp_Init()
    

    rp.rp_DpinSetDirection(rp.RP_DIO2_N,rp.RP_OUT)
    rp.rp_DpinSetDirection(rp.RP_DIO1_N,rp.RP_OUT)
    
    while(1):
        time.sleep(.1)
        print('.')

