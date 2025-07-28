#include "LSystem.h"


ULSystem::ULSystem()
{
	_state = FString(axiom);
	it = 0;
}

void ULSystem::setParentLSystemChangedFunction(void(*pF)(void*), void* p)
{
	parentLSystemChangedFunction = pF;
	parent = p;
}

void ULSystem::PostEditChangeProperty(FPropertyChangedEvent& pce)
{
	if(parentLSystemChangedFunction != 0)
		parentLSystemChangedFunction(parent);
}

uint32 ULSystem::iteration()
{
	return it;
}

FString ULSystem::state()
{
	return _state;
}

void ULSystem::stepForward()
{
	uint32 nRules = rules.Num();
	uint32 nChars = _state.Len();
	FString newState = FString("");

	for (uint32 i = 0; i < nChars; ++i)
	{
		bool replaced = false;
		for (uint32 j = 0; j < nRules; ++j)
		{
			uint32 rLen = rules[j].find.Len();
			if (rules[j].find == _state.Mid(i, rLen))
			{
				newState += rules[j].replace;
				replaced = true;
				i += rLen - 1;
				break;
			}
		}
		if (!replaced)
			newState += _state.Mid(i, 1);
	}

	_state = newState;
	++it;
}

void ULSystem::reset()
{
	it = 0;
	_state = FString(axiom);
}