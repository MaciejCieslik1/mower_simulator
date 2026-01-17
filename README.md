# mower_simulator
Authors: *Hanna Biegacz, Maciej Cieślik*

## Customizing mower movements
The mower movements can be customized by modifying the `Main.cc` file. There is a special method called '' in which the user can define the mower's movements using the 'controller' object.

Aviable commands:
- `move(double cm)`
- `rotate(short deg)`
- `setMowing(bool enable)`
- `addPoint(double x, double y)`
- `deletePoint(unsigned int id)`
- `moveToPoint(unsigned int point_id)`
- `rotateTowardsPoint(unsigned int point_id)`
- `getDistanceToPoint(unsigned int point_id, double& out_distance)`

Users are also able to customize other simulation parameters, such as the mower's speed and dimensions, as well as the lawn's dimensions.
Another thing that can be customized is the overall simulation speed.

## Running the Simulation
In order to start the mower simulator, run:
```
./setup_extra_libraries.sh
mkdir build/
cd build/
cmake ..
make 
./mower_simulator
```
After that you can run tests with the following command:
```
ctest
```
## Dependencies and necesary tools
- **Libraries**: Google Test, Qt5, pthread
- **Tools**: CMake, Make

For a quick setup, please verify the contents of `setup_extra_dependencies.sh` and run it.
