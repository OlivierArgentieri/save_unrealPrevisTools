// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "PrevisUIEditor/ShotsSystem/ShotsContainer/ShotsContainer.h"
#include "UObject/ConstructorHelpers.h"


#include "ShotCineCamera.generated.h"

/**
 * 
 */

class UMediaTexture;
class UMediaPlayer;
class UFileMediaSource;
class UMediaBundle;
class UBlackmagicMediaSource;


class APlaneKeyer;
UCLASS()
class PREVISUIEDITOR_API AShotCineCamera : public AActor
{
	GENERATED_BODY()

	// On Destroy Delegate
	//DECLARE_DELEGATE_OneParam(FOnDestroyShot, FString, _objectName); FOnDestroyShot OnDestroyShot;
	//DECLARE_EVENT(ATempCineCamera, FCineCameraEvent)

	
private:
	AShotCineCamera();
	virtual void Tick(float _deltaSeconds) override;
public:
	//FCineCameraEvent OnCameraTransform;
	//FCineCameraEvent OnCameraZoom;

	UPROPERTY()
	UStaticMesh* planeAsset;

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& _propertyNameChanged) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	
	void InitVRController();
	void InitCineCamera();
	void InitPlaneKeyer();
	void RefreshAllProperties();
	void RefreshControllerId();

	
	bool IsShotValid();
	
private:
	
	//void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

private:

	/// === Shot attribute ===
	///
	UPROPERTY()
	USceneComponent* sceneRoot;
	///
	/// End shot attribute
	
	/// ===  VR attribute ===
	/// 
	// The device id of the controller

	UPROPERTY()
	int32 controllerID = -1;

	// Controller component as static mesh
	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* VRController;

	// Camera component
	UPROPERTY(EditAnywhere, Category = Component)
	UCineCameraComponent* cineCamera;

	// Camera world offset
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector cameraWorldOffset;

	// Camera local offset from root component
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector cameraLocalOffset = FVector(30, 0, -30);
	///
	/// End === VR Attribute ===

	
	/// === Plane Attributes ===
	///
	UPROPERTY(EditAnywhere, Category = Component)
	UStaticMeshComponent* planeMeshKeyer;

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
	float depth = 500;

	UPROPERTY(EditAnywhere, Category = Tweak)
	float ratio = 0.5625;

	UPROPERTY(EditAnywhere, Category = Tweak)
	float scale = 0.02; // 1.92


	//UPROPERTY()
	//aAShotCineCamera* parentCameraHandle; todo

	UPROPERTY(EditAnywhere, Category = Keying)
	UMaterialInterface* defaultMaterial;

	UPROPERTY(EditAnywhere, Category = Keying)
	UMaterialInterface* keyingMaterialProperty;

	void updatePlaneGlobalTransform();
	void updatePlaneLocalTranslate();
	void updatePlaneLocalScale();
	void CreateMediaBundle();
	UMediaBundle* GetMediaBundleAsset() const;
	
	static UMaterial* GetKeyingMaterial();
	static void CreateKeyingMaterial();
	void virtual PostActorCreated() override;
	void virtual PostLoad() override;

	static UMediaBundle* mediaBundle;

	
	static UMaterial* keyingMaterial;

	
	UPROPERTY(EditAnywhere, Category = Keying)
	UMaterialInstanceDynamic* keyingMaterialInstance;

	static UMaterialFunction* KeyingMaterialFunction;
	
	///
	/// === End Attributes ===
public:
	// === Getters/Setters ===


	// Called when the camera local offset is changed
	void OnCameraLocalOffsetPropertyChange();
	
};
