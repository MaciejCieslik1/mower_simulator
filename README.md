# mower_simulator
Authors: *Hanna Biegacz, Maciej Cieślik*

## Running the Simulation
In order to start the mower simulator, run:
```
mkdir build/
cd build/
cmake ..
make 
./mower_simulator
```

## Prototype stage - 01.12.2025
Implemented features:
- **Compile-time constants**: set in /include/Constants.h (like lawn size limits, precision, PI). These are values that must be set before running the program.
- **Runtime configuration**: Config.cpp figures out mower and blade size based on the lawn's actual size every time the program starts.
- **Lawn and Point**: Lawn class with all its methods, plus a simple Point struct.
- **SimulationEngine**: runs the simulation in its own thread. At this moment it also artificialy 'mows' the lawn, so that there is anything changing in the simulation window.
- **LawnSimulationView**: window shows the current state of the Lawn class (green = unmowed, brown = mowed).
- **Unit tests** covering implemented functionalities (tests/ directory).
- **Main.cc** creates the lawn and starts the simulation thread.