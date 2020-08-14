#pragma once
#include <iostream>
#include <fstream>
#include <cmath>

class Vector {
public:
	Vector();
	Vector(double, double);
	Vector(const Vector&); //а надо ли оно?
	~Vector();

	void setX(double x);
	void setY(double y);
	double getX() const;
	double getY() const;

	Vector operator+(const Vector&);
	Vector operator+=(const Vector&);
	Vector operator-();
	Vector operator-(const Vector&) const;
	Vector operator*(double);
	friend Vector operator* (double, const Vector&);
	Vector operator=(const Vector&);
	bool operator==(const Vector&) const;

	static double dotProduct(const Vector&, const Vector&);
	static double norm(const Vector&);

	friend std::ostream& operator<<(std::ostream&, const Vector&);
	friend std::ofstream& operator<<(std::ofstream&, const Vector&);
	friend std::ifstream& operator>>(std::ifstream&, Vector&);
private:
	double x;
	double y;
};