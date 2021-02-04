#include "SceneViewClient.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraActor.h"
#include "CanvasItem.h"

FSceneViewClient::FSceneViewClient() :
	FEditorViewportClient(nullptr, nullptr, nullptr)
{
	// Set the editor's viewport to perpective
	SetViewportType(LVT_Perspective);
}

void FSceneViewClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);
}
