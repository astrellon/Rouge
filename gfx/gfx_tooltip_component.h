#pragma once

#include <vector>
using std::vector;

#include <base/imanaged.h>
using namespace am::base;

#include <string>
using namespace std;

namespace am {
namespace gfx {

	class TooltipComponent : public IManaged {
	public:

		TooltipComponent();
		TooltipComponent(const TooltipComponent &copy);
		~TooltipComponent();

		void setTooltip(const char *tooltip);
		const char *getTooltip() const;

		void setDetailedTooltip(const char *tooltip);
		const char *getDetailedTooltip() const;

	protected:

		string mTooltip;
		string mDetailedTooltip;
		
	};

}
}
