#pragma once
#include "framework.h"
#include <subhook.h>
#include <stdexcept>

template <typename TRet, typename... TArgs>
class Hook {
public:
    typedef TRet (*FunctionType)(TArgs...);

    Hook() : hook() {}

    void Install(FunctionType original, FunctionType hooked) {
        this->pointer = original;
        
        if (!hook.Install((void*)original, (void*)hooked, subhook::HookFlag64BitOffset)) {
            throw new std::runtime_error("Failed to hook function");
        }
    }
    void Remove() {
        hook.Remove();
    }

    TRet Original(TArgs... args) {
        subhook::ScopedHookRemove remove(&this->hook);
        
        return this->pointer(args...);
    }

    TRet operator()(TArgs... args)
    {
        return this->pointer(args...);
    }
private:
    FunctionType pointer;
    subhook::Hook hook;
};