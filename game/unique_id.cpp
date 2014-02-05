#include "unique_id.h"

namespace am {
namespace game {

    int UniqueId::sCounter = 0;

    UniqueId::UniqueId() :
        mUniqueId(nextCounter())
    {
    }

    int UniqueId::getUniqueId() const
    {
        return mUniqueId;
    }

    int UniqueId::nextCounter()
    {
        return ++sCounter;
    }

}
}

