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
	class Vector4 {
	public:
		Vector4() : x(0), y(0), z(0), w(1) {
		}
		Vector4(T x, T y, T z) :x(x), y(y), z(z), w(1) {
		}
		Vector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {
		}
		~Vector4() {}
	
		friend ostream& operator << (ostream &o, const Vector4<T> &v) {
			return o << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
	}
	
		T x;
		T y;
		T z;
		T w;
	
		inline Vector4<T> add(const Vector4<T> &rhs) const {
			return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, 1);
		}
		inline Vector4<T> add(T ix, T iy, T iz) const {
			return Vector4<T>(x + ix, y + iy, z + iz, 1);
		}
		inline Vector4<T> &addTo(const T &ix, const T &iy, const T &iz) {
			x += ix;
			y += iy;
			z += iz;
			return *this;
		}
		inline Vector4<T> &addTo(const Vector4<T> &rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		inline Vector4<T> sub(const Vector4<T> &rhs) const {
			return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, 1);
		}
		inline Vector4<T> sub(T ix, T iy, T iz) const {
			return Vector4<T>(x - ix, y - iy, z - iz, 1);
		}
		inline Vector4<T> &subFrom(const T &ix, const T &iy, const T &iz) {
			x -= ix;
			y -= iy;
			z -= iz;
			return *this;
		}
		inline Vector4<T> &subFrom(const Vector4<T> &rhs) {
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

		operator Vector4<float>() const
		{
			return Vector4<float>(static_cast<float>(x),
				static_cast<float>(y),
				static_cast<float>(z),
				static_cast<float>(w));
		}

		operator Vector4<double>() const
		{
			return Vector4<double>(static_cast<double>(x),
				static_cast<double>(y),
				static_cast<double>(z),
				static_cast<double>(w));
		}

		operator Vector4<long double>() const
		{
			return Vector4<long double>(static_cast<long double>(x),
				static_cast<long double>(y),
				static_cast<long double>(z),
				static_cast<long double>(w));
		}

		template <class F>
		operator Vector4<F>() const
		{
			return Vector4<F>(static_cast<F>(round(x)),
				static_cast<F>(round(y)),
				static_cast<F>(round(z)),
				static_cast<F>(round(w)));
		}

		inline Vector4<T> scale(double factor) const
		{
			return Vector4<T>(static_cast<T>(x * factor),
				static_cast<T>(y * factor),
				static_cast<T>(z * factor), 
				1);
		}
		inline Vector4<T> &scaleBy(double factor)
		{
			x *= static_cast<float>(factor);
			y *= static_cast<float>(factor);
			z *= static_cast<float>(factor);
			return *this;
		}

		inline Vector4<T> multiply(const Vector4<T> &rhs) const 
		{
			return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z);
		}

		inline Vector4<T> &normalise()
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

		inline bool equals(const Vector4<T> &rhs, double tolerance = 0)	const 
		{
			if (tolerance > 0.0)
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
		inline double dot(const Vector4<T> &rhs) const { 
			return static_cast<double>(x * rhs.x + y * rhs.y + z * rhs.z); 
		}
		inline Vector4<T> cross(const Vector4<T> &rhs) const {
			return Vector4<T>(y * rhs.z - z * rhs.y, 
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
		}
		inline double distance(const Vector4<T> &rhs) const {
			double dx = x - rhs.x;
			double dy = y - rhs.y;
			double dz = z - rhs.z;
			return sqrt(dx * dx + dy * dy + dz * dz);
		}

		inline bool operator == (const Vector4<T> &rhs) {
			return equals(rhs);
		}
		inline bool operator != (const Vector4<T> &rhs) {
			return !equals(rhs);
		}
		inline Vector4<T> &operator =(const Vector4<T> &rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		inline Vector4<T> &operator =(const float &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector4<T> &operator =(const double &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector4<T> &operator =(const long double &rhs) const {
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}

		inline Vector4<T> &operator =(const int &rhs)
		{
			x = (T)rhs;
			y = (T)rhs;
			z = (T)rhs;
			w = (T)rhs;
			return *this;
		}
		inline bool operator ==(const Vector4<T> &rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
	};

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<int> Vector4i;

	template <class T>
	class Vector2 {
	public:
		Vector2() : x(0), y(0) {
		}
		Vector2(T x, T y) :x(x), y(y) {
		}
		~Vector2() {}
	
		friend ostream& operator << (ostream &o, const Vector2<T> &v) {
			return o << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
		}
	
		T x;
		T y;

		inline Vector2<T> add(const Vector2<T> &rhs) const {
			return Vector2<T>(x + rhs.x, y + rhs.y);
		}
		inline Vector2<T> add(T ix, T iy) const {
			return Vector2<T>(x + ix, y + iy);
		}
		inline Vector2<T> &addTo(const T &ix, const T &iy) {
			x += ix;
			y += iy;
			return *this;
		}
		inline Vector2<T> &addTo(const Vector2<T> &rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		inline Vector2<T> sub(const Vector2<T> &rhs) const {
			return Vector2<T>(x - rhs.x, y - rhs.y);
		}
		inline Vector2<T> sub(T ix, T iy) const {
			return Vector2<T>(x - ix, y - iy);
		}
		inline Vector2<T> &subFrom(const T &ix, const T &iy) {
			x -= ix;
			y -= iy;
			return *this;
		}
		inline Vector2<T> &subFrom(const Vector2<T> &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		operator string() const
		{
			stringstream ss;
			ss << x << ", " << y;
			return ss.str();
		}

		operator Vector2<float>() const
		{
			return Vector2<float>(static_cast<float>(x),
				static_cast<float>(y));
		}

		operator Vector2<double>() const
		{
			return Vector2<double>(static_cast<double>(x),
				static_cast<double>(y));
		}

		operator Vector2<long double>() const
		{
			return Vector2<long double>(static_cast<long double>(x),
				static_cast<long double>(y));
		}

		template <class F>
		operator Vector2<F>() const
		{
			return Vector2<F>(static_cast<F>(round(x)),
				static_cast<F>(round(y)));
		}

		inline Vector2<T> scale(double factor) const
		{
			return Vector2<T>(static_cast<T>(x * factor),
				static_cast<T>(y * factor));
		}
		inline Vector2<T> &scaleBy(double factor)
		{
			x *= static_cast<float>(factor);
			y *= static_cast<float>(factor);
			return *this;
		}

		inline Vector2<T> multiply(const Vector2<T> &rhs) const {
			return Vector2<T>(x * rhs.x, y * rhs.y);
		}

		inline Vector2<T> &normalise()
		{
			double len = length();
			if(len != 0.0)
			{
				double resp = 1.0 / len;
				x = static_cast<T>(x * resp);
				y = static_cast<T>(y * resp);
			}

			return *this;
		}

		inline bool equals(const Vector2<T> &rhs, double tolerance = 0) const 
		{
			if (tolerance > 0.0)
			{
				double dx = static_cast<double>(rhs.x - x);
				double dy = static_cast<double>(rhs.y - y);
				return dx >= -tolerance && dx <= tolerance &&
					dy >= -tolerance && dy <= tolerance;
			}
			else
			{
				return rhs.x == x && rhs.y == y;
			}
		}

		inline double length() const { 
			return sqrt(static_cast<double>(x * x + y * y)); 
		}
		template <class F>
		F length() const {
			return static_cast<F>(length());
		}
		inline double lengthSqrd() const { 
			return static_cast<double>(x * x + y * y);
		}
		template <class F>
		F lengthSqrd() const {
			return static_cast<F>(lengthSqrd());
		}
		inline double dot(const Vector2<T> &rhs) const { 
			return static_cast<double>(x * rhs.x + y * rhs.y); 
		}
		inline double distance(const Vector2<T> &rhs) const {
			double dx = x - rhs.x;
			double dy = y - rhs.y;
			return sqrt(dx * dx + dy * dy);
		}

		inline bool operator == (const Vector2<T> &rhs) {
			return equals(rhs);
		}
		inline bool operator != (const Vector2<T> &rhs) {
			return !equals(rhs);
		}
		inline Vector2<T> &operator =(const Vector2<T> &rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		inline Vector2<T> &operator =(const float &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			return *this;
		}

		inline Vector2<T> &operator =(const double &rhs) {
			x = (T)rhs;
			y = (T)rhs;
			return *this;
		}

		inline Vector2<T> &operator =(const long double &rhs) const {
			x = (T)rhs;
			y = (T)rhs;
			return *this;
		}

		inline Vector2<T> &operator =(const int &rhs)
		{
			x = (T)rhs;
			y = (T)rhs;
			return *this;
		}

		inline bool operator ==(const Vector2<T> &rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Vector2i;

}
}
