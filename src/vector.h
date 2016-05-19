#ifndef VECTOR_H_
#define VECTOR_H_

#include <random>
#include <string>
#include <sstream>

template <typename T>
T lerp(T a, T b, double factor) {
	return a * (1 - factor) + (b * factor);
}

template <class T>
class Vector3
{
public:
	Vector3()
	{
		m_v[0] = T();
		m_v[1] = T();
		m_v[2] = T();
	}

	Vector3(T value)
	{
		m_v[0] = value;
		m_v[1] = value;
		m_v[2] = value;
	}

	Vector3(T x, T y, T z)
	{
		m_v[0] = x;
		m_v[1] = y;
		m_v[2] = z;
	}

	Vector3(const Vector3 &vector)
	{
		m_v[0] = vector.m_v[0];
		m_v[1] = vector.m_v[1];
		m_v[2] = vector.m_v[2];
	}

	T& x() {
		return m_v[0];
	}

	T& y() {
		return m_v[1];
	}

	T& z() {
		return m_v[2];
	}

	T x() const {
		return m_v[0];
	}

	T y() const {
		return m_v[1];
	}

	T z() const {
		return m_v[2];
	}

	Vector3 &operator=(const Vector3 &other) {
		m_v[0] = other.m_v[0];
		m_v[1] = other.m_v[1];
		m_v[2] = other.m_v[2];

		return *this;
	}

	bool operator==(const Vector3 &other) const {
		return m_v[0] == other.m_v[0] && m_v[1] == other.m_v[1] && m_v[2] == other.m_v[2];
	}

	bool operator!=(const Vector3 &other) const {
		return !(*this == other);
	}

	Vector3 operator-(const Vector3 &other) const {
		return Vector3(m_v[0] - other.m_v[0], m_v[1] - other.m_v[1], m_v[2] - other.m_v[2]);
	}

	Vector3 operator+(const Vector3 &other) const {
		return Vector3(m_v[0] + other.m_v[0], m_v[1] + other.m_v[1], m_v[2] + other.m_v[2]);
	}

	Vector3 operator*(const Vector3 &other) const {
		return Vector3(m_v[0] * other.m_v[0], m_v[1] * other.m_v[1], m_v[2] * other.m_v[2]);
	}

	Vector3 operator*(const double f) const {
		return Vector3(m_v[0] * f, m_v[1] * f, m_v[2] * f);
	}

	Vector3 operator/(const double f) const {
		return Vector3(m_v[0] / f, m_v[1] / f, m_v[2] / f);
	}

	Vector3 operator-(const double f) const {
		return Vector3(m_v[0] - f, m_v[1] - f, m_v[2] - f);
	}

	Vector3 operator+(const double f) const {
		return Vector3(m_v[0] + f, m_v[1] + f, m_v[2] + f);
	}

	Vector3 &operator+=(const Vector3 &other) {
		m_v[0] += other.m_v[0];
		m_v[1] += other.m_v[1];
		m_v[2] += other.m_v[2];

		return *this;
	}

	double length() const {
		return sqrt(pow(m_v[0], 2) + pow(m_v[1], 2) + pow(m_v[2], 2));
	}

	double length_squared() const {
		return pow(m_v[0], 2) + pow(m_v[1], 2) + pow(m_v[2], 2);
	}

	void normalize() {
		double len = length();
		m_v[0] /= len;
		m_v[1] /= len;
		m_v[2] /= len;
	}

	Vector3 normal_component(const Vector3 &n) const {
		return n * (dot(n));
	}

	Vector3 tangential_component(const Vector3 &n, const Vector3 &normal_component) const {
		return *this - normal_component;
	}

	Vector3 cross(const Vector3 &other) const {
		return Vector3(y() * other.z() - z() * other.y(),
			z() * other.x() - x() * other.z(),
			x() * other.y() - y() * other.x());
	}

	double dot(const Vector3 &other) const {
		return x() * other.x() + y() * other.y() + z() * other.z();
	}

