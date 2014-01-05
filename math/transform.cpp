#include "transform.h"

namespace am {
namespace math {

	const Vector4f Transform::REF_UP(0, 1, 0);
	const Vector4f Transform::REF_RIGHT(1, 0, 0);
	const Vector4f Transform::REF_FORWARD(0, 0, 1);

	Transform::Transform() :
        mForCamera(false),
		mDirty(false),
        mRotation(0.0f),
        mUpVector(REF_UP),
		mUp(REF_UP),
		mRight(REF_RIGHT),
		mForward(REF_FORWARD)
	{
		mTarget = mPosition.add(mForward);
	}
	Transform::~Transform() 
    {
    }
	
	void Transform::calcMatrix() 
    {
		mWorldToObj.xx = mRight.x;
		mWorldToObj.yx = mRight.y;
		mWorldToObj.zx = mRight.z;
		
		mWorldToObj.xy = mUp.x;
		mWorldToObj.yy = mUp.y;
		mWorldToObj.zy = mUp.z;

		if (mForCamera) 
        {
			mWorldToObj.xz = -mForward.x;
		    mWorldToObj.yz = -mForward.y;
			mWorldToObj.zz = -mForward.z;
		}
		else 
        {
			mWorldToObj.xz = mForward.x;
			mWorldToObj.yz = mForward.y;
			mWorldToObj.zz = mForward.z;
		}
		
		
		mWorldToObj.wx = mPosition.x;
		mWorldToObj.wy = mPosition.y;
		mWorldToObj.wz = mPosition.z;

		/*Vector4f pos = mWorldToObj.translateVectorConst(mPosition);
		mWorldToObj.wx = pos.x;
		mWorldToObj.wy = pos.y;
		mWorldToObj.wz = pos.z;*/
		mWorldToObj.ww = 1.0f;

		mDirty = false;
	}
	
	Vector4f Transform::getUp() const 
    {
		return mUp;
	}
	void Transform::setUp(const Vector4f &val) 
    {
		mUp = val;
		mUp.normalise();
		mForward = mUp.cross(mRight);
		mRight = mForward.cross(mUp);
		mDirty = true;
	}
	
	Vector4f Transform::getRight() const 
    {
		return mRight;
	}
	void Transform::setRight(const Vector4f &val) 
    {
		mRight = val;
		mRight.normalise();
		mForward = mUp.cross(mRight).normalise();
		mUp = mRight.cross(mForward).normalise();
		mDirty = true;
		calcMatrix();
	}
	
	Vector4f Transform::getForward() const 
    {
		return mForward;
	}
	void Transform::setForward(const Vector4f &val) 
    {
		mForward = val;
		mForward.normalise();
		mRight = mForward.cross(mUp).normalise();
		mUp = mRight.cross(mForward).normalise();
		mDirty = true;
		calcMatrix();
	}
	
	Vector4f Transform::getPosition() const 
    {
		return mPosition;
	}
	void Transform::setPosition(const Vector4f &val, bool keepDirection)
    {
        setPosition(val.x, val.y, val.z, keepDirection);
	}
	void Transform::setPosition(const float &x, const float &y, const float &z, bool keepDirection)
	{
		if (keepDirection)
		{
			double toTarget = mTarget.sub(mPosition).length();
    		if (toTarget < 0.001)
    		{
    			toTarget = 1.0;
    		}
			mTarget = mPosition.add(mForward.scale(toTarget));
		}

		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
        if (mForCamera)
        {
            mWorldToObj.wx = x;
            mWorldToObj.wy = y;
            mWorldToObj.wz = z;
        }
        else
        {
		    mWorldToObj.wx = x;
	    	mWorldToObj.wy = y;
    		mWorldToObj.wz = z;
        }

        if (!keepDirection)
        {
        	lookAtTarget();
        }
		mDirty = true;
	}
	
	Vector4f Transform::getTarget() const 
    {
		return mTarget;
	}
	void Transform::setTarget(const Vector4f &val, bool keepDirection)
    {
        setTarget(val.x, val.y, val.z, keepDirection);
	}
    void Transform::setTarget(const float &x, const float &y, const float &z, bool keepDirection)
    {
    	if (keepDirection)
    	{
    		double toTarget = mTarget.sub(mPosition).length();
    		if (toTarget < 0.001)
    		{
    			toTarget = 1.0;
    		}
    		mPosition = mTarget.sub(mForward.scale(toTarget));
    	}

		mTarget.x = x;
		mTarget.y = y;
		mTarget.z = z;
		
		if (!keepDirection)
		{
			lookAtTarget();
		}
		mDirty = true;
    }
	
