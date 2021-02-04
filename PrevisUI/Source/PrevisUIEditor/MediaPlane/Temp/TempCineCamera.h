#pragma once
#include "CoreMinimal.h"
#include "CineCameraActor.h"

#include "TempCineCamera.generated.h"


UCLASS()
class ATempCineCamera : public ACineCameraActor
{
	GENERATED_BODY()
	DECLARE_EVENT(ATempCineCamera, FCineCameraEvent)

public:
	FCineCameraEvent OnCameraTransform;
	FCineCameraEvent OnCameraZoom;

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

private:
	void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
};