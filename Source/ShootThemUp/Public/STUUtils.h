// CHEN

#pragma once

#include "CoreMinimal.h"


class  STUUtils
{
public:
	template<typename T>
	static T* STUGetPlayerComponent(AActor* Actor) 
	{
		if (!Actor)return nullptr;

		const auto Component = Actor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
