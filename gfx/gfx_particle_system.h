#pragma once

#include <base/handle.h>
#include <base/imanaged.h>

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_asset.h>

namespace am {
namespace gfx {

	class Particle;

	class ParticleSystem : public Renderable {
	public:
		ParticleSystem();
		~ParticleSystem();

		void setParticleAsset(const char *assetName);
		void setParticleAsset(Asset *asset);
		Asset *getParticleAsset();

		void setMaxAge(float age);
		float getMaxAge() const;

		void setAcceleration(float accX, float accY);
		float getAccelerationX() const;
		float getAccelerationY() const;

		void setMaxParticles(int max);
		int getMaxParticles() const;

		int getCurrentParticles() const;

		void setDirection(float direction);
		float getDirection() const;

		void setDirectionVariation(float variation);
		float getDirectionVariation() const;

		void setPower(float power);
		float getPower() const;

		void setPowerVariation(float variation);
		float getPowerVariation() const;

		void setParticlesPerSecond(float speed);
		float getParticlesPerSecond() const;

		void setStartAlpha(float start);
		float getStartAlpha() const;

		void setEndAlpha(float end);
		float getEndAlpha() const;

		virtual void render(float dt);

		friend class Particle;

	protected:

		typedef vector< am::base::Handle<Particle> > ParticleList;
		ParticleList mParticleList;
		
		am::base::Handle<Asset> mParticleAsset;

		float mMaxAge;
		float mAccelerationX;
		float mAccelerationY;
		int mMaxParticles;
		float mCurrentParticleCount;
		float mParticlesPerSecond;

		float mDirection;
		float mDirectionVariation;

		float mPower;
		float mPowerVariation;

		float mStartAlpha;
		float mEndAlpha;

		void newParticle();

	};

	class Particle : public Sprite {
	public:
		Particle();
		Particle(Asset *asset, float veloX, float veloY);
		~Particle();

		float getAge() const;
		float getVelocityX() const;
		float getVelocityY() const;

		friend class ParticleSystem;

	protected:

		float mAge;
		float mVelocityX;
		float mVelocityY;

		virtual void preRender(const ParticleSystem &system, const float accX, const float accY, float dt);
		
	};

}
}
