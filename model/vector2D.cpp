#include "vector2D.h"

using namespace std;

Vector::Vector() {
	x = 0;
	y = 0;
}

Vector::Vector(double x, double y) {
	this->x = x;
	this->y = y;
}

Vector::Vector(const Vector& vector) {
	x = vector.x;
	y = vector.y;
}

Vector::~Vector() {
}

void Vector::setX(double x) {
	this->x = x;
}

void Vector::setY(double y) {
	this->y = y;
}

 double Vector::getX() const {
	return x;
}

double Vector::getY() const {
	return y;
}

Vector Vector::operator+(const Vector& vector) {
	return Vector(x + vector.x, y + vector.y);
}

Vector Vector::operator+=(const Vector& vector) {
		x += vector.x;
		y += vector.y;
	return *this;
}

Vector Vector::operator-() {
	return Vector(-x, -y);
}

Vector Vector::operator-(const Vector& vector) const {
	return Vector(x - vector.x, y - vector.y);
}

Vector Vector::operator*(double scalar) {
	return Vector(x * scalar, y * scalar);
}

Vector operator*(double scalar, const Vector& vector) {
	return Vector(scalar * vector.x, scalar * vector.y);
}

Vector Vector::operator=(const Vector& vector) {
	x = vector.x;
	y = vector.y;

	return *this;
}

bool Vector::operator==(const Vector& vector) const{
	return x == vector.x && y == vector.y;
}

double Vector::dotProduct(const Vector& left, const Vector& right) {
	return left.x * right.x + left.y * right.y;
}

double Vector::norm(const Vector& v) {
	return sqrt(dotProduct(v, v));
}

ostream& operator<<(ostream& out, const Vector& vector) {
	out << "(" << vector.x << ", " << vector.y << ")" << std::endl;
	return out;
}


ofstream& operator<<(ofstream& out, const Vector& vector) {
	out  << vector.x << " " << vector.y << " ";
	return out;
}

ifstream& operator>>(std::ifstream& in, Vector& vector) {
	in >> vector.x;
	in >> vector.y;
	return in;
}
