// FPoisonFloor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPoisonFloor.generated.h"

UCLASS()
class FIRSTGAME_API AFPoisonFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPoisonFloor();

	void CheckOverlap();

protected:
	virtual void BeginPlay() override;
};
