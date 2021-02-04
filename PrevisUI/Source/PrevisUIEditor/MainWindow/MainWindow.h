#pragma once

#include "PrevisUIEditor/PrevisUIEditorInterface.h"
#include "Widgets/Docking/SDockTab.h"
class FMenuBarBuilder;
class FMenuBuilder;

class FMainWindow : public FPrevisUIEditorModule
{

#pragma region delegate
public:
	/// Delegate Part ///
	
#pragma endregion



public:
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    
    // build UI method
    void CreateMenuEntry(FMenuBuilder& _menuBuilder);
    void DisplayViewPortTab();
    void OpenWindow_Clicked();
    void AddToolBarExtension(FToolBarBuilder& builder);
    TSharedPtr<FExtender> ToolbarExtender;
    TSharedPtr<const FExtensionBase> Extension;
};