#include "VRControllerCamera.h"

#include "UObject/ConstructorHelpers.h"

// VR includes
#include "SteamVRFunctionLibrary.h"

AVRControllerCamera::AVRControllerCamera(const FObjectInitializer& ObjectInitializer)
{
	// Force tick in the editor
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create an arrow component
	// useful to add an offset to the camera based on the physical location of the controller
	// UArrowComponent *arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraOffset"));

	// Load the controller 3d asset
	VRController = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VRController"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> VRControllerAsset(TEXT("StaticMesh'/Game/Previs/Assets/VR/Controller/S_VR_Controller.S_VR_Controller'"));
	VRController->SetStaticMesh(VRControllerAsset.Object);

	RootComponent = VRController;

	// Create the camera and attach it to the arrow
	camera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CineCamera"));
	camera->SetupAttachment(VRController);
	camera->SetRelativeLocation(cameraLocalOffset);

	// Getting the VR controller id
	TArray<int32> controllerIds;
	USteamVRFunctionLibrary::GetValidTrackedDeviceIds(ESteamVRTrackedDeviceType::Controller, controllerIds);

	// If we found a controller
	if (controllerIds.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Valid tracked Id : %s"), *FString::FromInt(controllerIds[0]));
		controllerID = controllerIds[0];
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No valid tracker were found!!"));
	}
}

void AVRControllerCamera::BeginPlay()
{
	Super::BeginPlay();
}

void AVRControllerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update only if we actually found a controller
	if (controllerID > 0) {
		// Get controller position and orientation
		FVector controllerPosition;
		FRotator controllerOrientation;
		USteamVRFunctionLibrary::GetTrackedDevicePositionAndOrientation(controllerID, controllerPosition, controllerOrientation);

		// Apply the transform on the camera
		SetActorLocation(cameraWorldOffset + controllerPosition);
		SetActorRotation(controllerOrientation);
	}
}

void AVRControllerCamera::OnCameraLocalOffsetPropertyChange(FName PropertyName)
{
	if (!(PropertyName == GET_MEMBER_NAME_CHECKED(AVRControllerCamera, cameraLocalOffset))) return;


	// Set the offset location
	camera->SetRelativeLocation(cameraLocalOffset);

}

void AVRControllerCamera::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	OnCameraLocalOffsetPropertyChange(PropertyName);
	Super::PostEditChangeProperty(e);
}