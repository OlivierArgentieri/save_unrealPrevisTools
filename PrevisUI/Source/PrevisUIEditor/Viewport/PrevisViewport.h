#pragma once

#include "SEditorViewport.h"

// Forward declaration
class FEditorViewportClient;

class SPrevisViewport : public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SPrevisViewport) { }
	SLATE_ARGUMENT(TSharedPtr<class FString>, name)
	SLATE_ARGUMENT(TSharedPtr<FEditorViewportClient>, client)
	SLATE_END_ARGS()

	void setName(FString _name);
	void setClient(TSharedPtr<FEditorViewportClient> _client);
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<FString> name;
	TSharedPtr<FEditorViewportClient> client;

public:
	// Tick function that will update the content of WindowViewport
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};