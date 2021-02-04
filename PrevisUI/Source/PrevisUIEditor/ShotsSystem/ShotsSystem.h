#include "PrevisUIEditor/PrevisUIEditorInterface.h"
#include "Widgets/Docking/SDockTab.h"

class FMenuBarBuilder;
class FMenuBuilder;

class FShotsSystem : public FPrevisUIEditorModule
{
public:
	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;

	// build UI method
	void CreateMenuEntry(FMenuBuilder& _menuBuilder);


	// CallbackUI
	virtual TSharedRef<SDockTab> SpawnWindow(const FSpawnTabArgs& _tabSpawnArgs);
};
