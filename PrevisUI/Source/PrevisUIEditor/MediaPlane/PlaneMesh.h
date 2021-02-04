#pragma once
#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

#include "PlaneMesh.generated.h"

UCLASS()
class UPlaneMesh : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	void CreatePlane(FVector2D _size);
	void UpdatePlane(FVector2D _size);
};