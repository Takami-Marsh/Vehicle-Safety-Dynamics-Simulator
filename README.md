# Vehicle Safety Dynamics Simulator: Maximum Safe Speed on Curves

This project simulates the maximum safe speed a vehicle can achieve on curves without slipping or losing contact with the road, considering forces such as torque, friction, and aerodynamic drag. The model is designed with worst-case scenarios in mind, ensuring that vehicles maintain stability in challenging road conditions, especially in Japan where unpredictable weather events often occur.

## Purpose

The objective of this project is to model the maximum safe speed a vehicle can travel through circular curves by computing whether the vehicle can navigate the curve safely based on physical parameters such as mass, wheelbase, aerodynamic drag, and more. The project is aimed at supporting the development of autonomous driving systems and improving their safety checks.

The algorithm calculates forces acting on the vehicle and checks if the speed is constrained by torque, friction, or aerodynamic drag. By iterating through various speed values, it identifies the maximum safe speed.

## Features

- Computes the maximum velocity a vehicle can safely travel without slipping on curves.
- Considers forces such as aerodynamic drag, centripetal force, torque, and friction.
- Supports a variety of vehicle and environmental parameters, including mass, wheelbase, and wind speed.
- Uses a binary search algorithm to determine the maximum safe velocity efficiently.

## How It Works

### Mathematical Model

The model uses classical mechanics and aerodynamic principles to simulate the forces acting on a vehicle as it navigates curves. Below are the core equations used in the model:

### 1. **Centripetal Force**:
The centripetal force $` F_c `$ keeps the vehicle moving along the curve:
```math
F_c = \frac{m \cdot v^2}{r + h}
```
Where:
- $` m `$ is the mass of the vehicle $`[ \text{kg} ]`$,
- $` v `$ is the velocity of the vehicle $`[ \text{m/s} ]`$,
- $` r `$ is the radius of the curve $`[ \text{m} ]`$,
- $` h `$ is the height of the vehicle's center of mass $`[ \text{m} ]`$.

This force is responsible for the circular motion of the vehicle and increases with speed or as the radius of the curve decreases.

### 2. **Aerodynamic Drag Force**:
The drag force $` F_{ad} `$ opposes the vehicle's motion through the air and is calculated as:
```math
F_{ad} = \frac{0.525 \cdot P \cdot A \cdot v'^2}{287.05 \cdot T}
```
Where:
- $` P `$ is the air pressure $`[ \text{Pa} ]`$,
- $` A `$ is the effective cross-sectional area of the vehicle $`[ \text{m}^2 ]`$,
- $` v' `$ is the vehicle’s relative velocity to the wind $`[ \text{m/s} ]`$,
- $` T `$ is the temperature $`[ \text{K} ]`$.

This force depends on the vehicle's shape, speed, and wind conditions.

### 3. **Torque**:
Torque plays a critical role in determining the vehicle's stability on a curve. The torque $` \tau `$ acting on the vehicle is computed as:
```math
\tau = \left(\frac{h}{2}\right) \cdot \left(F_{ad} \cdot \sin(\lambda - \phi - \alpha) + F_{c'}\right)
```
Where:
- $` h `$ is the height of the vehicle $`[ \text{m} ]`$,
- $` \lambda `$ is the wind angle relative to the vehicle $`[ \text{rad} ]`$,
- $` \phi `$ is the angle traveled along the curve $`[ \text{rad} ]`$,
- $` \alpha `$ is the angle between the vehicle's outer-most contact point and the ground $`[ \text{rad} ]`$,
- $` F_{c'} `$ is the adjusted centripetal force $`[ \text{N} ]`$.

If torque exceeds the stability threshold, the vehicle is at risk of rolling over.

### 4. **Static Friction**:
Friction between the tires and the road prevents the vehicle from slipping. The forces acting on the front and rear tires are calculated as:
```math
F_f = F_c + F_{ad} \cdot \sin(\lambda - \phi - \theta)
```
```math
F_r = F_c \cdot \cos(\theta) + F_{ad} \cdot \sin(\lambda - \phi)
```
Where:
- $` F_f `$ and $` F_r `$ are the forces acting on the front and rear tires, respectively $`[ \text{N} ]`$,
- $` \theta `$ is the tire angle $`[ \text{rad} ]`$.

