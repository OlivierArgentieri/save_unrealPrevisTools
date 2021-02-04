#include "PlaneKeyer.h"
#include "PlaneMesh.h"
#include "Temp/TempCineCamera.h"

#include "BlackmagicMediaSource.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialFunction.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistryModule.h"

#include <math.h>

#include "../ShotsSystem/ShotCineCamera/ShotCineCamera.h"


// Called when created
APlaneKeyer::APlaneKeyer()
{
	// Allow the actor to tick
	PrimaryActorTick.bCanEverTick = true;

	// Set the SceneComponent as root
	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("sceneRoot"));
	RootComponent = sceneRoot;

	// Create the ProceduralMeshComponent
	planeMesh = CreateDefaultSubobject<UPlaneMesh>(TEXT("planeMesh"));

	// Create the ProceduralMeshComponent data
	planeMesh->CreatePlane(FVector2D(scale*100, scale*100*ratio));

	// Parent the SceneComponent to the ProceduralMeshComponent
	planeMesh->SetupAttachment(sceneRoot);

	defaultMaterial = planeMesh->GetMaterial(0);

	
}

// Called when the game starts or when spawned
void APlaneKeyer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlaneKeyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the actor is already in a level and the level is opened
void APlaneKeyer::PostLoad()
{
	Super::PostLoad();
}

// Called when a UProperty is changed
void APlaneKeyer::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	if (e.Property == NULL) { return; }

	FName _propertyName = e.Property->GetFName();
	if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, parentCamera)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("PARENT CAMERA CHANGED"));
		if (parentCamera)
		{
			if (!IsValid(keyingMaterialInstance)) return;
			keyingMaterialInstance = UMaterialInstanceDynamic::Create(keyingMaterial, NULL);

			parentCameraHandle = parentCamera;
			cameraTransformEvent = parentCameraHandle->OnCameraTransform.AddUObject(this, &APlaneKeyer::OnCameraTransform);
			cameraZoomEvent = parentCameraHandle->OnCameraZoom.AddUObject(this, &APlaneKeyer::OnCameraZoom);

			updatePlaneGlobalTransform();
			updatePlaneLocalTranslate();
			updatePlaneLocalScale();

			if (!IsValid(mediaPlayer))InitMediaPlayer();;
			mediaPlayer->OpenSource(mediaSourceFile);

			if (!IsValid(mediaTexture)) InitMediaTexture();
			mediaTexture->UpdateResource();
			
			planeMesh->SetMaterial(0, keyingMaterialInstance);

			keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);
			keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);
			keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);
		}
		else
		{
			parentCameraHandle->OnCameraTransform.Remove(cameraTransformEvent);
			parentCameraHandle->OnCameraZoom.Remove(cameraZoomEvent);
			parentCameraHandle = nullptr;
		}
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, keyingMaterialProperty))
	{
		UE_LOG(LogTemp, Warning, TEXT("KEYING MATERIAL CHANGED"));
		if (keyingMaterial)
		{
			planeMesh->SetMaterial(0, keyingMaterialProperty);
		}
		else
		{
			planeMesh->SetMaterial(0, defaultMaterial);
		}
	}

	
	
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, keyingColor))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, lumaMask))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, alphaMin))
	{
		if (!IsValid(keyingMaterialInstance)) return;
		keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, depht))
	{
		updatePlaneLocalTranslate();
		updatePlaneLocalScale();
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, ratio))
	{
		updatePlaneLocalScale();
	}
	else if (_propertyName == GET_MEMBER_NAME_CHECKED(APlaneKeyer, scale))
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
}

void APlaneKeyer::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	updatePlaneGlobalTransform();
}

void APlaneKeyer::SetCamera(ATempCineCamera* _cineCamera)
{
	if (!_cineCamera) return;
	
	UE_LOG(LogTemp, Warning, TEXT("PARENT CAMERA CHANGED"));

	keyingMaterialInstance = UMaterialInstanceDynamic::Create(keyingMaterial, NULL);

	parentCamera = _cineCamera;
	parentCameraHandle = parentCamera;
	cameraTransformEvent = parentCameraHandle->OnCameraTransform.AddUObject(this, &APlaneKeyer::OnCameraTransform);
	cameraZoomEvent = parentCameraHandle->OnCameraZoom.AddUObject(this, &APlaneKeyer::OnCameraZoom);

	updatePlaneGlobalTransform();
	updatePlaneLocalTranslate();
	updatePlaneLocalScale();

	if (!IsValid(mediaPlayer))InitMediaPlayer();

	if (!IsValid(mediaSourceFile)) InitMediaSource();
	mediaPlayer->OpenSource(mediaSourceFile);
	
	if (!IsValid(mediaTexture)) InitMediaTexture();
	mediaTexture->UpdateResource();
	
	planeMesh->SetMaterial(0, keyingMaterialInstance);

	keyingMaterialInstance->SetVectorParameterValue("Key Color", keyingColor);
	keyingMaterialInstance->SetScalarParameterValue("LumaMask", lumaMask);
	keyingMaterialInstance->SetScalarParameterValue("Alpha Min", alphaMin);

	updatePlaneGlobalTransform();
	updatePlaneLocalTranslate();
	updatePlaneLocalScale();


	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Vector : %s"), *parentCamera->GetActorTransform().ToString()));
}

