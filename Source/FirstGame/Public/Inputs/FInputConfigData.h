// FInputConfigData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FInputConfigData.generated.h"

UCLASS()
class FIRSTGAME_API UFInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 'IA_Move'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr <class UInputAction> MoveAction;
	
	// 'IA_Look'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> LookAction;

	// 'IA_Jump'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> JumpAction;

	// 'IA_Attack'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> AttackAction;
};
