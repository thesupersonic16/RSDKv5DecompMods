#include "GameAPI/Game.hpp"
namespace Helpers {
    bool32 CheckTouch(int32 x, int32 y, int32 w, int32 h);
    
    template <typename T>
    void (T::*ToMember(void *ptr))() {
        union FuncUnion {
            void* in;
            void (T::*out)();
        };

        FuncUnion caster;
        caster.in = ptr;
        return caster.out;
    }

    template <typename T>
    void (*ToGeneric(void (T::*ptr)()))() {
        union FuncUnion {
            void (T::*in)();
            void (*out)();
        };

        FuncUnion caster;
        caster.in = ptr;
        return caster.out;
    }

} // namespace Helpers