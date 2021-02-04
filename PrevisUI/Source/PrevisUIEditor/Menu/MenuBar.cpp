#include "MenuBar.h"


void FMenuBar::OnStartupModule()
{
	if (!IsRunningCommandlet())
	{

		// Store a reference to the level editor
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		// Store a reference to the extensibility manager
		TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		// Initialize the class containing the elements for the extension (in our case  it will be the menu bar)
		MenuExtender = MakeShareable(new FExtender);

		// Add to the extender a menu bar extension
		MenuExtender->AddMenuBarExtension(
			"Window",																// The name of the extension of the menu bar
			EExtensionHook::After,													// The place where we want to place our extension
			NULL,																	// The command list to be called when it is open
			FMenuBarExtensionDelegate::CreateRaw(this, &FMenuBar::CreateMenuBar));	// Funcion that will create the UI element

		// Apply the extender to the extensibility manager
		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
	

	UE_LOG(LogTemp, Warning, TEXT("PrevisUI Module : Menu bar loaded"));
}

void FMenuBar::OnShutdownModule()
{
	
}

void FMenuBar::CreateMenuBar(FMenuBarBuilder& menuBuilder)
{
	// Create the menu bar
	menuBuilder.AddPullDownMenu(
		FText::FromString("Example"),									// The name of the menu bar
		FText::FromString("Open the Example menu"),						// The tooltip
		FNewMenuDelegate::CreateRaw(this, &FMenuBar::CreateMenuContent),	// Funcion that will create the content of this menu bar
		"Example");														// Hook name to place future extensions
}

void FMenuBar::CreateMenuContent(FMenuBuilder& menuBuilder)
{
	// Create sections for the menu
	menuBuilder.BeginSection("ExampleSection", FText::FromString("Section 1"));
	menuBuilder.AddMenuSeparator(FName("Section_1"));
	menuBuilder.EndSection();

	menuBuilder.BeginSection("ExampleSection", FText::FromString("Section 2"));
	menuBuilder.AddMenuSeparator(FName("Section_2"));
	menuBuilder.EndSection();

	menuBuilder.BeginSection("DebugSection", FText::FromString("Shot Section"));
	menuBuilder.AddMenuSeparator(FName("Shot_Section"));
	menuBuilder.EndSection();
}

void FMenuBar::AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList>& CommandList, EExtensionHook::Position position)
{
	if (!CommandList) return;
;	MenuExtender->AddMenuExtension(extensionHook, position, CommandList, extensionDelegate);
}