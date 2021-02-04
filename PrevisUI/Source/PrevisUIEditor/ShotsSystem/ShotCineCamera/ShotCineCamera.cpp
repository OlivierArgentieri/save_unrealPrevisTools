// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotCineCamera.h"



#include "CineCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "PrevisUIEditor/MediaPlane/PlaneKeyer.h"
#include "UObject/ConstructorHelpers.h"


#include "AssetToolsModule.h"

// VR includes
#include "SteamVRFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "AssetRegistryModule.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"


UMediaBundle* AShotCineCamera::mediaBundle = nullptr;
UMaterial* AShotCineCamera::keyingMaterial = nullptr;
UMaterialFunction* AShotCineCamera::KeyingMaterialFunction = nullptr;


AShotCineCamera::AShotCineCamera()
{
	/*
	 *  Shot : Actor
	 *		VRController			-> VRAsset controller
	 *			Camera				-> CineCamera Object
	 *				PlaneKeyer		-> Keyer
	 *
	 */

	 // Force tick in the editor
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("sceneRoot"));
	
	RootComponent = sceneRoot;

	if (IsShotValid()) return;
	 
	InitVRController();
	InitCineCamera();
	InitPlaneKeyer();
	RefreshControllerId();
	
	updatePlaneLocalTranslate();
	updatePlaneLocalScale();

	
	
	if (!IsShotValid())
		UE_LOG(LogTemp, Error, TEXT("ERROR : On construct new SHOT"));
}

void AShotCineCamera::Tick(float _deltaSeconds)
{
	Super::Tick(_deltaSeconds);

	// Update only if we actually found a controller
	if (controllerID > 0) {
		// Get controller position and orientation
		FVector _controllerPosition;
		FRotator _controllerOrientation;
		USteamVRFunctionLibrary::GetTrackedDevicePositionAndOrientation(controllerID, _controllerPosition, _controllerOrientation);

		// Apply the transform on the camera
		SetActorLocation(cameraWorldOffset + _controllerPosition);
		SetActorRotation(_controllerOrientation);
	}

}

void AShotCineCamera::PostEditChangeProperty(FPropertyChangedEvent& _propertyNameChanged)
{
	AActor::PostEditChangeProperty(_propertyNameChanged);

	if (_propertyNameChanged.Property == nullptr) return;
	if (!IsShotValid()) return;

	RefreshAllProperties();
	
	///mediaBundle->GetMaterial()->SetVe
	/*
	// plane keyer
	_propertyName = _propertyNameChanged.Property->GetFName();
	if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, cineCamera))
	{
		UE_LOG(LogTemp, Warning, TEXT("PARENT CAMERA CHANGED"));
		if (cineCamera)
		{
			if (!IsValid(keyingMaterialInstance)) return;
			keyingMaterialInstance = UMaterialInstanceDynamic::Create(keyingMaterial, NULL);

			//cineCameraparentCameraHandle = parentCamera;
			//cameraTransformEvent = cineCamera->OnCameraTransform.AddUObject(this, &APlaneKeyer::OnCameraTransform);
			//cameraZoomEvent = cineCamera->OnCameraZoom.AddUObject(this, &APlaneKeyer::OnCameraZoom);

			updatePlaneGlobalTransform();
			updatePlaneLocalTranslate();
			updatePlaneLocalScale();

			planeMeshKeyer->SetMaterial(0, keyingMaterialInstance);

			keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);
			keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);
			keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);
		}
		else
		{
			// ??? todo
		}
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, keyingMaterialProperty))
	{
		UE_LOG(LogTemp, Warning, TEXT("KEYING MATERIAL CHANGED"));
		if (keyingMaterial)
		{
			planeMeshKeyer->SetMaterial(0, keyingMaterialProperty);
		}
		else
		{
			planeMeshKeyer->SetMaterial(0, defaultMaterial);
		}
	}

	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, keyingColor))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, lumaMask))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, alphaMin))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, depht))
	{
		updatePlaneLocalTranslate();
		updatePlaneLocalScale();
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, ratio))
	{
		updatePlaneLocalScale();
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(AShotCineCamera, scale))
	{
		updatePlaneLocalScale();
	}
	else if (_propertyName == FName("X") || _propertyName == FName("Y") || _propertyName == FName("Z"))
	{
		updatePlaneGlobalTransform();
	}
	else if (_propertyName == FName("Roll") || _propertyName == FName("Pitch") || _propertyName == FName("Yaw"))
	{
		updatePlaneGlobalTransform();
	}

	OnCameraLocalOffsetPropertyChange();*/
}

bool AShotCineCamera::ShouldTickIfViewportsOnly() const
{
	return true;
}


