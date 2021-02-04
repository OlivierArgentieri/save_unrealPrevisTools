#pragma once
#include "PrevisUIEditorInterface.h"

// For basic classes
#include "UnrealEd.h"
// For classes related to the level editor (FLevelEditorModule, FExtensibilityManager, ...)
#include "LevelEditor.h"

// Forward declaration
class FMenuBar;
class FMenuButtons;
class FWindowBase;
class FCamSettings;
class FMainWindow;
class FShotsSystem;


class FPrevisUIEditor : public IPrevisUIEditorInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    // Called at StartupModule() to load all the modules
    virtual void AddModule() override;

    // Singleton like acces to this module
    static inline FPrevisUIEditor& Get()
    {
        return FModuleManager::LoadModuleChecked< FPrevisUIEditor >("PrevisUIEditor");
    }

    // Check if the module was loaded correctly
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("PrevisUIEditor");
    }


public:
    // Pointers to modules to be loaded, usefull to acces them from any other module
    FMenuBar* menuBar = nullptr;
    FMenuButtons* menuButtons = nullptr;
    FWindowBase* windowBase = nullptr;
    FCamSettings* camSettings = nullptr;
    FMainWindow * mainWindow = nullptr;
    FShotsSystem* shotsSystem = nullptr;

};