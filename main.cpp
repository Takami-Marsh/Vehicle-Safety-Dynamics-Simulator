#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;
typedef long double ld;

class VehicleParameters
{
public:
  ld height;      // Vehicle height (m)
  ld length;      // Vehicle length (m)
  ld width;       // Vehicle width (m)
  ld wheelbase;   // Distance between wheel contact points (m)
  ld wheelWidth;  // Wheel width (m)
  ld mass;        // Vehicle mass (kg)
  ld curveRadius; // Curve radius (m)

  // Environmental parameters
  ld airPressure;   // Air pressure (Pa)
  ld temperature;   // Temperature (K)
  ld windVelocity;  // Wind velocity (m/s)
  ld windAngle;     // Wind angle (rad)
  ld frictionCoeff; // Coefficient of static friction

  // Physical constants
  static constexpr ld GRAVITY = 9.80665;
  static constexpr ld GAS_CONSTANT = 287.05;
  static constexpr ld DRAG_COEFFICIENT = 0.525;

  VehicleParameters()
  {
    // Initialize with worst-case scenario values for Japan
    height = 3.8;
    length = 12.0;
    width = 2.39;
    wheelbase = 9.0;
    wheelWidth = 0.3;
    mass = 12500.0;
    curveRadius = 30.0;
    airPressure = 101330.0;
    temperature = 313.0;
    windVelocity = 25.0;
    windAngle = M_PI / 2.0;
    frictionCoeff = 0.4;
  }
};

class SafeSpeedCalculator
{
private:
  VehicleParameters params;
  ld theta; // Tire angle
  ld alpha; // Support point angle

  ld calculatePhi(ld velocity, ld time) const
  {
    return (velocity * time) / params.curveRadius;
  }

  ld calculateRelativeVelocityVertical(ld velocity, ld time) const
  {
    return velocity * cos(calculatePhi(velocity, time) + theta) + params.windVelocity * cos(params.windAngle);
  }

  ld calculateRelativeVelocityHorizontal(ld velocity, ld time) const
  {
    return velocity * sin(calculatePhi(velocity, time) + theta) + params.windVelocity * sin(params.windAngle);
  }

  ld calculateRelativeVelocity(ld vVert, ld vHoriz) const
  {
    return sqrt(pow(vVert, 2) + pow(vHoriz, 2));
  }

  ld calculateLambda(ld vHoriz, ld vVert) const
  {
    return atan2(vHoriz, vVert);
  }

  ld calculateCentripetalForce(ld velocity) const
  {
    return (params.mass * pow(velocity, 2)) / (params.curveRadius + params.width);
  }

  ld calculateModifiedCentripetalForce(ld centripetalForce) const
  {
    return centripetalForce * cos(alpha);
  }

  ld calculateCrossSectionalArea(ld lambda, ld phi) const
  {
    return params.height * (fabs(params.width * cos(lambda - phi)) + fabs(params.length * sin(lambda - phi)));
  }

  ld calculateAerodynamicForce(ld area, ld relativeVelocity) const
  {
    return (params.DRAG_COEFFICIENT * params.airPressure * area * pow(relativeVelocity, 2)) / (params.GAS_CONSTANT * params.temperature);
  }

  bool checkMaximumLimits(ld velocity)
  {
    theta = fabs(asin(params.wheelbase / (2 * params.curveRadius)));
    alpha = atan((params.wheelWidth * cos(theta)) / (params.wheelbase * (1 + (0.5 * sin(theta)))));

    ld timeMax = (params.curveRadius * (M_PI / 2 - theta)) / velocity;
    ld timeStep = timeMax / 10000;
    ld maxTorque = 0, maxForce = 0;

    ld centripetalForce = calculateCentripetalForce(velocity);
    ld modifiedCentripetalForce = calculateModifiedCentripetalForce(centripetalForce);

    for (ld time = 0; time < timeMax; time += timeStep)
    {
      ld vHoriz = calculateRelativeVelocityHorizontal(velocity, time);
      ld vVert = calculateRelativeVelocityVertical(velocity, time);
      ld relVel = calculateRelativeVelocity(vVert, vHoriz);
      ld lambda = calculateLambda(vHoriz, vVert);
      ld phi = calculatePhi(velocity, time);
      ld area = calculateCrossSectionalArea(lambda, phi);
      ld aeroForce = calculateAerodynamicForce(area, relVel);

      // Calculate forces on front and rear tires
      ld frontForce = centripetalForce + (aeroForce * sin(lambda - phi - theta));
      ld rearForce = (centripetalForce * cos(theta)) + (aeroForce * sin(lambda - phi));

      // Calculate torque
      ld torque = fabs((params.height / 2) * (aeroForce * sin(lambda - phi - alpha) + modifiedCentripetalForce));

      maxForce = max(maxForce, max(frontForce, rearForce));
      maxTorque = max(maxTorque, torque);
    }

    ld weightTorque = ((params.mass * params.GRAVITY * (params.width + (params.wheelWidth * cos(theta)))) / 2);
    ld frictionLimit = params.mass * params.GRAVITY * params.frictionCoeff;

    return maxTorque >= weightTorque || maxForce >= frictionLimit;
  }

public:
  SafeSpeedCalculator(const VehicleParameters &parameters) : params(parameters) {}

  ld calculateMaximumSafeSpeed()
  {
    ld left = 0, right = 10000, mid;
    const ld EPSILON = 1e-9;

    while (right - left > EPSILON)
    {
      mid = (left + right) / 2;
      if (checkMaximumLimits(mid))
      {
        right = mid;
      }
      else
      {
        left = mid;
      }
    }

    return checkMaximumLimits(left) ? -1 : left;
  }

  void setRadius(ld radius)
  {
    params.curveRadius = radius;
  }
};

int main()
{
  VehicleParameters params;
  SafeSpeedCalculator calculator(params);

  vector<ld> testRadii = {15, 30, 50, 80, 120, 230, 380, 570};

  cout << "Radius (m) | Max Safe Speed (km/h)\n";
  cout << "-----------|-----------------\n";

  for (ld radius : testRadii)
  {
    calculator.setRadius(radius);
    ld maxSpeed = calculator.calculateMaximumSafeSpeed();

    if (maxSpeed == -1)
    {
      cout << setw(10) << radius << " | No solution\n";
      continue;
    }

    cout << setw(10) << fixed << setprecision(0) << radius << " | " << fixed << setprecision(2) << maxSpeed * 3.6 << "\n";
  }

  return 0;
}