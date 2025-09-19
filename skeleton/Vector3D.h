#pragma once
#include <cmath>

class Vector3D
{
	double X, Y, Z;
	double m;

	Vector3D() = default;
	Vector3D(double x, double y, double z) 
		: X(x), Y(y), Z(z) 
	{
		m = module();
	}

	// getters
	double getX() const { return X; }
	double getY() const { return Y; }
	double getZ() const { return Z; }
	double getModule() const { return m; }

	// normalizar
	void normalize() 
	{
		X = X / m;
		Y = Y / m;
		Z = Z / m;
		m = module();
	}

	// obtener modulo
	double module() 
	{
		// vec / norm = mod
		normalize();
	}

	// --- operadores
	void operator*(Vector3D& vec) 
	{
		X * vec.getX() + Y * vec.getY() + Z * vec.getZ();
	}

	void operator*(double e) 
	{
		X *= e;
		Y *= e;
		Z *= e;
	}

	void operator+() 
	{

	}

	void operator-() 
	{

	}

	void operator=() 
	{

	}
};

