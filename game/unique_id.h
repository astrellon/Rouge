#pragma once

namespace am {
namespace game {

    class UniqueId {
    public:

        typedef int Type;
        UniqueId();

        int getUniqueId() const;

    private:
        int mUniqueId;

        static int sCounter;
        static int nextCounter();
    };

}
}
