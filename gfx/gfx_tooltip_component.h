#pragma once

#include <vector>
using std::vector;

#include <base/imanaged.h>
using namespace am::base;

#include <string>
using namespace std;

#include <ui/ievent_listener.h>
using namespace am::ui;

namespace am {
namespace gfx {

	class Renderable;

	class TooltipComponent : public IEventListener {
	public:

		TooltipComponent();
		TooltipComponent(const TooltipComponent &copy);
		~TooltipComponent();

		virtual void setTooltip(const char *tooltip);
		virtual const char *getTooltip() const;

		virtual void setDetailedTooltip(const char *tooltip);
		virtual const char *getDetailedTooltip() const;

		virtual void setParent(Renderable *parent);
		virtual Renderable *getParent() const;

		virtual void onEvent(MouseEvent *e);

	protected:

		string mTooltip;
		string mDetailedTooltip;
		Renderable *mParent;
		bool mListeners;

		void updateListeners();
		void addListeners();
		void removeListeners();
		
	};

}
}
