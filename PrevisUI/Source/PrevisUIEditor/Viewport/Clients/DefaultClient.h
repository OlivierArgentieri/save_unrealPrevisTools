#pragma once

#include "EditorViewportClient.h"

//Forward declaration
class UTextureRenderTarget2D;
class FPreviewScene;

class FDefaultClient : public FEditorViewportClient
{
public:
	FDefaultClient();

	// Called by the FViewport that will be connected to this FWindowViewportClient's Draw function 
	virtual void Draw(FViewport* _viewport, FCanvas* _canvas) override;
};