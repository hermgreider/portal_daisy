### Osc application

Simple test program to verify your setup. 

### To run the Osc program
First, connect the Daisy Seed to the computer. Press Boot, then Reset to put in DFU mode.
```
cd src/Osc
make clean; make; make program-dfu
```

Should produce a drone sound on middle C.