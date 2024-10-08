#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

    DebugMenu::Menu DebugMenu::mainMenu = Menu("Main Menu", 2,
        new Menu("Mission", 10,
            new Menu("Load Mission 109", 0, &DebugMenu::Action_LoadMissionTest),
            new Menu("Load Mission 61", 0, &DebugMenu::Action_LoadMissionTest2),
            new Menu("Load Mission 72", 0, &DebugMenu::Action_LoadMissionTest3),
            new Menu("Load Mission 52", 0, &DebugMenu::Action_LoadMissionTest4),
            new Menu("Load Mission 58", 0, &DebugMenu::Action_LoadMissionTest5),
            new Menu("Load Mission 67", 0, &DebugMenu::Action_LoadMissionTest6),
            new Menu("Load Mission 56", 0, &DebugMenu::Action_LoadMissionTest7),
            new Menu("Load Mission 55", 0, &DebugMenu::Action_LoadMissionTest8),
            new Menu("Complete Objective", 0, &DebugMenu::Action_CompleteObjective),
            new Menu("Back", 0, nullptr)
        ),
        new Menu("Exit", 0, nullptr)
        );

    // -------------------
    // Object Registration
    // -------------------

    RSDK_REGISTER_OBJECT(DebugMenu);

    // ----------------------
    // Standard Entity Events
    // ----------------------

    void DebugMenu::Create(void *data) {
        if (sceneInfo->inEditor)
            return;

        this->active = ACTIVE_ALWAYS;
        this->visible = true;
        this->drawGroup = DRAWGROUP_COUNT - 1;
        this->currentMenu = &mainMenu;
        this->selectedIndex = 0;
        this->menuPos.x = 6;
        this->menuPos.y = 6;
    }

    void DebugMenu::Update(void) {
        auto controller = &controllerInfo[0];

        if (controller->keyUp.press) {
            if (this->selectedIndex <= 0)
                this->selectedIndex = this->currentMenu->childrenCount - 1;
            else
                this->selectedIndex--;
        } 
        else if (controller->keyDown.press) {
            if (this->selectedIndex >= this->currentMenu->childrenCount - 1)
                this->selectedIndex = 0;
            else
                this->selectedIndex++;
        }
        else if (controller->keyStart.press || controller->keyA.press) {
            auto menu = this->currentMenu->children[this->selectedIndex];
            if (menu->childrenCount == 0) {
                if (menu->action.state)
                    menu->action.Run(this);
                else {
                    if (menu->parent) {
                        this->currentMenu = menu->parent;
                        this->selectedIndex = 0;
                    }
                    else
                        this->CloseMenu();
                }
            }
            else {
                menu->parent = this->currentMenu;
                this->currentMenu = menu;
                this->selectedIndex = 0;
            }
        }
    }

    void DebugMenu::Draw(void) {
        
        Vector2 currPos = this->menuPos;
        Graphics::DrawRect(currPos.x, currPos.y, 160, 24, 0x000000, 0xFF, INK_NONE, true);
        DrawHelpers::DrawDevString(this->currentMenu->displayName, currPos.x + 80, currPos.y + 8, ALIGN_CENTER, 0xF0F0F0);
        currPos.y += 28;

        for (int i = 0; i < this->currentMenu->childrenCount; ++i) {
            Graphics::DrawRect(currPos.x, currPos.y, 160, 18, this->selectedIndex == i ? 0x404040 : 0x000000, 0xFF, INK_NONE, true);
            DrawHelpers::DrawDevString(this->currentMenu->children[i]->displayName, currPos.x + 80, currPos.y + 6, ALIGN_CENTER, 0xF0F0F0);
            currPos.y += 18;
        }
    }

    void DebugMenu::StageLoad(void) {
    }

    void DebugMenu::CloseMenu(void) {
        PauseMenu::ResumeMusic();
        Stage::SetEngineState(ENGINESTATE_REGULAR);
        this->Destroy();
    }

    void DebugMenu::Action_LoadMissionTest(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[109];
        Mission::StartMission(missionData, Mission::MISSIONNO_BALLOONBURST);
    }

    void DebugMenu::Action_LoadMissionTest2(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[61];
        Mission::StartMission(missionData, Mission::MISSIONNO_NONE);
    }

    void DebugMenu::Action_LoadMissionTest3(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[72];
        Mission::StartMission(missionData, Mission::MISSIONNO_BALLOONBURST);
    }

    void DebugMenu::Action_LoadMissionTest4(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[52];
        Mission::StartMission(missionData, Mission::MISSIONNO_MERCY);
    }

    void DebugMenu::Action_LoadMissionTest5(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[58];
        Mission::StartMission(missionData, Mission::MISSIONNO_RINGVACCUM);
    }

    void DebugMenu::Action_LoadMissionTest6(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[67];
        Mission::StartMission(missionData, Mission::MISSIONNO_NONE);
    }

    void DebugMenu::Action_LoadMissionTest7(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[56];
        Mission::StartMission(missionData, (Mission::MissionFunctions)11);
    }

    void DebugMenu::Action_LoadMissionTest8(void) {
        if (!modGlobals->misionInfo) {
            Dev::Print(Dev::PRINT_POPUP, "Mission Info Missing");
            return;
        }

        auto missionData = &modGlobals->misionInfo->m_missionData[55];
        Mission::StartMission(missionData, (Mission::MissionFunctions)11);
    }

    void DebugMenu::Action_CompleteObjective(void) {
        if (!modGlobals->missionData) {
            Dev::Print(Dev::PRINT_POPUP, "No Mission Data");
            return;
        }

        for (int i = 0; i < 3; ++i)
            modGlobals->missionProgress[i] = modGlobals->missionData->m_conditions[i].m_value;
        this->CloseMenu();
    }

} // namespace GameLogic