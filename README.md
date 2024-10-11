# Vehicle Dynamics Simulator: Maximum Safe Speed on Curves

This project simulates the maximum safe speed a vehicle can achieve on curves without slipping or losing contact with the road, considering forces such as torque, friction, and aerodynamic drag.

## Purpose

This project models the maximum safe speed a vehicle can travel on circular curves, providing an algorithmic approach that calculates whether a vehicle can navigate a curve safely, based on physical parameters such as mass, wheelbase, drag force, and more.

The code simulates the behavior of vehicles traveling through curves of varying radii. It determines if the vehicle's maximum speed is constrained by torque and friction, using physical laws like centripetal force, aerodynamic drag, and frictional forces.

## Features

- Computes the maximum velocity a vehicle can safely travel without slipping on curves.
- Considers aerodynamic drag, centripetal force, torque, and friction.
- Configurable physical parameters such as vehicle mass, dimensions, and wheel radius.
- Supports different curve radii to test how these parameters affect the vehicle's stability.

## How It Works

### Mathematical Model

The vehicle's motion and safety constraints are modeled using the following key mathematical equations, derived from classical mechanics and aerodynamic principles.

### 1. **Centripetal Force**:
The centripetal force $`F_c`$ keeps the vehicle moving along the curve:
```math
F_c = \frac{m \cdot v^2}{r}
```
Where:
- $`m`$ is the mass of the vehicle.
- $`v`$ is the velocity of the vehicle.
- $`r`$ is the radius of the curve.

This force points towards the center of the curve and is responsible for the vehicle's circular motion.

### 2. **Aerodynamic Drag Force**:
The drag force $`F_{ad}`$ opposes the vehicle's motion through the air and is given by:
```math
F_{ad} = \frac{0.525 \cdot P \cdot A \cdot v'^2}{287.05 \cdot T}
```
Where:
- $`P`$ is the air pressure.
- $`A`$ is the effective cross-sectional area of the vehicle.
- $`v'`$ is the vehicle's relative velocity to the wind.
- $`T`$ is the temperature.

### 3. **Torque**:
Torque plays a role when the vehicle takes a curve, potentially causing the vehicle to roll over. The torque $` \tau `$ exerted on the vehicle is calculated as:
```math
\tau = \left(\frac{h}{2}\right) \cdot \left(F_{ad} \cdot \sin(\lambda - \phi - \alpha) + F_{c'}\right)
```
Where:
- $`h`$ is the height of the vehicle.
- $`\lambda`$ is the wind angle relative to the vehicle.
- $`\phi`$ is the rotation angle due to wheel movement.
- $`\alpha`$ is the body angle of the vehicle.
- $`F_{c'}`$ is the adjusted centripetal force.

If the torque exceeds a certain threshold, the vehicle risks losing stability and rolling over.

### 4. **Static Friction**:
Friction between the tires and the road prevents the vehicle from slipping. The forces acting perpendicular to the tires are calculated for the front and rear tires:
```math
F_f = F_c + F_{ad} \cdot \sin(\lambda - \phi - \theta)
```
```math
F_r = F_c \cdot \cos(\theta) + F_{ad} \cdot \sin(\lambda - \phi)
```
Where:
- $`F_f`$ is the force acting perpendicular to the front tires.
- $`F_r`$ is the force acting perpendicular to the rear tires.
- $`\theta`$ is the angle of the tires to the horizontal.
  
The vehicle will start slipping when these forces exceed the maximum static friction, calculated as:
```math
\mu \cdot m \cdot g
```
Where:
- $`\mu`$ is the coefficient of static friction.
- $`g`$ is the gravitational acceleration.

### 5. **Cross-sectional Area Calculation**:
The cross-sectional area of the vehicle relative to the wind is:
```math
A = h \cdot \left( |w \cdot \cos(\lambda - \phi)| + |l \cdot \sin(\lambda - \phi)| \right)
```
Where:
- $`w`$ is the width of the vehicle.
- $`l`$ is the length of the vehicle.

The aerodynamic drag force depends on this area as well as the relative velocity of the vehicle to the wind.

### Code Structure

1. **Velocity Calculation**:
   The code calculates the vertical and horizontal components of velocity relative to the wind, considering the vehicle's speed and wind direction.

2. **Torque and Friction Check**:
   It iterates through time steps to compute torque and friction forces acting on the vehicle. If either the maximum allowable torque or friction is exceeded, the vehicle is deemed unstable at that velocity.

3. **Binary Search**:
   A binary search algorithm is used to find the maximum velocity at which the vehicle can safely navigate the curve. The algorithm checks the stability at progressively higher speeds until it finds the maximum stable speed.

### Example Usage

1. **Clone the Repository**:

git clone https://github.com/Takami-Marsh/Vehicle-Safety-Dynamics-Simulator.git

2. **Navigate to the Source Directory**:

cd vehicle-dynamics/src

3. **Compile the Code**:
Use a C++ compiler to compile the code:

g++ main.cpp -o vehicle_dynamics

4. **Run the Program**:

./vehicle_dynamics

The program will output the maximum safe speeds for different wheel radii (in km/h).

### Parameters

The vehicle's physical parameters are initialized in the `main()` function:
- **height**: Height of the vehicle (in meters).
- **length**: Length of the vehicle (in meters).
- **width**: Width of the vehicle (in meters).
- **wheelbase**: Distance between the front and rear wheels (in meters).
- **wheelWidth**: Width of the wheels (in meters).
- **mass**: Mass of the vehicle (in kilograms).
- **curveRadius**: Radius of the curve (in meters).
- **airPressure**: Atmospheric pressure (in Pascals).
- **airTemperature**: Air temperature (in Kelvin).
- **windVelocity**: Wind speed (in meters per second).
- **windDirection**: Wind direction angle (in radians).
- **frictionCoefficient**: Coefficient of static friction between tires and road.

### License

This project is licensed under the MIT License.
