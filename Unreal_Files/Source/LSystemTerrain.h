#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSystem.h"
#include "LSystemTerrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class DISSERTATION_API ALSystemTerrain : public AActor
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, Category = "LSystem") uint32 generatorIterations;
	UPROPERTY(EditAnywhere, Category = "LSystem") ULSystem* generator;

	UPROPERTY(EditAnywhere, Category = "Generation") uint32 width = 0;
	UPROPERTY(EditAnywhere, Category = "Generation") uint32 height = 0;
	UPROPERTY(EditAnywhere, Category = "Generation") float amplify = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Generation") int32 offsetX = 0;
	UPROPERTY(EditAnywhere, Category = "Generation") int32 offsetY = 0;

	UPROPERTY(EditAnywhere, Category = "Rendering") double cellWidth = 100.0;
	UPROPERTY(EditAnywhere, Category = "Rendering") double cellHeight = 100.0;
	UPROPERTY(EditAnywhere, Category = "Rendering") UMaterialInterface* material;
	// TODO: Test Hiding this..?
	UPROPERTY(EditAnywhere, Category = "Model") UProceduralMeshComponent* mesh;
protected:
	virtual void OnConstruction(const FTransform& trans) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& pce) override;

	TArray<float> runTurtle(FString input);
	void LSystemChanged();
public:	
	// Sets default values for this actor's properties
	ALSystemTerrain();
};