#pragma once
#include "../PrevisUIEditorInterface.h"

// For classes related to the level editor (FLevelEditorModule, FExtensibilityManager, ...)
#include "LevelEditor.h"


class FMenuButtons : public FPrevisUIEditorModule
{
public:
    virtual ~FMenuButtons() {}

    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;

    // Called by AddMenuExtension, creates the buttons of the menu
    void MakeMenuEntry(FMenuBuilder& menuBuilder);

private:
    // Pointer to a list of commands to be called by the buttons
    TSharedPtr<FUICommandList> CommandList;
    // Add commands to CommandList
    void MapCommands();

    // Code to execute for the MenuCommand1
    void MenuCommand1();


};