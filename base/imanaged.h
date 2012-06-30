#pragma once

namespace am {
namespace base {

	class IManaged {
	public:
		IManaged();
		virtual ~IManaged();

		virtual void release();
		virtual void retain();

	private:

		int mRefCounter;
	};

}
}
