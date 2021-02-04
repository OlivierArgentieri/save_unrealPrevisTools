#pragma once

#include "EditorViewportClient.h"

//Forward declaration
class UTextureRenderTarget2D;
class FPreviewScene;

class FAlphaViewClient : public FEditorViewportClient
{
public:
	FAlphaViewClient();

	// Called by the FViewport that will be connected to this FWindowViewportClient's Draw function 
	virtual void Draw(FViewport* _viewport, FCanvas* _canvas) override;
	
	void SetMaterial(UMaterialInstanceDynamic* _renderingMaterial);

private:
	UCanvas* renderingCanvas = nullptr;
	UMaterialInstanceDynamic* renderingMaterialInstance = nullptr;
	TSharedPtr<FPreviewScene> PreviewScene;
	FVector2D renderSize;
};