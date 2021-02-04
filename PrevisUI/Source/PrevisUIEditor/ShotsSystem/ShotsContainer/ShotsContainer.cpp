// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotsContainer.h"


#include "PrevisUIEditor/MediaPlane/PlaneKeyer.h"
#include "PrevisUIEditor/ShotsSystem/ShotCineCamera/ShotCineCamera.h"

// Sets default values
AShotsContainer::AShotsContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	// to fix reading ptr issue on unload level 
	auto OnLevelChanged = [this](AActor*, const AActor*)
	{
		shots.Empty();
	};
	GEngine->OnLevelActorDetached().AddLambda(OnLevelChanged);
}

// Called when the game starts or when spawned
void AShotsContainer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShotsContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AShotCineCamera*> AShotsContainer::GetShots()
{
	TArray<AShotCineCamera*> _toReturn = TArray<AShotCineCamera*>();

	auto _world = GEditor->GetEditorWorldContext().World();

	if (!_world->GetCurrentLevel()) return _toReturn;
	
	for (AActor* _actor : _world->GetCurrentLevel()->Actors)
	{
		if (!IsValid(_actor)) continue;

		AShotCineCamera* _shot = Cast<AShotCineCamera>(_actor);
		if(!_shot) continue;
		_toReturn.Add(_shot);
	}
	return _toReturn;
}

void AShotsContainer::CreateNewShot()
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AShotCineCamera* _shotCamera = GWorld->SpawnActor<AShotCineCamera>(Location, Rotation, SpawnInfo);
	
	_shotCamera->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

TArray<TSharedPtr<FName>> AShotsContainer::GetShotsNames() const
{
	TArray<TSharedPtr<FName>> _toReturn;
	auto _world = GEditor->GetEditorWorldContext().World();
	if (!_world->GetCurrentLevel()) return _toReturn;

	for (AActor* _actor : _world->GetCurrentLevel()->Actors)
	{
		if (!IsValid(_actor)) continue;

		AShotCineCamera* _shot = Cast<AShotCineCamera>(_actor);
		if (!_shot) continue;
		TSharedPtr<FName> _shotName(new FName(_shot->GetName()));
		_toReturn.Add(_shotName);
	}
	return _toReturn;
}


AShotCineCamera* AShotsContainer::GetShotByName(FString _shotName) const
{
	auto _world = GEditor->GetEditorWorldContext().World();
	for (AActor* _actor : _world->GetCurrentLevel()->Actors)
	{
		if (!IsValid(_actor)) continue;

		AShotCineCamera* _shot = Cast<AShotCineCamera>(_actor);
		if (!_shot) continue;
		if (_shot->GetName() == _shotName)
			return _shot;
	}
	return nullptr;
}