void AShotCineCamera::InitVRController()
{
	// Load the controller 3d asset 
	VRController = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VRController"));
	VRController->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> _VRControllerAsset(TEXT("StaticMesh'/Game/Previs/Assets/VR/Controller/S_VR_Controller.S_VR_Controller'"));

	if (!IsValid(_VRControllerAsset.Object))
	{
		UE_LOG(LogTemp, Error, TEXT("Asset of tracker NOT FOUND"));
		return;
	}

	VRController->SetStaticMesh(_VRControllerAsset.Object);
}

void AShotCineCamera::InitCineCamera()
{
	if (!IsValid(VRController)) return;

	// Create the camera and attach it to the VRController asset
	cineCamera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CineCamera"));
	cineCamera->SetupAttachment(VRController);
	cineCamera->SetRelativeLocation(cameraLocalOffset);
}

void AShotCineCamera::InitPlaneKeyer()
{
	if (!IsValid(cineCamera)) return;

	// Create the ProceduralMeshComponent
//	ConstructorHelpers::FObjectFinder<UStaticMesh> _planeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	planeAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object;
	planeMeshKeyer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMeshKeyer"));

	if (!IsValid(planeAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("Asset of plane NOT FOUND"));
		return;
	}
	planeMeshKeyer->SetStaticMesh(planeAsset);
	
	planeMeshKeyer->SetupAttachment(cineCamera);
	

	planeMeshKeyer->SetRelativeRotation(FQuat(FVector::RightVector, FMath::DegreesToRadians(90)));
	planeMeshKeyer->AddRelativeRotation(FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90)));

	
