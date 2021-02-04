#include "PrevisUIEditor/ShotsSystem/ShotsSystem.h"
#include "PrevisUIEditor/PrevisUIEditor.h"
#include "PrevisUIEditor/Menu/MenuBar.h"


#include "./ShotsSystemTab/ShotsSystemTab.h"

#define LOCTEXT_NAMESPACE "ShotSystem"
void FShotsSystem::OnStartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Open Shot System"), FOnSpawnTab::CreateRaw(this, &FShotsSystem::SpawnWindow))
		.SetDisplayName(FText(FText::FromString("ShotSystem")))  // Set the displayed name of the window
		.SetTooltipText(FText(FText::FromString("ShotSystem")));


	FPrevisUIEditor::Get().menuBar->AddMenuExtension(
		FMenuExtensionDelegate::CreateRaw(this, &FShotsSystem::CreateMenuEntry),  // Function that will actually create the UI element
		FName("Shot_Section"));
}

void FShotsSystem::OnShutdownModule()
{
}

void FShotsSystem::CreateMenuEntry(FMenuBuilder& _menuBuilder)
{
	FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(_menuBuilder, FName("Open Shot System"));
}

TSharedRef<SDockTab> FShotsSystem::SpawnWindow(const FSpawnTabArgs& _tabSpawnArgs)
{
	// Create the window of type : SDockTab
	TSharedRef<SDockTab> SpawnedWindow = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			// Create a child widget of custom type defined in WindowPanel.h
			SNew(SShotSystemTab)
			//SNew(SWindowPanel)
		];

	return SpawnedWindow;
}
#undef LOCTEXT_NAMESPACE