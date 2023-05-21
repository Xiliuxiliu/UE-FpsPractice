// CHEN

#pragma once

#include "CoreMinimal.h"


class  STUUtils
{
public:
	template<typename T>
	static T* STUGetPlayerComponent(APawn* PlayerPawn) 
	{
		if (!PlayerPawn)return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
