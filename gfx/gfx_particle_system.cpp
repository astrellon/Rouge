#include "gfx_particle_system.h"

#include <time.h>

#include "gfx_engine.h"

namespace am {
namespace gfx {

	Particle::Particle() :
		Sprite(),
		mAge(0.0f),
		mVelocityX(0.0f),
		mVelocityY(0.0f)
	{
	}
	Particle::Particle(Asset *asset, float veloX, float veloY) :
		Sprite(asset),
		mAge(0.0f),
		mVelocityX(veloX),
		mVelocityY(veloY)
	{
	}
	Particle::~Particle()
	{
	}

	float Particle::getAge() const
	{
		return mAge;
	}

	float Particle::getVelocityX() const
	{
		return mVelocityX;
	}
	float Particle::getVelocityY() const
	{
		return mVelocityY;
	}

	void Particle::preRender(const ParticleSystem &system, const float accX, const float accY, float dt)
	{
		mAge += dt;
		mVelocityX += accX;
		mVelocityY += accY;
		mTransform.translateWorld(mVelocityX * dt, mVelocityY * dt);

		Sprite::preRender(dt);
	}

	ParticleSystem::ParticleSystem() :
		mParticlesPerSecond(1.0f),
		mCurrentParticleCount(0.0f),
		mMaxParticles(1),
		mAccelerationX(0.0f),
		mAccelerationY(0.0f),
		mMaxAge(1.0f),
		mDirection(0.0f),
		mDirectionVariation(0.0f),
		mPower(0.0f),
		mPowerVariation(0.0f),
		mStartAlpha(1.0f),
		mEndAlpha(0.0f)
	{
		srand( static_cast<unsigned int>(time(nullptr)) );
	}
	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::setParticleAsset(const char *assetName)
	{
		mParticleAsset = GfxEngine::getEngine()->getAssetLua(assetName);
	}
	void ParticleSystem::setParticleAsset(Asset *asset)
	{
		mParticleAsset = asset;
	}
	Asset *ParticleSystem::getParticleAsset()
	{
		return mParticleAsset;
	}

	void ParticleSystem::setMaxAge(float age)
	{
		mMaxAge = age;
	}
	float ParticleSystem::getMaxAge() const
	{
		return mMaxAge;
	}

	void ParticleSystem::setAcceleration(float accX, float accY)
	{
		mAccelerationX = accX;
		mAccelerationY = accY;
	}
	float ParticleSystem::getAccelerationX() const
	{
		return mAccelerationX;
	}
	float ParticleSystem::getAccelerationY() const
	{
		return mAccelerationY;
	}

	void ParticleSystem::setMaxParticles(int max)
	{
		mMaxParticles = max;
	}
	int ParticleSystem::getMaxParticles() const
	{
		return mMaxParticles;
	}

	int ParticleSystem::getCurrentParticles() const
	{
		return mParticleList.size();
	}

	void ParticleSystem::setParticlesPerSecond(float speed)
	{
		mParticlesPerSecond = speed;
	}
	float ParticleSystem::getParticlesPerSecond() const
	{
		return mParticlesPerSecond;
	}

	void ParticleSystem::setDirection(float direction)
	{
		mDirection = direction;
	}
	float ParticleSystem::getDirection() const
	{
		return mDirection;
	}

	void ParticleSystem::setDirectionVariation(float variation)
	{
		mDirectionVariation = variation;
	}
	float ParticleSystem::getDirectionVariation() const
	{
		return mDirectionVariation;
	}

	void ParticleSystem::setPower(float power)
	{
		mPower = power;
	}
	float ParticleSystem::getPower() const
	{
		return mPower;
	}

	void ParticleSystem::setPowerVariation(float variation)
	{
		mPowerVariation = variation;
	}
	float ParticleSystem::getPowerVariation() const
	{
		return mPowerVariation;
	}

	void ParticleSystem::setStartAlpha(float start)
	{
		mStartAlpha = start;
	}
	float ParticleSystem::getStartAlpha() const
	{
		return mStartAlpha;
	}

	void ParticleSystem::setEndAlpha(float end)
	{
		mEndAlpha = end;
	}
	float ParticleSystem::getEndAlpha() const
	{
		return mEndAlpha;
	}

	void ParticleSystem::render(float dt)
	{
		preRender(dt);

		unsigned int newParticles = 0;
		if (mParticleList.size() < static_cast<unsigned int>(mMaxParticles))
		{
			mCurrentParticleCount += dt * mParticlesPerSecond;
			newParticles = static_cast<unsigned int>(mCurrentParticleCount);
			if (mParticleList.size() + newParticles > static_cast<unsigned int>(mMaxParticles)) 
			{
				newParticles = mMaxParticles - mParticleList.size();
			}
			mCurrentParticleCount -= static_cast<float>(newParticles);
		}
		else
		{
			mCurrentParticleCount = 0.0f;
		}

		if (newParticles > 0)
		{
			for (unsigned int i = 0; i < newParticles; i++)
			{
				newParticle();
			}
		}

		float accX = getAccelerationX() * dt;
		float accY = getAccelerationY() * dt;
		float ageResp = 1.0f / mMaxAge;
		float alphaRange = mEndAlpha - mStartAlpha;
		for (size_t i = 0; i < mParticleList.size(); i++)
		{
			Particle *particle = mParticleList[i];

			float t = particle->getAge() * ageResp;
			particle->getGfxComponent()->setAlpha(t * alphaRange + mStartAlpha);
			particle->preRender(*this, accX, accY, dt);
			if (particle->getAge() >= mMaxAge)
			{
				particle->postRender(dt);
				mParticleList.erase(mParticleList.begin() + i);
				i--;
				continue;
			}
			particle->render(dt);
			particle->postRender(dt);
		}

		postRender(dt);
	}

	void ParticleSystem::newParticle()
	{
		float randValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
		float angle = mDirection + M_PIF2 * randValue * mDirectionVariation;

		randValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
		float power = mPower + randValue * mPowerVariation;

		float velocityX = cos(angle) * power;
		float velocityY = sin(angle) * power;

		Particle *particle = new Particle(mParticleAsset, velocityX, velocityY);
		particle->setGfxComponent(new GfxComponent());
		mParticleList.push_back(particle);
	}

}
}
