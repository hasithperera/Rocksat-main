# Rocksat-main

The final code will run the WVU RocksatX experiment. 

## configuration 

- The Makefile needs to be changed to show the correct hw platfrom. Set Model(line 1) accordingly
- `./bin/test_Z10_64.bin` is the final FPGA image 

## Testing

- `./scripts/test_1.sh` runs a test sequence to ensure the PWM and experiment restart functionality is operational
- `./scripts/pre_launch.sh` clears the state history to make it ready for launch

## Output file format

The file name created during the experiment will have the following syntax `xxxxx_y_zzzzz.bin`.
- `xxxxx` specifies the run id. This is increased automatically on a code restart
- `y`indicates the experiment id.
- `zzzzz` specifies the time block

The files will contain a total of $(2 + \bufferlength) \times 4$ bytes of binary data. the first 16 bytes contain a time stamp followed by data expored by the user.
You can use the following python code block to access the raw data

```
file_i = 'data/00018_1_{:05}.bin'.format(0)
time_stamp = np.fromfile(file_i, dtype=np.intc, count=2, offset = 0)
data = np.fromfile(file_i, dtype=np.float32, count=-1, offset = 8)
	
```
## To-Do list

- [x] IO control
- [x] LP SPI DAC
- [x] LP SPI ADC: Using RF2 input
- [x] File saving
- [x] Sounding random numbers: Via RF1 output
- [x] PWM control


