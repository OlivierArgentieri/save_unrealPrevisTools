#pragma once

#include "EditorViewportClient.h"

//Forward declaration
class UTextureRenderTarget2D;
class ASceneCapture2D;
class USceneCaptureComponent2D;

class FSceneViewClient : public FEditorViewportClient
{
public:
	FSceneViewClient();

	// Called by the FViewport that will be connected to this FWindowViewportClient's Draw function 
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

};