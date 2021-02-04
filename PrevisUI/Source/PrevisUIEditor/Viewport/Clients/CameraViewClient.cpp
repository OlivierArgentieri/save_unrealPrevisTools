#include "CameraViewClient.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraActor.h"
#include "CanvasItem.h"

FCameraViewClient::FCameraViewClient() :
	FEditorViewportClient(nullptr, nullptr, nullptr),
	textureRenderTarget2D(NewObject<UTextureRenderTarget2D>()),
	renderSize(FVector2D(1920, 1080)) 
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		const TArray<FEditorViewportClient*> EditorViewportClients = GEditor->GetAllViewportClients();

		// Create the scene capture elements
		sceneCaptureActor = (ASceneCapture2D*)World->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass());
		sceneCaptureActor->SetActorLabel(FString("CameraCaptureComponent"));

		sceneCaptureComponent = sceneCaptureActor->GetCaptureComponent2D();
		// Set capture component's parameters
		sceneCaptureComponent->ProjectionType = ECameraProjectionMode::Type::Perspective;
		sceneCaptureComponent->FOVAngle = 90.0f;
		sceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
		sceneCaptureComponent->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
		sceneCaptureComponent->bCaptureOnMovement = false;
		sceneCaptureComponent->bCaptureEveryFrame = false;
		sceneCaptureComponent->MaxViewDistanceOverride = -1.0f;
		sceneCaptureComponent->bAutoActivate = true;
		sceneCaptureComponent->DetailMode = EDetailMode::DM_High;

		// Set the clear color to blue (so if the viewport is blue its a problem from the source)
		textureRenderTarget2D->ClearColor = FLinearColor::Black;
		// Set the resolution of the render target (TODO : adapt the resolution to the viewport size)
		textureRenderTarget2D->InitAutoFormat(renderSize.X, renderSize.Y);

		// Link the scene capture component with the render target
		sceneCaptureComponent->TextureTarget = textureRenderTarget2D;

		// Set the editor's viewport to perpective
		SetViewportType(LVT_Perspective);
	}
}

FCameraViewClient::~FCameraViewClient()
{
	if (sceneCaptureActor)
	{
		sceneCaptureActor->Destroy();
	}
}

void FCameraViewClient::Draw(FViewport* _viewport, FCanvas* _canvas)
{
	if (sceneCaptureActor && cameraActor)
	{
		sceneCaptureActor->SetActorTransform(cameraActor->GetActorTransform());
	}

	if (sceneCaptureComponent)
	{
		// Update the render target
		sceneCaptureComponent->CaptureScene();
	}
	
	// Clear entire canvas
	_canvas->Clear(FLinearColor::Blue);

	// Draw ScenCaptureComponent texture to entire canvas
	if (textureRenderTarget2D->Resource != nullptr)
	{
		FCanvasTileItem TileItem(
			FVector2D(0, 0),
			textureRenderTarget2D->Resource,
			FVector2D(_viewport->GetRenderTargetTexture()->GetSizeX(), _viewport->GetRenderTargetTexture()->GetSizeY()),
			FLinearColor::White);

		TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Opaque;
		_canvas->DrawItem(TileItem);
	}
}

void FCameraViewClient::SetCamera(ACameraActor* _cameraActor)
{
	cameraActor = _cameraActor;
}