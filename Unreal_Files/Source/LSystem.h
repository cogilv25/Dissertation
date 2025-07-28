#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSystem.generated.h"

USTRUCT()
struct FLSystemRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) FString find;
	UPROPERTY(EditAnywhere) FString replace;
};

UCLASS()
class DISSERTATION_API ULSystem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) FString axiom;
	UPROPERTY(EditAnywhere) TArray<FLSystemRule> rules;
protected:
	FString _state;
	uint32 it;
	void (*parentLSystemChangedFunction)(void*) = 0;
	void* parent = 0;
public:
	ULSystem();

	void setParentLSystemChangedFunction(void(*parentLSystemChangedFunction)(void*), void* parent);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& pce) override;

	uint32 iteration();
	FString state();

	void stepForward();
	void reset();
};

