#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct DebugMenu : GameObject::Entity {

    // ---------------
    // Enums & Structs
    // ---------------

    struct Menu {
        typedef void (DebugMenu::*ActionPtr)();
        const char *displayName = "NO NAME";
        int32 childrenCount = 0;
        Menu* parent = nullptr;
        Menu* children[8] { };
        StateMachine<DebugMenu> action;

        Menu(const char* displayName, int32 childrenCount, ...)
        {
            this->displayName = displayName;
            this->childrenCount = childrenCount;
            this->action.Init();
            va_list args;
            va_start(args, childrenCount);
            
            if (childrenCount == 0)
                action.Set(va_arg(args, ActionPtr));
            else
                for (int32 i = 0; i < childrenCount; ++i)
                    children[i] = va_arg(args, Menu*);
            va_end(args);
        }
    };

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static { };

    // ----------------
    // Entity Variables
    // ----------------

    Vector2 menuPos;
    Animator animator;
    Menu* currentMenu;
    int32 selectedIndex;

    // ----------------------
    // Standard Entity Events
    // ----------------------

    void Create(void *data);
    void Update(void);
    void Draw(void);
    static void StageLoad(void);

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    void CloseMenu(void);

    // -------------
    // Object States
    // -------------

    void Action_LoadMissionTest(void);
    void Action_LoadMissionTest2(void);
    void Action_LoadMissionTest3(void);
    void Action_LoadMissionTest4(void);
    void Action_LoadMissionTest5(void);
    void Action_LoadMissionTest6(void);
    void Action_LoadMissionTest7(void);
    void Action_LoadMissionTest8(void);
    void Action_CompleteObjective(void);

    // -------------------
    // Static Declarations
    // -------------------

    static Menu mainMenu;

    RSDK_DECLARE(DebugMenu)
};

} // namespace GameLogic