	Vector4f Transform::getUpDirection() const
	{
		return mUpVector;
	}
	void Transform::setUpDirection(const Vector4f &val)
	{
		mUpVector = val;
	}
    void Transform::setUpDirection(const float &x, const float &y, const float &z)
    {
        mUpVector.x = x;
        mUpVector.y = y;
        mUpVector.z = z;
    }

	void Transform::lookAtTarget() 
    {
		Vector4f toTarget = mTarget.sub(mPosition);
        if (mForCamera)
        {
            toTarget.scale(-1.0f);
        }
		setForward(toTarget);
	}
	void Transform::updateTarget() 
    {
		Vector4f toTarget = mTarget.sub(mPosition);
		double len = toTarget.length();
		
		mTarget = mPosition.add(mForward.scale(len));
	}
	
	Matrix4f Transform::getWorldToObj() const 
    {
		return mWorldToObj;
	}
	
	Matrix4f &Transform::getWorldToObj() 
    {
		preproc();
		return mWorldToObj;
	}
	float *Transform::data()
	{
		preproc();
        return mWorldToObj.data();
	}
	void Transform::preproc()
	{
		if (mDirty) {
			calcMatrix();
		}
	}
	
	void Transform::translate(const Vector4f &rhs) 
    {
		Vector4f vec;
		vec.addTo(mRight.scale(rhs.x));
		vec.addTo(mUp.scale(rhs.y));
		vec.addTo(mForward.scale(rhs.z));
		mPosition.addTo(vec);
		mTarget.addTo(vec);
		
		mDirty = true;
		//calcMatrix();
	}
	void Transform::translate(const float x, const float y, const float z) 
    {
		Vector4f vec;
		vec.addTo(mRight.scale(x));
		vec.addTo(mUp.scale(y));
		vec.addTo(mForward.scale(z));
		vec.y = -vec.y;
		mPosition.addTo(vec);
		mTarget.addTo(vec);
		
		mDirty = true;
		//calcMatrix();
	}

	void Transform::translateWorld(const Vector4f &rhs) 
    {
		mPosition.addTo(rhs);
		
		//calcMatrix();
		mDirty = true;
	}
	void Transform::translateWorld(const float x, const float y, const float z) 
    {
		mPosition.addTo(x, y, z);
		
		mDirty = true;
		//calcMatrix();
	}
	
	void Transform::rotate(float dx, float dy) 
    {
		Matrix4f m;
		m.rotate(mRight, dy);
		m.transformVectorConst(mForward);
		m.transformVectorConst(mUp);
		
		if (mForCamera) 
        {
			m.rotate(REF_UP, dx);
			m.transformVectorConst(mUp);
		}
		else 
        {
            m.rotate(mUp, dx);
		}
		
		m.transformVectorConst(mRight);
		m.transformVectorConst(mForward);
		
		updateTarget();
		mDirty = true;
		calcMatrix();
	}
	
	void Transform::orbit(float dx, float dy) 
    {
		Vector4f toPosition = mPosition.sub(mTarget);
		double len = toPosition.length();
		//std::cout << "Length: " << len << '\n';
		toPosition.scaleBy(1.0 / len);
		Matrix4f m;
		m.rotate(mRight, dy);
		//m.transformVectorConst(toPosition);
		
		if (mForCamera)
		{
			m.rotate(mUpVector, dx);
		}
		else 
		{
			m.rotate(mUp, dx);
		}
		m.transformVectorConst(toPosition);
		mPosition = mTarget.add(toPosition.scale(len));
		mForward = toPosition.scale(-1);
		mRight = mForward.cross(mUp);
		mUp = mRight.cross(mForward);
		
		mDirty = true;
	}
	
	void Transform::setForCamera(bool forCam) 
    {
		mForCamera = forCam;
	}
	bool Transform::isForCamera() const 
    {
		return mForCamera;
	}
	
}
}
