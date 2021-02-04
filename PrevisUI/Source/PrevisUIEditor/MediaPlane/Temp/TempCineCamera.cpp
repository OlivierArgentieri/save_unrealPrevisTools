#include "TempCineCamera.h"
#include "Camera/CameraComponent.h"

void ATempCineCamera::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	if (e.Property == NULL) { return; }

	FName _propertyName = e.Property->GetFName();
	if (_propertyName == FName("X") || _propertyName == FName("Y") || _propertyName == FName("Z"))
	{
		OnCameraTransform.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("CAMERA MOVED"));
	}
	else if (_propertyName == FName("Roll") || _propertyName == FName("Pitch") || _propertyName == FName("Yaw"))
	{
		UE_LOG(LogTemp, Warning, TEXT("CAMERA ROTATED"));
		OnCameraTransform.Broadcast();
	}
	else if (_propertyName == FName("CameraComponent"))
	{
		float _fov = GetCameraComponent()->FieldOfView;
		UE_LOG(LogTemp, Warning, TEXT("CAMERA COMPONENT CHANGED FOV : %f"), _fov);
		OnCameraZoom.Broadcast();
	}
}

void ATempCineCamera::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	OnCameraTransform.Broadcast();
}