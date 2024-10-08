#include "S3KExtensions.hpp"

namespace Origins {
    MissionInfo *MissionInfo::ParseRFL(void *rfl) {
        int32 offset = 0x00;
        // Check BINA
        if (*(uint32*)rfl == 0x414E4942)
            offset = 0x40;
        auto missionInfo = reinterpret_cast<MissionInfo*>(((char*)rfl + offset));

        // Fix pointers
        for (auto missionData : missionInfo->m_missionData)
            missionData.m_debugDescription = 
            (const char*)(missionInfo) + (uintptr_t)missionData.m_debugDescription;

        return missionInfo;
    }
} //namespace Helpers