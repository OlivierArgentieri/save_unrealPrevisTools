#include "PrevisUIEditor.h"
#include "Menu/MenuBar.h"
#include "Menu/MenuButtons.h"
#include "Viewport/WindowBase.h"

#include "MainWindow/MainWindow.h"

#include "ShotsSystem/ShotsSystem.h"



IMPLEMENT_GAME_MODULE(FPrevisUIEditor, PrevisUIEditor)


void FPrevisUIEditor::AddModule()
{
    // Initialize all the modules
    menuBar = new FMenuBar;
    menuButtons = new FMenuButtons;
    windowBase = new FWindowBase;
    mainWindow = new FMainWindow;
    shotsSystem = new FShotsSystem;

	// Load all the modules
    Modules.Add(MakeShareable(menuBar));
    Modules.Add(MakeShareable(menuButtons));
    Modules.Add(MakeShareable(windowBase));
    Modules.Add(MakeShareable(shotsSystem));
    Modules.Add(MakeShareable(mainWindow));
}

void FPrevisUIEditor::StartupModule()
{
    IPrevisUIEditorInterface::StartupModule();

    UE_LOG(LogTemp, Warning, TEXT("PrevisUI Module : Modules loaded successfull"));
}

void FPrevisUIEditor::ShutdownModule()
{
    IPrevisUIEditorInterface::ShutdownModule();
}