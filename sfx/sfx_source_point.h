#pragma once

#include <sfx/sfx_isource.h>

namespace am {
namespace sfx {

	class ISound;

	class SourcePoint : public ISource {
	public:
		
		SourcePoint();
		SourcePoint(ISound *sound);
		~SourcePoint();

		void setSourceRelative(bool value);
		bool isSourceRelative() const;

		virtual float calcGain() const;
		virtual bool isOutOfRange() const;

	protected:

		bool mSourceRelative;

		virtual void applyToSource();

	};

}
}
