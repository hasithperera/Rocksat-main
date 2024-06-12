# Rocksat-main

The final code will run the WVU RocksatX experiment. 


## configuration 

- The Makefile needs to be changed to show the correct hw platfrom. Set Model(line 1) accordingly
- `./bin/test_Z10_63.bin` is the final FPGA image 

## To-Do list

- [x] IO control
- [x] LP SPI DAC
- [x] LP SPI ADC: Using RF2 input
- [x] File saving
- [x] Sounding random numbers: Via RF1 output
- [x] PWM control

## Testing

- `./scripts/test_1.sh` runs a test sequence to ensure the PWM and experiment restart functionality is operational
- `./scripts/pre_launch.sh` clears the state history to make it ready for launch

