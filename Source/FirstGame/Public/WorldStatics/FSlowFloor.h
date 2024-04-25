// FSlowFloor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSlowFloor.generated.h"

UCLASS()
class FIRSTGAME_API AFSlowFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFSlowFloor();

	void CheckOverlap();

protected:
	virtual void BeginPlay() override;
};
