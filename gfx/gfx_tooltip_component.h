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
namespace ui {
	class Tooltip;
}
using am::ui::Tooltip;

namespace gfx {

	class Renderable;

	class TooltipComponent : public IEventListener {
	public:

		TooltipComponent();
		TooltipComponent(Renderable *parent);
		TooltipComponent(const TooltipComponent &copy);
		~TooltipComponent();

		void setTooltip(const char *tooltip);
		const char *getTooltip() const;

		void setDetailedTooltip(const char *tooltip);
		const char *getDetailedTooltip() const;

		void setParent(Renderable *parent);
		Renderable *getParent() const;

		void setEnabled(bool enabled);
		bool isEnabled() const;

		virtual void onEvent(MouseEvent *e);

	protected:

		string mTooltip;
		string mDetailedTooltip;
		Renderable *mParent;
		bool mListeners;
		bool mEnabled;

		void updateListeners();
		void addListeners();
		void removeListeners();

		virtual Tooltip *getTooltipObject() const;
		
	};

}
}
