#include "CommandWindow.h"
#include "PrevisUIEditor/PrevisUIEditor.h"
#include "Framework/Commands/Commands.h"

void FCommandWindow::RegisterCommands()
{
	#define LOCTEXT_NAMESPACE " "
	UI_COMMAND(OpenWindowPreviz, "OpenPreviz", "Open Previz Window", EUserInterfaceActionType::Button, FInputGesture());
	#undef LOCTEXT_NAMESPACE 
}

