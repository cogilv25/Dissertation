#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ISLSystem.generated.h"


//Stochastic L-System Rule
USTRUCT()
struct FSLSystemRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) FString find;
	UPROPERTY(EditAnywhere) FString replace;
	UPROPERTY(EditAnywhere) float weight = 1.0f;
};

//Internally used version of SLS rule.
USTRUCT()
struct F_SLSystemRule
{
	GENERATED_BODY()
public:
	FString find;
	TArray<FString> replacers;
	TArray<float> endPoints;
};

//Indiscrete Stochastic L-System
UCLASS()
class DISSERTATION_API UISLSystem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) FString axiom;
	UPROPERTY(EditAnywhere) TArray<FSLSystemRule> rules;
protected:
	bool initialized = false;
	FString _state;
	FString prevState;
	uint32 it;
	void (*parentLSystemChangedFunction)(void*) = 0;
	void* parent = 0;
	TArray<F_SLSystemRule> _rules;
public:
	UISLSystem();

	void setParentLSystemChangedFunction(void(*parentLSystemChangedFunction)(void*), void* parent);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& pce) override;

	uint32 iteration();
	uint32 symbolIndex = 0;
	FString state();

	bool stepForward(uint32 maxSymbols);
	void stepForward();
	bool isUndefined();
	void reset();
	void initialize();
};

// Static Indiscrete Stochastic L-System
UCLASS()
class DISSERTATION_API USISLSystem : public UISLSystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) uint32 iterations;

	bool generate(uint32 maxSymbols);
	void generate();
};