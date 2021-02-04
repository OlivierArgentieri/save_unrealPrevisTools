#pragma once
#include "../PrevisUIEditorInterface.h"

// For SDockTab Slate widget
#include "Widgets/Docking/SDockTab.h"

// Base class for windows
class FWindowBase : public FPrevisUIEditorModule
{
public:
    virtual ~FWindowBase() {}

    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;

    // Called by OnStartupModule to spawn the window
    virtual TSharedRef<SDockTab> SpawnWindow(const FSpawnTabArgs& TabSpawnArgs);

    // Called by OnStartupModule to create the menu entry
    virtual void CreateMenuEntry(FMenuBuilder& menuBuilder);

private:
    FName WindowName;
    FText WindowDisplayName;
    FText ToolTip;
};