#include "ISLSystem.h"

UISLSystem::UISLSystem()
{
	_state = FString(axiom);
	it = 0;
}

void UISLSystem::setParentLSystemChangedFunction(void(*pF)(void*), void* p)
{
	parentLSystemChangedFunction = pF;
	parent = p;
}

void UISLSystem::PostEditChangeProperty(FPropertyChangedEvent& pce)
{
	if(parentLSystemChangedFunction != 0)
		parentLSystemChangedFunction(parent);
}

uint32 UISLSystem::iteration()
{
	return it;
}

FString UISLSystem::state()
{
	return _state;
}

bool UISLSystem::stepForward(uint32 mS)
{
	if (!initialized) initialize();

	uint32 nRules = _rules.Num();
	uint32 nChars = prevState.Len();

	uint32 i = symbolIndex;
	for (; i < nChars && (i - symbolIndex) < mS; ++i)
	{
		bool replaced = false;
		for (uint32 j = 0; j < nRules; ++j)
		{
			uint32 rLen = _rules[j].find.Len();
			if (_rules[j].find.Equals(prevState.Mid(i, rLen), ESearchCase::CaseSensitive))
			{
				float select = FMath::RandRange(0.0f, 1.0f);
				int32 k = 0; while (_rules[j].endPoints[k] < select) ++k;
				_state += _rules[j].replacers[k];
				replaced = true;
				i += rLen - 1;
				break;
			}
		}
		if (!replaced)
			_state += prevState.Mid(i, 1);
	}

	if (i >= nChars)
	{
		prevState = _state;
		symbolIndex = 0;
		++it;
		return true;
	}

	symbolIndex = i;
	return false;
}

void UISLSystem::stepForward()
{
	if (!initialized) initialize();

	uint32 nRules = _rules.Num();
	uint32 nChars = prevState.Len();

	for (uint32 i = 0; i < nChars; ++i)
	{
		bool replaced = false;
		for (uint32 j = 0; j < nRules; ++j)
		{
			uint32 rLen = _rules[j].find.Len();
			if (_rules[j].find.Equals(prevState.Mid(i, rLen), ESearchCase::CaseSensitive))
			{
				float select = FMath::RandRange(0.0f, 1.0f);
				int32 k = 0; while (_rules[j].endPoints[k] < select) ++k;
				_state += _rules[j].replacers[k];
				replaced = true;
				i += rLen - 1;
				break;
			}
		}
		if (!replaced)
			_state += prevState.Mid(i, 1);
	}

	prevState = _state;
	symbolIndex = 0;
	++it;
}

bool UISLSystem::isUndefined()
{
	return symbolIndex != 0;
}

void UISLSystem::reset()
{
	it = 0;
	symbolIndex = 0;
	prevState = FString(axiom);
	_state = "";
}

bool USISLSystem::generate(uint32 maxSymbols)
{
	if (!initialized) initialize();

	if (it < iterations)
	{
		stepForward(maxSymbols);
		return false;
	}

	return true;
}

void USISLSystem::generate()
{
	if (!initialized) initialize();

	while (it < iterations)
		stepForward();
}

void UISLSystem::initialize()
{
	/* 
	*  Technically there's a bug where a rule with a lower precedence 
	*   gets selected over a higher one where it shares a search term
	*   with a rule that is higher than both.. e.g.
	*
	*   w: 0.5, a -> aac
	*   w: 1.0, ab -> aff
	*   w: 0.5, a -> df
	*
	*   Here because rule 1 and 3 are combined as one rule, rule 3 takes
	*   precedence over rule 2 where really if rule 3 was selected over 
	*   rule 1 then we should evaluate if rule 2 can be applied first.. 
	*/

	TArray<bool> mask;
	mask.Init(false, rules.Num());
	for (int32 i = 0; i < rules.Num(); ++i)
	{
		// Skip processed rules.
		if (mask[i]) continue;

		F_SLSystemRule rule;
		rule.find = rules[i].find;
		// Prevents divide by 0 if the user enters all 0's while being
		//  unlikely to significantly affect any forseen use-case.
		float totalWeight = 1.0E-20F;

		// Find matching rules
		for (int32 j = i; j < rules.Num(); ++j)
		{
			if (mask[j]) continue;
			if (rule.find == rules[j].find)
			{
				rule.replacers.Add(rules[j].replace);
				rule.endPoints.Add(rules[j].weight);
				totalWeight += rules[j].weight;
				mask[j] = true;
			}
		}

		// When choosing a rule to use during generation a number
		//  between 0 and 1 is generated and if it is less than a
		//  rules endPoint that rule is used.
		rule.endPoints[0] /= totalWeight;
		for (int32 j = 1; j < rule.endPoints.Num(); ++j)
			rule.endPoints[j] = rule.endPoints[j - 1] + rule.endPoints[j] / totalWeight;
		// Rounding errors could make the last endpoint != 1...
		//  not a bug I wish to hunt down later!
		rule.endPoints[rule.endPoints.Num() - 1] = 1.0f;

		_rules.Add(rule);
	}

	prevState = FString(axiom);
	_state = "";
	initialized = true;
}