void APlaneKeyer::PostActorCreated()
{
	// Check if the media bundle is already created
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	FAssetData MeshAssetList = AssetRegistry.GetAssetByObjectPath(FName("/Game/Previs/MediaBundle.MediaBundle"));
	mediaBundle = Cast<UMediaBundle>(MeshAssetList.GetAsset());

	if (!mediaBundle)
	{
		UE_LOG(LogTemp, Warning, TEXT("mediaBundle does not exists!!"));
		// Create a media bundle
		IAssetTools& AssetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		const FString ParentName = "/Game/Previs/";
		FString OutAssetName;
		FString OutPackageName;

		AssetTools.CreateUniqueAssetName(*(ParentName + TEXT("/MediaBundle")), TEXT(""), OutPackageName, OutAssetName);
		mediaBundle = Cast<UMediaBundle>(AssetTools.CreateAsset(OutAssetName, ParentName, UMediaBundle::StaticClass(), nullptr));

		mediaBundle->CreateInternalsEditor();
	}

	mediaTexture = mediaBundle->GetMediaTexture();

	// Set media bundle source
	mediaBundle->GetMediaSource();

	// Create the material
	keyingMaterial = NewObject<UMaterial>();
	keyingMaterial->MaterialDomain = EMaterialDomain::MD_Surface;
	keyingMaterial->BlendMode = EBlendMode::BLEND_Translucent;
	keyingMaterial->SetShadingModel(EMaterialShadingModel::MSM_Unlit);

	// Set two sided to see the plane from behind
	keyingMaterial->TwoSided = true;

	keyingMaterialInstance = NewObject<UMaterialInstanceDynamic>();

	//////////////////////////////////////////////////
	/// KEYING MATERIAL
	//////////////////////////////////////////////////

	UMaterialExpressionTextureSample* mediaTextureExpression = NewObject<UMaterialExpressionTextureSample>(keyingMaterial);
	mediaTextureExpression->Texture = mediaTexture;
	mediaTextureExpression->SamplerType = SAMPLERTYPE_External;
	keyingMaterial->Expressions.Add(mediaTextureExpression);

	UMaterialExpressionTextureCoordinate* mediaPlateUV = NewObject<UMaterialExpressionTextureCoordinate>(keyingMaterial);
	keyingMaterial->Expressions.Add(mediaPlateUV);


	KeyingMaterialFunction = FindObject<UMaterialFunction>(NULL, TEXT("/Engine/MaterialTemplates/Chromakeying/MF_Chromakeyer"));
	if (!KeyingMaterialFunction)
	{
		KeyingMaterialFunction = LoadObject<UMaterialFunction>(NULL, TEXT("/Engine/MaterialTemplates/Chromakeying/MF_Chromakeyer"));
	}

	UMaterialExpressionMaterialFunctionCall* KeyingFunctionCall = NewObject<UMaterialExpressionMaterialFunctionCall>(keyingMaterial);
	KeyingFunctionCall->SetMaterialFunction(KeyingMaterialFunction);
	keyingMaterial->Expressions.Add(KeyingFunctionCall);
	KeyingFunctionCall->FunctionInputs[0].Input.Expression = mediaTextureExpression;
	KeyingFunctionCall->FunctionInputs[1].Input.Expression = mediaPlateUV;
	keyingMaterial->EmissiveColor.Expression = KeyingFunctionCall;
	keyingMaterial->EmissiveColor.OutputIndex = 0;
	keyingMaterial->Opacity.Expression = KeyingFunctionCall;
	keyingMaterial->Opacity.OutputIndex = 1;

	// Compile the material
	keyingMaterial->PreEditChange(NULL);
	keyingMaterial->PostEditChange();

	// Play the video source
	mediaBundle->OpenMediaSource();
}

// Allow the actor to tick in the viewport
bool APlaneKeyer::ShouldTickIfViewportsOnly() const
{
	return true;
}

void APlaneKeyer::OnCameraTransform()
{
	updatePlaneGlobalTransform();
}

void APlaneKeyer::OnCameraZoom()
{
	updatePlaneLocalScale();
}

void APlaneKeyer::InitMediaTexture()
{
	mediaTexture = NewObject<UMediaTexture>();
	mediaTexture->SetDefaultMediaPlayer(mediaPlayer);
	mediaTexture->Filter = TextureFilter::TF_Bilinear;
}

void APlaneKeyer::InitMediaPlayer()
{
	mediaPlayer = NewObject<UMediaPlayer>();
	if (sourceType == EMediaSourceType::File) { mediaPlayer->OpenSource(mediaSourceFile); }
	if (sourceType == EMediaSourceType::Live) { mediaPlayer->OpenSource(mediaSourceBlackMagic); }
	mediaPlayer->SetLooping(true);
}

void APlaneKeyer::InitMediaSource()
{
	mediaSourceFile = NewObject<UFileMediaSource>();
	mediaSourceFile->SetFilePath(filepath);
}

void APlaneKeyer::updatePlaneGlobalTransform()
{
	if (!parentCamera) { return; }

	FTransform _parentCameraTransform = parentCamera->GetActorTransform();
	sceneRoot->SetWorldTransform(_parentCameraTransform);
}

void APlaneKeyer::updatePlaneLocalTranslate()
{
	if (!parentCamera) { return; }

	planeMesh->SetRelativeTransform(FTransform(FVector(depht, 0, 0)));
}

void APlaneKeyer::updatePlaneLocalScale()
{
	if (!parentCamera) { return; }

	float _fov = parentCameraHandle->GetCameraComponent()->FieldOfView;
	float _width = tan((_fov / 2) * (PI / 180)) * depht;
	float _height = _width * ratio;

	planeMesh->UpdatePlane(FVector2D(_width * scale, -_height * scale));
}