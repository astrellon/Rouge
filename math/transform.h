#pragma once

#include "vector.h"
#include "matrix.h"

namespace am {
namespace math {

	class Transform 
	{
	public:
		Transform();
		~Transform();
		
		void calcMatrix();
	
		Vector4f getUp() const;
		void setUp(const Vector4f &val);
	
		Vector4f getRight() const;
		void setRight(const Vector4f &val);
	
		Vector4f getForward() const;
		void setForward(const Vector4f &val);
		
		Vector4f getPosition() const;
		void setPosition(const Vector4f &val);
		void setPosition(const float &x, const float &y, const float &z = 0.0f);
	
		//Vector4f getTarget() const;
		//void setTarget(const Vector4f &val);

		Vector4f getUpDirection() const;
		void setUpDirection(const Vector4f &val);
	
		//void lookAtTarget();
		//void updateTarget();
	
		Matrix4f getWorldToObj() const;
		Matrix4f &getWorldToObj();
		float *data();
		void preproc();
	
		void translate(const Vector4f &rhs);
		void translate(const float x, const float y, const float z = 0.0f);

		void translateWorld(const Vector4f &rhs);
		void translateWorld(const float x, const float y, const float z = 0.0f);

		void rotate(float dx, float dy);
		//void orbit(float dx, float dy);
		//void setRotation(float dr);
		//float getRotation() const;
	
		//void setForCamera(bool forCam);
		//bool isForCamera() const;
	
		static const Vector4f REF_UP;
		static const Vector4f REF_RIGHT;
		static const Vector4f REF_FORWARD;
	
	private:
		//bool mForCamera;
		//bool mDirty;
		//float mRotation;
		//Vector4f mUpVector;
		Vector4f mUp;
		Vector4f mRight;
		Vector4f mForward;
		Vector4f mPosition;
		//Vector4f mTarget;
		Matrix4f mWorldToObj;
	};

}
}
