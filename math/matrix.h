#pragma once

#include <stdio.h>
#include <math.h>
#include "vector.h"

#ifndef M_PI
#	define M_PI 3.1415926535897932384626433832795
#	define M_PIF 3.1415926535897932384626433832795f
#	define M_PI2	M_PI * 2.0
#	define M_PIF2	M_PIF * 2.0f
#endif

namespace am {
namespace math {

	template <class T>
	class Matrix
	{
	public:
		Matrix(void) { identity(); }
		~Matrix(void) {}

		void rotate(const Vector<T> &axis, double angle)
		{
			identity();
			double c = cos(angle);
			double s = sin(angle);
			double t = 1.0 - c;
			
			double x = axis.x;
			double y = axis.y;
			double z = axis.z;
			
			xx = static_cast<T>(t * x * z + c);
			xy = static_cast<T>(t * x * y + s * z); 	
			xz = static_cast<T>(t * x * z - s * y);
            	
			yx = static_cast<T>(t * x * y - s * z); 	
			yy = static_cast<T>(t * y * y + c);
			yz = static_cast<T>(t * y * z + s * x);
        	
			zx = static_cast<T>(t * x * z + s * y); 	
			zy = static_cast<T>(t * y * z - s * x); 	
			zz = static_cast<T>(t * z * z + c);
		}

		void transformVectorConst(Vector<T> &vec) const
		{
			T x = vec.x;
			T y = vec.y;
			T z = vec.z;
		
			vec.x = x * xx + y * xy + z * xz;
			vec.y = x * yx + y * yy + z * yz;
			vec.z = x * zx + y * zy + z * zz;
		}

		Vector<T> transformVector(const Vector<T> &vec) const
		{
			T x = vec.x;
			T y = vec.y;
			T z = vec.z;
		
			return Vector<T>(x * xx + y * xy + z * xz, 
				x * yx + y * yy + z * yz,
				x * zx + y * zy + z * zz);
		}

		void multiply(const Matrix<T> &rhs)
		{
			Matrix<T> clone = *this;
			xx = clone.xx * rhs.xx +
				 clone.xy * rhs.yx +
				 clone.xz * rhs.zx +
				 clone.xw * rhs.wx;

			xy = clone.xx * rhs.xy +
				 clone.xy * rhs.yy +
				 clone.xz * rhs.zy +
				 clone.xw * rhs.wy;

			xz = clone.xx * rhs.xz +
				 clone.xy * rhs.yz +
				 clone.xz * rhs.zz +
				 clone.xw * rhs.wz;

			xw = clone.xx * rhs.xw +
				 clone.xy * rhs.yw +
				 clone.xz * rhs.zw +
				 clone.xw * rhs.ww;
			 
			yx = clone.yx * rhs.xx +
				 clone.yy * rhs.yx +
				 clone.yz * rhs.zx +
				 clone.yw * rhs.wx;

			yy = clone.yx * rhs.xy +
				 clone.yy * rhs.yy +
				 clone.yz * rhs.zy +
				 clone.yw * rhs.wy;

			yz = clone.yx * rhs.xz +
				 clone.yy * rhs.yz +
				 clone.yz * rhs.zz +
				 clone.yw * rhs.wz;

			yw = clone.yx * rhs.xw +
				 clone.yy * rhs.yw +
				 clone.yz * rhs.zw +
				 clone.yw * rhs.ww;
			 
			zx = clone.zx * rhs.xx +
				 clone.zy * rhs.yx +
				 clone.zz * rhs.zx +
				 clone.zw * rhs.wx;

			zy = clone.zx * rhs.xy +
				 clone.zy * rhs.yy +
				 clone.zz * rhs.zy +
				 clone.zw * rhs.wy;
		
			zz = clone.zx * rhs.xz +
				 clone.zy * rhs.yz +
				 clone.zz * rhs.zz +
				 clone.zw * rhs.wz;
			 
			zw = clone.zx * rhs.xw +
				 clone.zy * rhs.yw +
				 clone.zz * rhs.zw +
				 clone.zw * rhs.ww;
			 
			wx = clone.wx * rhs.xx +
				 clone.wy * rhs.yx +
				 clone.wz * rhs.zx +
				 clone.ww * rhs.wx;

			wy = clone.wx * rhs.xy +
				 clone.wy * rhs.yy +
				 clone.wz * rhs.zy +
				 clone.ww * rhs.wy;
		
			wz = clone.wx * rhs.xz +
				 clone.wy * rhs.yz +
				 clone.wz * rhs.zz +
				 clone.ww * rhs.wz;
			 
			ww = clone.wx * rhs.xw +
				 clone.wy * rhs.yw +
				 clone.wz * rhs.zw +
				 clone.ww * rhs.ww;
		}

		inline void identity()
		{
			xy = xz = xw = 
			yx = yz = yw = 
			zx = zy = zw = 
			wx = wy = wz = 0;
			xx = yy = zz = ww = 1;
		}

		void displayMatrix() const
		{
			printf("%f, %f, %f, %f\n", xx, xy, xz, xw);
			printf("%f, %f, %f, %f\n", yx, yy, yz, yw);
			printf("%f, %f, %f, %f\n", zx, zy, zz, zw);
			printf("%f, %f, %f, %f\n", wx, wy, wz, ww);
		}

		inline void translate(T dx, T dy, T dz)
		{
			wx += dx;
			wy += dy;
			wz += dz;
		}

		inline void translate(const Vector<T> &vec)
		{
			wx += vec.x;
			wy += vec.y;
			wz += vec.z;
		}
	
		inline void translateVector(Vector<T> &vec) const {
			vec.x += wx;
			vec.y += wy;
			vec.z += wz;
		}
	
		inline Vector<T> translateVectorConst(const Vector<T> &vec) const {
			return Vector<T>(vec.x + wx, vec.y + wy, vec.z + wz, 1);
		}

		inline Matrix<T> &operator=(const Matrix<T> &rhs)
		{
			xx = rhs.xx;
			xy = rhs.xy;
			xz = rhs.xz;
			xw = rhs.xw;

			yx = rhs.yx;
			yy = rhs.yy;
			yz = rhs.yz;
			yw = rhs.yw;

			zx = rhs.zx;
			zy = rhs.zy;
			zz = rhs.zz;
			zw = rhs.zw;

			wx = rhs.wx;
			wy = rhs.wy;
			wz = rhs.wz;
			ww = rhs.ww;

			return *this;
		}

		T xx;
		T xy;
		T xz;
		T xw;

		T yx;
		T yy;
		T yz;
		T yw;

		T zx;
		T zy;
		T zz;
		T zw;

		T wx;
		T wy;
		T wz;
		T ww;

	};

	typedef Matrix<float> Matrix4f;
	typedef Matrix<double> Matrix4d;

}
}
