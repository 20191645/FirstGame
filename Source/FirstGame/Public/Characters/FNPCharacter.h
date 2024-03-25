// FNPCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "FNPCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFNPCharacter : public AFCharacter
{
	GENERATED_BODY()
	
public:
	AFNPCharacter();

	virtual void BeginPlay() override;
};
