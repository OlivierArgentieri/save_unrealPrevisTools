#pragma once

#include "EditorViewportClient.h"

//Forward declaration
class UTextureRenderTarget2D;
class ASceneCapture2D;
class USceneCaptureComponent2D;

class FCameraViewClient : public FEditorViewportClient
{
public:
	FCameraViewClient();
	~FCameraViewClient();

	// Called by the FViewport that will be connected to this FWindowViewportClient's Draw function 
	virtual void Draw(FViewport* _viewport, FCanvas* _canvas) override;

	void SetCamera(ACameraActor* _cameraActor);

private:
	UTextureRenderTarget2D* textureRenderTarget2D = nullptr;
	ACameraActor* cameraActor = nullptr;
	ASceneCapture2D* sceneCaptureActor = nullptr;
	USceneCaptureComponent2D* sceneCaptureComponent = nullptr;
	FVector2D renderSize;
};