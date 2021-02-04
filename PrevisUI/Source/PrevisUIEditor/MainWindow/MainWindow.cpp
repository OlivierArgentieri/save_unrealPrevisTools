#include "MainWindow.h"


#include "PrevisUIEditor/Menu/CommandWindow.h"
#include "PrevisUIEditor/PrevisUIEditor.h"
#include "Interfaces/IMainFrameModule.h"
#include "PrevisUIEditor/Menu/MenuBar.h"

#include "PrevisUIEditor/ShotsSystem/ShotsSystemTab/ShotsSystemTab.h"

#include "../Viewport/ViewportLayout.h"

#include "Widgets/Docking/SDockTab.h"
#define LOCTEXT_NAMESPACE "MainWindow"


void FMainWindow::OnStartupModule()
{
	FCommandWindow::Register();
	TSharedPtr<FUICommandList> CommandList = MakeShareable(new FUICommandList());
	CommandList->MapAction(FCommandWindow::Get().OpenWindowPreviz, FExecuteAction::CreateRaw(this, &FMainWindow::OpenWindow_Clicked),
		FCanExecuteAction());
	ToolbarExtender = MakeShareable(new FExtender());
	Extension = ToolbarExtender
		->AddToolBarExtension("Compile", EExtensionHook::Before, CommandList, FToolBarExtensionDelegate::CreateRaw(this, &FMainWindow::AddToolBarExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()
		->AddExtender(ToolbarExtender);
}


void FMainWindow::OnShutdownModule()
{
}

void FMainWindow::CreateMenuEntry(FMenuBuilder& _menuBuilder)
{
	// Add a button that will spawn the given window
	FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(_menuBuilder, FName("Open PrevisUIEditor "));
}




void FMainWindow::DisplayViewPortTab()
{
	static bool HasRegisteredPrevizSettings = false;
	if(!HasRegisteredPrevizSettings)
	{
		TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("Main_Window_Layout")
			->AddArea(
				FTabManager::NewArea(720, 360)
				->Split(
					FTabManager::NewStack()
					->AddTab("Viewport", ETabState::ClosedTab)
					->AddTab("ShotSelector",  ETabState::ClosedTab)
					
				)
			);

		FGlobalTabmanager::Get()->RestoreFrom(layout, TSharedPtr<SWindow>());
		HasRegisteredPrevizSettings = true;
	}
	FGlobalTabmanager::Get()->InsertNewDocumentTab(
		"ShotSelector", FTabManager::ESearchPreference::RequireClosedTab, SNew(SDockTab)
		.Label(LOCTEXT("Label_ShotSelector", "ShotSelector"))
		.TabRole(ETabRole::DocumentTab)
		[
			SNew(SShotSystemTab)
		]
	);

	FGlobalTabmanager::Get()->InsertNewDocumentTab(
		"Viewport", FTabManager::ESearchPreference::RequireClosedTab, SNew(SDockTab)
		.Label(LOCTEXT("Label_viewport", "Viewport"))
		.TabRole(ETabRole::DocumentTab)
		[
			SNew(SViewportLayout)
		]
	);

}



void FMainWindow::OpenWindow_Clicked() {
	DisplayViewPortTab();
};

void FMainWindow::AddToolBarExtension(FToolBarBuilder &builder)
{
	FSlateIcon IconBrush = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions", "LevelEditor.ViewOptions.Small");
	builder.AddToolBarButton(FCommandWindow::Get().OpenWindowPreviz, NAME_None, FText::FromString("Open Window Previz"),
		FText::FromString("Open Previs tab"), IconBrush, NAME_None);
}

#undef LOCTEXT_NAMESPACE