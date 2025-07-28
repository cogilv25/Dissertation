#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ISLSystem.h"
#include "AMaze.generated.h"

// This would be a Multi-Layer Indiscrete Stochastic Pseudo L-System which is a bit of a mouthful..


class UProceduralMeshComponent;
class UMaterialInterface;

struct AmazeTurtleState
{
	int32    x, y, z;
	int32 dirX, dirY, dirZ;
	int32 upDX, upDY, upDZ;
};

UCLASS()
class DISSERTATION_API AMaze : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Generation") int32 generationSpeed = 100;
	UPROPERTY(EditAnywhere, Category = "Generation") float minimumGenerationTime = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Generation") FIntVector dimensions = FIntVector(30, 30, 10);
	UPROPERTY(EditAnywhere, Category = "Generation") FIntVector startPos = FIntVector(0, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Generation") TArray<USISLSystem*> stages;
	UPROPERTY(EditAnywhere, Category = "Rendering") bool pipeMode;
	UPROPERTY(EditAnywhere, Category = "Rendering") FVector cellDimensions = FVector(300.0, 300.0, 300.0);
	UPROPERTY(EditAnywhere, Category = "Rendering") UMaterialInterface* material;
protected:
	UPROPERTY() UProceduralMeshComponent* mesh;

	TArray<FVector> vertices;
	TArray<int> indices;
	TArray<FVector2D> uv0;
	TArray<uint8> cells;
	FString lSystemState = "";
	uint32 currentGenerationStage = 0;
	bool newMeshReady = false;
	bool generationComplete = false;
	bool turtleComplete = false;
	float generationTime = 0.0f;

	int32 turtleStringPosition = 0;
	FIntVector direction = FIntVector(1, 0, 0);
	FIntVector upDir = FIntVector(0, 0, 1);
	FIntVector position = FIntVector(0, 0, 0);
	TArray<AmazeTurtleState> stateStack;

	FIntVector renderPosition = FIntVector(0, 0, 0);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& pce) override;

public:
	AMaze();
	
	void turtleStep(FString::ElementType* CharPos);
	void runTurtle(FString input);
	bool runTurtle(int32 steps);
	void renderCellWall(double x, double y, double z, uint8 index);
	void renderCell(double x, double y, double z);
};