The vehicle will start slipping when these forces exceed the maximum static friction, given by:
```math
F_{max} = \mu \cdot m \cdot g
```
Where:
- $` \mu `$ is the coefficient of static friction $`[ \text{unitless} ]`$,
- $` g `$ is the gravitational acceleration $`[ 9.81 \, \text{m/s}^2 ]`$.

### 5. **Cross-sectional Area Calculation**:
The vehicle’s cross-sectional area relative to the wind is:
```math
A = h \cdot \left( |w \cdot \cos(\lambda - \phi)| + |l \cdot \sin(\lambda - \phi)| \right)
```
Where:
- $` w `$ is the width of the vehicle $`[ \text{m} ]`$,
- $` l `$ is the length of the vehicle $`[ \text{m} ]`$.

### 6. **Binary Search Algorithm**:
The algorithm employs a binary search approach to find the maximum safe speed. The search progressively narrows the range of velocities, checking at each step if the computed forces remain below the safety thresholds.

### Algorithm Efficiency:
- **Time Complexity**: $` O(\log n) `$ — The binary search method ensures efficient computation, checking fewer speed values as the range narrows down exponentially.
- **Space Complexity**: $` O(1) `$ — Only a fixed amount of space is used, regardless of the problem size.

### Code Structure

1. **Velocity Calculation**:
   The code calculates the vertical and horizontal components of the vehicle's velocity relative to the wind.

2. **Torque and Friction Check**:
   It iterates through time steps to compute torque and friction forces. If either the maximum allowable torque or friction is exceeded, the vehicle is deemed unstable at that speed.

3. **Binary Search**:
   A binary search algorithm identifies the maximum stable speed by evaluating whether the vehicle maintains stability under given conditions at each speed increment.

## Example Usage

1. **Clone the Repository**:

```bash
git clone https://github.com/Takami-Marsh/Vehicle-Safety-Dynamics-Simulator.git
```

2. **Navigate to the Source Directory**:

```bash
cd vehicle-dynamics/src
```

3. **Compile the Code**:
Use a C++ compiler to compile the code:

```bash
g++ main.cpp -o vehicle_dynamics
```

4. **Run the Program**:

```bash
./vehicle_dynamics
```

The program will output the maximum safe speeds for different curve radii (in km/h).

## Parameters

The vehicle’s physical parameters are initialized in the main() function:

-	height: Height of the vehicle (in meters).
-	length: Length of the vehicle (in meters).
-	width: Width of the vehicle (in meters).
-	wheelbase: Distance between the front and rear wheels (in meters).
-	wheelWidth: Width of the wheels (in meters).
-	mass: Mass of the vehicle (in kilograms).
-	curveRadius: Radius of the curve (in meters).
-	airPressure: Atmospheric pressure (in Pascals).
-	airTemperature: Air temperature (in Kelvin).
-	windVelocity: Wind speed (in meters per second).
-	windDirection: Wind direction angle (in radians).
-	frictionCoefficient: Coefficient of static friction between tires and road.

## Legal Comparison

The model outputs a close match with Japan’s minimum curve radius and design speed limits when using values considering the worst-case scenario that could happen in Japan (which is the default parameters in the code):

| Curve Radius (m)    | Design Speed (km/h) | Model Output (km/h) |
| -------- | ------- | ------- |
| 15  | 20    | 23.16    |
| 30 | 30     | 31.36    |
| 50    | 40    | 39.83    |
| 80    | 50    | 49.82    |
| 120    | 60    | 60.50    |
| 230    | 80    | 82.52    |
| 380    | 100    | 104.57    |
| 570    | 120    | 126.27    |

<img width="476" alt="image" src="https://github.com/user-attachments/assets/b6d1525b-2b84-4be8-bdc7-b02b67ab78f9">

This shows a strong correlation (when plotting model's output vs design speed, y = 0.9531x + 0.7822 with R² = 0.998) between the model’s predictions and legal limits, confirming the model’s reliability.

## License

This project is licensed under the MIT License.
