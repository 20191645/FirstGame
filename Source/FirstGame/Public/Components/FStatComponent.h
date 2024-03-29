// FStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentHPChangeDelegate, float, InOldCurrentHP, float, InNewCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPChangeDelegate, float, InOldMaxHP, float, InNewMaxHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTGAME_API UFStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFStatComponent();

	virtual void BeginPlay() override;

	// MaxHP 속성의 Getter, Setter
	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

	// CurrentHP 속성의 Getter, Setter
	float GetCurrentHP()const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);

public:	
	// 'CurrentHP' 값이 0에 가까워졌을 때 바운드될 델리게이트
	FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;
	// 'CurrentHP' 값이 변화할 때 바운드될 델리게이트
	FOnCurrentHPChangeDelegate OnCurrentHPChangeDelegate;
	// 'MaxHP' 값이 변화할 때 바운드될 델리게이트
	FOnMaxHPChangeDelegate OnMaxHPChangeDelegate;

private:
	// Game Instance('FGameInstance') 데이터를 담을 속성
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFStatComponent", meta = (AllowPrivateAccess))
	TObjectPtr<class UFGameInstance> GameInstance;

	// 최대 HP
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFStatComponent", meta = (AllowPrivateAccess))
	float MaxHP;
	// 현재 HP
	// Transient: 계속 변하기 때문에 Serialize가 필요 없는 속성임을 알려준다
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "UFStatComponent", meta = (AllowPrivateAccess))
	float CurrentHP;
};
