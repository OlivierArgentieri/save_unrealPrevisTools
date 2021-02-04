#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"

#include "CineCameraComponent.h"
// #include "Components/StaticMeshComponent.h"

#include "VRControllerCamera.generated.h"

/**
 *
 */
UCLASS()
class PREVISUIEDITOR_API AVRControllerCamera : public AActor
{
	GENERATED_BODY()

private:
	// The device id of the controller
	int32 controllerID = -1;

	// Controller component as static mesh
	UPROPERTY()
	UStaticMeshComponent* VRController;

	// Camera component
	UPROPERTY()
	UCineCameraComponent* camera;

	// Camera world offset
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector cameraWorldOffset;

	// Camera local offset from root component
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector cameraLocalOffset = FVector(30, 0, -30);

	// Called when the camera local offset is changed
	void OnCameraLocalOffsetPropertyChange(FName PropertyName);

protected:
	virtual void BeginPlay() override;

public:
	AVRControllerCamera(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

	virtual bool ShouldTickIfViewportsOnly() const override
	{
		return true;
	}
};