#include "DefaultClient.h"
#include "Engine/Canvas.h"

FDefaultClient::FDefaultClient() :
	FEditorViewportClient(nullptr, nullptr, nullptr) {}

void FDefaultClient::Draw(FViewport* _viewport, FCanvas* _canvas)
{
	// Clear entire canvas
	_canvas->Clear(FLinearColor::Black);
}