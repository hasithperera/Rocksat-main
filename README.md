
# RockSat-main

Redpitaya based data acquisition and storage system developed for the Rocksat-X (2024) payload for the West Virginia University team.

RockSat-main is an FPGA/embedded linux based system that is run on a [Redpitaya-STEMlab](https://redpitaya.com/stemlab-125-14/) supporting main antenna deployment, Langmuir probe and the waves experiments. This project is implemented using Xilinx, C and Python. This project contains all the tools developed to test during pre flight and vibration testing as well as flight ready software for the final flight.

## My contributions

- Designed a custom FPGA image to fulfill the mission requirements
	- Antenna deployment
	- Langmuir probe experiment (DAC and ADC)
	- Waves - experimental RF payload to generate broadband RF noise during flight 0-30 MHz and save spectra for post processing after flight
- Performed software verification and testing
- Wrote testing scripts in python to test the payload operation and deployable during testing
- Developed final flight software 


## Demo/Testing 

### Langmuir probe sweeps 

![](docs/bin/Pasted%20image%2020260715201335.png)

Figure shows the Lp sweep generated to drive the probe 
### 0-30 MHz RF generation

![](docs/bin/Pasted%20image%2020260715201440.png)

Figure shows measured RF pulses in the waterfall measured from an external SDR


