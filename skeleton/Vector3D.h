#pragma once
#include <cmath>

class Vector3D
{
	double X, Y, Z;
	double m;

public:
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
		this->X = X / m;
		this->Y = Y / m;
		this->Z = Z / m;
		this->m = module();
	}

	// obtener modulo
	double module() 
	{
		// vec / norm = mod
		//normalize();
		return sqrt(pow(2.0, this->X) + pow(2.0, this->Y) + pow(2.0, this->Z));
	}

	// --- operadores
	Vector3D& operator*(Vector3D& vec)
	{
		this->X * vec.getX() + this->Y * vec.getY() + this->Z * vec.getZ();
		return *this;
	}

	Vector3D& operator*(double e)
	{
		this->X *= e;
		this->Y *= e;
		this->Z *= e;
	}

	// suma de vectores
	Vector3D& operator+(Vector3D& vec)
	{
		this->X += vec.getX();
		this->Y += vec.getY();
		this->Z += vec.getZ();
		this->m = module();

		return *this;
	}

	// resta de vectores
	Vector3D& operator-(Vector3D& vec)
	{
		this->X -= vec.getX();
		this->Y -= vec.getY();
		this->Z -= vec.getZ();
		this->m = module();

		return *this;
	}

	// asigancion
	Vector3D& operator=(Vector3D const& vec)
	{
		this->X = vec.getX();
		this->Y = vec.getY();
		this->Z = vec.getZ();
		this->m = vec.getModule();

		return *this;
	}
};