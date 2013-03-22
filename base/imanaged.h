#pragma once

namespace am {
namespace base {

	class IManaged {
	public:
		IManaged();
		IManaged(const IManaged &copy);
		virtual ~IManaged();

		virtual void release();
		virtual void retain();
		
		int getReferenceCounter() const;

	private:

		int mRefCounter;

	};

}
}
