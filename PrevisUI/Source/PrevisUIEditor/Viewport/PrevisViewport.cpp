#include "PrevisViewport.h"
#include "Clients/DefaultClient.h"

#include "Slate/SceneViewport.h"

void SPrevisViewport::setName(FString _name)
{
	name = MakeShareable(&_name);
}

void SPrevisViewport::setClient(TSharedPtr<FEditorViewportClient> _client)
{
	client = _client;
}

TSharedRef<FEditorViewportClient> SPrevisViewport::MakeEditorViewportClient()
{
	if (client.IsValid())
	{
		return client.ToSharedRef();
	}
	return MakeShareable(new FDefaultClient());
}

void SPrevisViewport::Construct(const FArguments& InArgs)
{
	if (InArgs._name.IsValid())
	{
		setName(*(InArgs._name.Get()));
	}
	if (InArgs._client.IsValid())
	{
		setClient(InArgs._client);
	}
	
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SPrevisViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Update the viewport (also calls the Draw function of the viewport client connected to this viewport)
	SceneViewport->Draw();
}