	bool is_zero() const {
		return !m_v[0] && !m_v[1] && !m_v[2];
	}

	void clamp(const Vector3 &min, const Vector3 &max) {
		x() = min.x() < x() ? x() : min.x();
		y() = min.y() < y() ? y() : min.y();
		z() = min.z() < z() ? z() : min.z();

		x() = max.x() > x() ? x() : max.x();
		y() = max.y() > y() ? y() : max.y();
		z() = max.z() > z() ? z() : max.z();
	}

	void max_vector(const Vector3 &other)
	{
		if(other.x() > x())
			x() = other.x();
		if(other.y() > y())
			y() = other.y();
		if(other.z() > z())
			z() = other.z();
	}

	void min_vector(const Vector3 &other)
	{
		if(other.x() < x())
			x() = other.x();
		if(other.y() < y())
			y() = other.y();
		if(other.z() < z())
			z() = other.z();
	}

	void divide(double factor) {
		m_v[0] /= factor;
		m_v[1] /= factor;
		m_v[2] /= factor;
	}

	void multiply(double factor) {
		m_v[0] *= factor;
		m_v[1] *= factor;
		m_v[2] *= factor;
	}

	void flip() {
		m_v[0] = -m_v[0];
		m_v[1] = -m_v[1];
		m_v[2] = -m_v[2];
	}


	static Vector3<T> lerp(const Vector3 &v1, const Vector3 &v2, double factor) {
		return Vector3<T>(::lerp(v1.x(), v2.x(), factor), ::lerp(v1.y(), v2.y(), factor), ::lerp(v1.z(), v2.z(), factor));
	}
	
	static Vector3 rand_unit_in_hemisphere(const Vector3 &normal, std::mt19937 &random)
	{
		std::uniform_real_distribution<> distribution(0, 1);

		double mt_x = (distribution(random)) - 0.5;
		double mt_y = (distribution(random)) - 0.5;
		double mt_z = (distribution(random)) - 0.5;

		Vector3 vector(mt_x, mt_y, mt_z);
		vector.normalize();
		if(vector.dot(normal) < 0)
			vector.flip();

		return vector;
	}

	std::string to_string() const {
		std::ostringstream ss;
		ss << "[" << x() << ", " << y() << "," << z() << "]";
		return ss.str();
	}
private:
	T m_v[3];
};

template <class T>
class Vector2
{
public:
	Vector2() {
		m_v[0] = T();
		m_v[1] = T();
	}

	Vector2(T x, T y) {
		m_v[0] = x;
		m_v[1] = y;
	}

	T& x() {
		return m_v[0];
	}

	T& y() {
		return m_v[1];
	}

	T x() const {
		return m_v[0];
	}

	T y() const {
		return m_v[1];
	}

	bool operator!=(const Vector2 &other) const {
		return m_v[0] != other.m_v[0] || m_v[1] != other.m_v[1] || m_v[2] != other.m_v[2];
	}

	Vector2 operator-(const Vector2 &other) const {
		return Vector2(m_v[0] - other.m_v[0], m_v[1] - other.m_v[1]);
	}

	Vector2 operator+(const Vector2 &other) const {
		return Vector2(m_v[0] + other.m_v[0], m_v[1] + other.m_v[1]);
	}

	Vector2 operator*(const double f) const {
		return Vector2(m_v[0] * f, m_v[1] * f);
	}

	double length() const {
		return sqrt(pow(m_v[0], 2) + pow(m_v[1], 2));
	}

	void normalize() {
		double len = length();
		m_v[0] /= len;
		m_v[1] /= len;
	}

	std::string to_string() const {
		std::ostringstream ss;
		ss << "[" << x() << ", " << y() << "]";
		return ss.str();
	}
private:
	T m_v[2];
};

typedef Vector2<int> Vector2i;
typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;
typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;

typedef Vector3<float> Color;

#endif