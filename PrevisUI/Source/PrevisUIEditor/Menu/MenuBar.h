#pragma once
#include "../PrevisUIEditorInterface.h"

// For classes related to the level editor (FLevelEditorModule, FExtensibilityManager, ...)
#include "LevelEditor.h"


class FMenuBar : public FPrevisUIEditorModule
{
public:
    virtual ~FMenuBar() {}

    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;

    // Called by MenuButtons.cpp, add content to the menu bar we will create
    void AddMenuExtension(
        const FMenuExtensionDelegate& extensionDelegate, 
        FName extensionHook, 
        const TSharedPtr<FUICommandList>& CommandList = NULL, 
        EExtensionHook::Position position = EExtensionHook::Before);


private:
    // Store all the infos for the extension of the menu bar
    TSharedPtr<FExtender> MenuExtender;

    // Called by AddMenuBarExtension as a callback to create the menu bar
    void CreateMenuBar(FMenuBarBuilder& menuBuilder);
    // Called by CreateMenuBar as a callback to create the menu content
    void CreateMenuContent(FMenuBuilder& menuBuilder);
};