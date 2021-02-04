#pragma once
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Internationalization/Text.h"

#include "EditorStyleSet.h"
class FCommandWindow : public TCommands<FCommandWindow>
{
public :
	FCommandWindow() : TCommands<FCommandWindow>( FName(TEXT("Open_Window_Previz")), FText::FromString("Open Window Previz"), NAME_None,
		FEditorStyle::GetStyleSetName())
	{

	};

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> OpenWindowPreviz;
};

