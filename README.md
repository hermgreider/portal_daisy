# portal_daisy
Daisy application for the portal


### To clone and build libDaisy and DaisySP:

git clone --recurse-submodules https://github.com/hermgreider/portal_daisy.git
./rebuild_all.sh

### To run the Osc program
First, connect the Daisy Seed to the computer. Press Boot, then Reset to put in DFU mode.
```
cd src/Osc
make clean; make; make program-dfu
```

