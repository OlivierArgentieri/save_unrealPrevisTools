// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShotsContainer.generated.h"

class AShotCineCamera;
class UStaticMeshComponent;
class APlaneKeyer;
UCLASS()
class PREVISUIEDITOR_API AShotsContainer : public AActor
{
	GENERATED_BODY()

private:
	
public:	
	// Sets default values for this actor's properties
	AShotsContainer();
	
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AShotCineCamera*> shots;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// for debug :
	TArray<AShotCineCamera*> GetShots();

	void CreateNewShot();
	TArray<TSharedPtr<FName>> GetShotsNames()const ;
	AShotCineCamera* GetShotByName(FString _shotName) const;
};