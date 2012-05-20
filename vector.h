#pragma once

#include <math.h>

#include <string>
#include <sstream>
#include <ostream>

using std::string;
using std::stringstream;
using std::ostream;

#ifndef _WIN32
inline int abs(int x)
{
	return abs(x);
}
#endif

namespace am {
namespace math {

	template <class T>
	class Vector {
	public:
		Vector() : x(0), y(0), z(0), w(1) {
		}
		Vector(T x, T y, T z) :x(x), y(y), z(z), w(1) {
		}
		Vector(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {
		}
		~Vector() {}
	
		friend ostream& operator << (ostream &o, const Vector<T> &v) {
			return o << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
	}
	
		T x;
		T y;
		T z;
		T w;
	
		inline Vector<T> add(const Vector<T> &rhs) const {
			return Vector<T>(x + rhs.x, y + rhs.y, z + rhs.z, 1);
		}
		inline Vector<T> add(T ix, T iy, T iz) const {
			return Vector<T>(x + ix, y + iy, z + iz, 1);
		}
		inline Vector<T> &addTo(const T &ix, const T &iy, const T &iz) {
			x += ix;
			y += iy;
			z += iz;
			return *this;
		}
		inline Vector<T> &addTo(const Vector<T> &rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		inline Vector<T> sub(const Vector<T> &rhs) const {
			return Vector<T>(x - rhs.x, y - rhs.y, z - rhs.z, 1);
		}
		inline Vector<T> sub(T ix, T iy, T iz) const {
			return Vector<T>(x - ix, y - iy, z - iz, 1);
		}
		inline Vector<T> &subFrom(const T &ix, const T &iy, const T &iz) {
			x -= ix;
			y -= iy;
			z -= iz;
			return *this;
		}
		inline Vector<T> &subFrom(const Vector<T> &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		operator string() const
		{
			stringstream ss;
			ss << x << ", " << y << ", " << z;
			return ss.str();
		}

		operator Vector<float>() const
		{
			return Vector<float>(static_cast<float>(x),
				static_cast<float>(y),
				static_cast<float>(z),
				static_cast<float>(w));
		}

		operator Vector<double>() const
		{
			return Vector<double>(static_cast<double>(x),
				static_cast<double>(y),
				static_cast<double>(z),
				static_cast<double>(w));
		}

		operator Vector<long double>() const
		{
			return Vector<long double>(static_cast<long double>(x),
				static_cast<long double>(y),
				static_cast<long double>(z),
				static_cast<long double>(w));
		}

		template <class F>
		operator Vector<F>() const
		{
			return Vector<F>(static_cast<F>(round(x)),
				static_cast<F>(round(y)),
				static_cast<F>(round(z)),
				static_cast<F>(round(w)));
		}

		inline Vector<T> scale(double factor) const
		{
			return Vector<T>(static_cast<T>(x * factor),
				static_cast<T>(y * factor),
				static_cast<T>(z * factor), 
				1);
		}
		inline Vector<T> &scaleBy(double factor)
		{
			x *= static_cast<float>(factor);
			y *= static_cast<float>(factor);
			z *= static_cast<float>(factor);
			return *this;
		}

		inline Vector<T> multiply(const Vector<T> &rhs) const {
			return Vector<T>(x * rhs.x, y * rhs.y, z * rhs.z);
		}

		inline Vector<T> &normalise()
		{
			double len = length();
			if(len != 0.0)
			{
				double resp = 1.0 / len;
				x = static_cast<T>(x * resp);
				y = static_cast<T>(y * resp);
				z = static_cast<T>(z * resp);
				w = 1; 
			}

			return *this;
		}

		inline bool equals(const Vector<T> &rhs, double tolerance = 0)
		const {
			if(tolerance > 0.0)
			{
				double dx = static_cast<double>(rhs.x - x);
				double dy = static_cast<double>(rhs.y - y);
				double dz = static_cast<double>(rhs.z - z);
				return dx >= -tolerance && dx <= tolerance &&
					dy >= -tolerance && dy <= tolerance &&
					dz >= -tolerance && dz <= tolerance;
			}
			else
			{
				return rhs.x == x && rhs.y == y && rhs.z == z;
			}
		}

		inline double length() const { 
			return sqrt(static_cast<double>(x * x + y * y + z * z)); 
		}
		inline double lengthSqrd() const { 
			return static_cast<double>(x * x + y * y + z * z); 
		}
		inline double dot(const Vector<T> &rhs) const { 
			return static_cast<double>(x * rhs.x + y * rhs.y + z * rhs.z); 
		}
		inline Vector<T> cross(const Vector<T> &rhs) const {
			return Vector<T>(y * rhs.z - z * rhs.y, 
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
		}
		inline double distance(const Vector<T> &rhs) const {
			double dx = x - rhs.x;
			double dy = y - rhs.y;
			double dz = z - rhs.z;
			return sqrt(dx * dx + dy * dy + dz * dz);
		}

		inline bool operator == (const Vector<T> &rhs) {
			return equals(rhs);
		}
		inline bool operator != (const Vector<T> &rhs) {
			return !equals(rhs);
		}
		inline Vector<T> &operator =(const Vector<T> &rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		inline Vector<T> &operator =(const float &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector<T> &operator =(const double &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector<T> &operator =(const long double &rhs) const {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector<T> &operator =(const int &rhs)
		{
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}
		inline bool operator ==(const Vector<T> &rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
	};

	typedef Vector<float> Vector4f;
	typedef Vector<double> Vector4d;
	typedef Vector<int> Vector4i;

}
}