//	defaultMaterial = planeMeshKeyer->GetMaterial(0);
}
void AShotCineCamera::RefreshAllProperties()
{
	//updatePlaneGlobalTransform();
	updatePlaneLocalTranslate();
	updatePlaneLocalScale();
	OnCameraLocalOffsetPropertyChange();

	
	keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);

	keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);

	keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);
}
void AShotCineCamera::RefreshControllerId()
{
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

bool AShotCineCamera::IsShotValid()
{
	return  IsValid(VRController) && IsValid(cineCamera) && IsValid(planeMeshKeyer);
}


void AShotCineCamera::updatePlaneGlobalTransform()
{
	if (!IsValid(cineCamera)) return;

//	FTransform _parentCameraTransform = cineCamera->Gxet();
//	sceneRoot->SetWorldTransform(_parentCameraTransform);

}

void AShotCineCamera::updatePlaneLocalTranslate()
{
	if (!IsValid(planeMeshKeyer)) return;
	planeMeshKeyer->SetRelativeLocation(FVector(depth, 0, 0));
}

void AShotCineCamera::updatePlaneLocalScale()
{
	if (!IsValid(cineCamera)) return;
	if (!IsValid(planeMeshKeyer)) return;

	float _fov = cineCamera->FieldOfView;
	float _width = tan((_fov / 2) * (PI / 180)) * depth;
	float _height = _width * ratio;

	FVector2D _newScale = FVector2D(_width * scale, -_height * scale);

	
	planeMeshKeyer->SetRelativeScale3D(FVector(_newScale.X, _newScale.Y, 1));

}

void AShotCineCamera::CreateMediaBundle()
{
	UE_LOG(LogTemp, Warning, TEXT("mediaBundle does not exists!!"));
	// Create a media bundle
	IAssetTools& _assetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	const FString ParentName = "/Game/Previs/";
	FString _outAssetName;
	FString _outPackageName;

	_assetTools.CreateUniqueAssetName(*(ParentName + TEXT("/MediaBundle")), TEXT(""), _outPackageName, _outAssetName);
	mediaBundle = Cast<UMediaBundle>(_assetTools.CreateAsset(_outAssetName, ParentName, UMediaBundle::StaticClass(), nullptr));

	// Create all media texture and media player
	mediaBundle->CreateInternalsEditor();
}

UMediaBundle* AShotCineCamera::GetMediaBundleAsset() const
{
	// Check if the media bundle is already created
	FAssetRegistryModule& _assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& _assetRegistry = _assetRegistryModule.Get();
	FAssetData _meshAssetList = _assetRegistry.GetAssetByObjectPath(FName("/Game/Previs/MediaBundle.MediaBundle"));
	UMediaBundle* _tempMediaBundle = Cast<UMediaBundle>(_meshAssetList.GetAsset());

	return _tempMediaBundle;
}

UMaterial* AShotCineCamera::GetKeyingMaterial()
{
	// Check if the media bundle is already created
	FAssetRegistryModule& _assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& _assetRegistry = _assetRegistryModule.Get();
	FAssetData _meshAssetList = _assetRegistry.GetAssetByObjectPath(FName("/Game/Previs/M_KeyingMaterial.M_KeyingMaterial"));
	UMaterial* _tempMediaBundle = Cast<UMaterial>(_meshAssetList.GetAsset());

	return _tempMediaBundle;
}

void AShotCineCamera::CreateKeyingMaterial()
{
	UMaterial* _temp = GetKeyingMaterial();
	keyingMaterial = _temp;
	if (IsValid(keyingMaterial)) return;
	
	// Create the material
	//keyingMaterial = NewObject<UMaterial>();
	//keyingMaterial->MaterialDomain = EMaterialDomain::MD_Surface;
	//keyingMaterial->BlendMode = EBlendMode::BLEND_Translucent;
	//keyingMaterial->SetShadingModel(EMaterialShadingModel::MSM_Unlit);
	//
	//// Set two sided to see the plane from behind
	//keyingMaterial->TwoSided = true;

	

	//////////////////////////////////////////////////
	/// KEYING MATERIAL
	//////////////////////////////////////////////////

	IAssetTools& _assetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	const FString ParentName = "/Game/Previs/";
	FString _outAssetName;
	FString _outPackageName;
	
	_assetTools.CreateUniqueAssetName(*(ParentName + TEXT("/M_KeyingMaterial")), TEXT(""), _outPackageName, _outAssetName);
	keyingMaterial = Cast<UMaterial>(_assetTools.CreateAsset(_outAssetName, ParentName, UMaterial::StaticClass(), nullptr));
	
	keyingMaterial->MaterialDomain = EMaterialDomain::MD_UI;
	keyingMaterial->BlendMode = EBlendMode::BLEND_Translucent;
	//keyingMaterial->SetShadingModel(EMaterialShadingModel::MSM_Unlit);
	
	keyingMaterial->TwoSided = true;
	
	UMaterialExpressionTextureSample* _mediaTextureExpression = NewObject<UMaterialExpressionTextureSample>(keyingMaterial);
	_mediaTextureExpression->Texture = mediaBundle->GetMediaTexture();
	_mediaTextureExpression->SamplerType = SAMPLERTYPE_External;
	keyingMaterial->Expressions.Add(_mediaTextureExpression);

	UMaterialExpressionTextureCoordinate* mediaPlateUV = NewObject<UMaterialExpressionTextureCoordinate>(keyingMaterial);
	keyingMaterial->Expressions.Add(mediaPlateUV);


	KeyingMaterialFunction = FindObject<UMaterialFunction>(nullptr, TEXT("/Engine/MaterialTemplates/Chromakeying/MF_Chromakeyer"));
	if (!KeyingMaterialFunction)
	{
		KeyingMaterialFunction = LoadObject<UMaterialFunction>(nullptr, TEXT("/Engine/MaterialTemplates/Chromakeying/MF_Chromakeyer"));
	}

	UMaterialExpressionMaterialFunctionCall* _keyingFunctionCall = NewObject<UMaterialExpressionMaterialFunctionCall>(keyingMaterial);
	_keyingFunctionCall->SetMaterialFunction(KeyingMaterialFunction);

	keyingMaterial->Expressions.Add(_keyingFunctionCall);

	_keyingFunctionCall->FunctionInputs[0].Input.Expression = _mediaTextureExpression;
	_keyingFunctionCall->FunctionInputs[1].Input.Expression = mediaPlateUV;

	keyingMaterial->EmissiveColor.Expression = _keyingFunctionCall;
	keyingMaterial->EmissiveColor.OutputIndex = 0;

	keyingMaterial->Opacity.Expression = _keyingFunctionCall;
	keyingMaterial->Opacity.OutputIndex = 1;

	// Compile the material
	keyingMaterial->PreEditChange(nullptr);
	keyingMaterial->PostEditChange();

	
	
}

void AShotCineCamera::PostActorCreated()
{
	UMediaBundle* _tempMediaBundle = GetMediaBundleAsset();
	if (!IsValid(_tempMediaBundle))	CreateMediaBundle();
	else mediaBundle = _tempMediaBundle;

	//CreateKeyingMaterial();
	CreateKeyingMaterial();
	

	//InitPlaneKeyer();

	// === test set terial parameters
	keyingMaterialInstance = NewObject<UMaterialInstanceDynamic>();
	keyingMaterialInstance = UMaterialInstanceDynamic::Create(keyingMaterial, nullptr);
	keyingMaterialInstance->SetScalarParameterValue("EnableKeyer", 1);
	
	
	planeMeshKeyer->SetMaterial(0, keyingMaterialInstance);

	// Play the video source
	mediaBundle->OpenMediaSource();

	RefreshAllProperties();

}

void AShotCineCamera::PostLoad()
{
	Super::PostLoad();
	PostActorCreated();
}

void AShotCineCamera::OnCameraLocalOffsetPropertyChange()
{
	if (!IsShotValid()) return;
	
	// Set the offset location
	cineCamera->SetRelativeLocation(cameraLocalOffset);
}
