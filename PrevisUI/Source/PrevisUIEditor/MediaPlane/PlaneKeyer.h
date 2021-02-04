#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "BlackmagicDeviceProvider.h"
#include "MediaBundle.h"

#include "PlaneKeyer.generated.h"

class ATempCineCamera;
// Forward declaration
class UPlaneMesh;
class ATempCineCamera;
class USceneComponent;
class UBlackmagicMediaSource;

UENUM(BlueprintType)
enum class EMediaSourceType : uint8
{
	File,
	Live
};

UCLASS(hideCategories = (Rendering, Replication, Input, Actor, Base, Collision, Shape, LOD, Cooking, Audio, Video, Debug))
class APlaneKeyer : public AActor
{
	GENERATED_BODY()

public:
	APlaneKeyer();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLoad() override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	void OnCameraTransform();
	void OnCameraZoom();
	void SetCamera(ATempCineCamera* _cineCamera);
	
protected:
	virtual void BeginPlay() override;
	void PostActorCreated() override;
	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = MediaInput)
	ATempCineCamera* parentCamera;

	UPROPERTY(EditAnywhere, Category = MediaInput)
	EMediaSourceType sourceType = EMediaSourceType::Live;

	UPROPERTY(EditAnywhere, Category = MediaInput)
	FString filepath = FString("C:/Users/Simon/Downloads/JeanClaudeVanDamme_Green_screen_footage.mp4");

	UPROPERTY(EditAnywhere, Instanced, Category = MediaInput)
	UBlackmagicMediaSource* mediaSourceBlackMagic;
	
	UPROPERTY(EditAnywhere, Category = Keying)
	FLinearColor keyingColor = FLinearColor(0.00403, 0.366, 0.0185, 1);

	UPROPERTY(EditAnywhere, Category = Keying)
	float lumaMask = 0;

	UPROPERTY(EditAnywhere, Category = Keying)
	float alphaMin = 0;

	UPROPERTY(EditAnywhere, Category = Tweak)
	float depht = 500;

	UPROPERTY(EditAnywhere, Category = Tweak)
	float ratio = 0.5625;
	
	UPROPERTY(EditAnywhere, Category = Tweak)
	float scale = 1.92;

	UPROPERTY()
	ATempCineCamera* parentCameraHandle;

	UPROPERTY()
	UPlaneMesh* planeMesh;
	UPROPERTY()
	USceneComponent* sceneRoot;

	UPROPERTY()
	UMaterialInterface* defaultMaterial;

	UPROPERTY()
	UMaterialInterface* keyingMaterialProperty;

	FDelegateHandle cameraTransformEvent;

	FDelegateHandle cameraZoomEvent;

	void InitMediaTexture();
	void InitMediaPlayer();
	void InitMediaSource();

	void updatePlaneGlobalTransform();
	void updatePlaneLocalTranslate();
	void updatePlaneLocalScale();

	UPROPERTY()
	UMediaBundle* mediaBundle;

	UPROPERTY()
	UFileMediaSource* mediaSourceFile = nullptr;

	UPROPERTY()
	UMediaPlayer* mediaPlayer = nullptr;

	UPROPERTY()
	UMediaTexture* mediaTexture = nullptr;

	UPROPERTY()
	UMaterial* keyingMaterial = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* keyingMaterialInstance = nullptr;

	UPROPERTY()
	UMaterialFunction* KeyingMaterialFunction = nullptr;
};