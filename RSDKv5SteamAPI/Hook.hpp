#pragma once
#include "framework.h"
#include <cstdint>

#if _WINDOWS
#if defined(_M_IX86) || defined(__i386__)
#define JMPSIZE 5
static inline void WriteJump(void* writeaddress, void* funcaddress)
{
    uint8_t data[JMPSIZE]{ 0xE9 };
    *(int32_t*)(data + 1) = (uint32_t)funcaddress - ((uint32_t)writeaddress + JMPSIZE);
    DWORD origprot;
    VirtualProtect(writeaddress, JMPSIZE, PAGE_EXECUTE_WRITECOPY, &origprot);
    memcpy(writeaddress, data, JMPSIZE);
}
#elif defined(_M_AMD64) || defined(__amd64__)
#define JMPSIZE 14
static inline void WriteJump(void* writeaddress, void* funcaddress)
{
    uint8_t data[JMPSIZE]{ 0xFF, 0x25 };
    *(void**)(data + 6) = funcaddress;
    DWORD origprot;
    VirtualProtect(writeaddress, JMPSIZE, PAGE_EXECUTE_WRITECOPY, &origprot);
    memcpy(writeaddress, data, JMPSIZE);
}
#endif
#endif

template<typename TRet, typename... TArgs>
class Func
{
public:
    typedef TRet(*PointerType)(TArgs...);

    Func(intptr_t address)
    {
        pointer = (decltype(pointer))address;
    }

    Func(void* address)
    {
        pointer = (decltype(pointer))address;
    }

    ~Func()
    {
        if (ishooked)
            Unhook();
    }

    TRet operator()(TArgs... args)
    {
        return pointer(args...);
    }

    PointerType operator&()
    {
        return pointer;
    }

    operator PointerType()
    {
        return pointer;
    }

    void Hook(PointerType hookfunc)
    {
        if (ishooked)
            throw new std::exception("Attempted to hook already hooked function!");
        memcpy(origdata, pointer, JMPSIZE);
        WriteJump(pointer, hookfunc);
        ishooked = true;
    }

    void Unhook()
    {
        if (!ishooked)
            throw new std::exception("Attempted to unhook function that wasn't hooked!");
        memcpy(pointer, origdata, JMPSIZE);
        ishooked = false;
    }

    TRet Original(TArgs... args)
    {
        if (ishooked)
        {
            uint8_t hookdata[JMPSIZE];
            memcpy(hookdata, pointer, JMPSIZE);
            memcpy(pointer, origdata, JMPSIZE);
            TRet retval = pointer(args...);
            memcpy(pointer, hookdata, JMPSIZE);
            return retval;
        }
        else
            return pointer(args...);
    }

private:
    PointerType pointer = nullptr;
    bool ishooked = false;
    uint8_t origdata[JMPSIZE]{};
};

template<typename... TArgs>
class Func<void, TArgs...>
{
public:
    typedef void (*PointerType)(TArgs...);

    Func(intptr_t address)
    {
        pointer = (decltype(pointer))address;
    }

    Func(void* address)
    {
        pointer = (decltype(pointer))address;
    }

    ~Func()
    {
        if (ishooked)
            Unhook();
    }

    void operator()(TArgs... args)
    {
        pointer(args...);
    }

    PointerType operator&()
    {
        return pointer;
    }

    operator PointerType()
    {
        return pointer;
    }

    void Hook(PointerType hookfunc)
    {
        if (ishooked)
            throw new std::exception("Attempted to hook already hooked function!");
        memcpy(origdata, pointer, JMPSIZE);
        WriteJump(pointer, hookfunc);
        ishooked = true;
    }

    void Unhook()
    {
        if (!ishooked)
            throw new std::exception("Attempted to unhook function that wasn't hooked!");
        memcpy(pointer, origdata, JMPSIZE);
        ishooked = false;
    }

    void Original(TArgs... args)
    {
        if (ishooked)
        {
            uint8_t hookdata[JMPSIZE];
            memcpy(hookdata, pointer, JMPSIZE);
            memcpy(pointer, origdata, JMPSIZE);
            pointer(args...);
            memcpy(pointer, hookdata, JMPSIZE);
        }
        else
            pointer(args...);
    }

private:
    PointerType pointer = nullptr;
    bool ishooked = false;
    uint8_t origdata[JMPSIZE]{};
};