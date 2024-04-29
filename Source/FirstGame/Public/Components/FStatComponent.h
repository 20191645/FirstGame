// FStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentHPChangeDelegate, float, InOldCurrentHP, float, InNewCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPChangeDelegate, float, InOldMaxHP, float, InNewMaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentMPChangeDelegate, float, InOldCurrentMP, float, InNewCurrentMP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxMPChangeDelegate, float, InOldMaxMP, float, InNewMaxMP);

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

	// MaxMP 속성의 Getter, Setter
	float GetMaxMP() const { return MaxMP; }
	void SetMaxMP(float InMaxMP);
	// CurrentMP 속성의 Getter, Setter
	float GetCurrentMP()const { return CurrentMP; }
	void SetCurrentMP(float InCurrentMP);

public:	
	// 'CurrentHP' 값이 0에 가까워졌을 때 바운드될 델리게이트
	FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;
	// 'CurrentHP' 값이 변화할 때 바운드될 델리게이트
	FOnCurrentHPChangeDelegate OnCurrentHPChangeDelegate;
	// 'MaxHP' 값이 변화할 때 바운드될 델리게이트
	FOnMaxHPChangeDelegate OnMaxHPChangeDelegate;

	// 'CurrentHP' 값이 변화할 때 바운드될 델리게이트
	FOnCurrentMPChangeDelegate OnCurrentMPChangeDelegate;
	// 'MaxHP' 값이 변화할 때 바운드될 델리게이트
	FOnMaxMPChangeDelegate OnMaxMPChangeDelegate;

private:
	// 'CurrentStage' 값이 변하면[델리게이트에서 Broadcast] 바운드될 함수
	UFUNCTION()
	void OnCurrentStageChanged(int32 NewCurrentStage);

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

	// 최대 MP
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFStatComponent", meta = (AllowPrivateAccess))
	float MaxMP;
	// 현재 MP
	// Transient: 계속 변하기 때문에 Serialize가 필요 없는 속성임을 알려준다
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "UFStatComponent", meta = (AllowPrivateAccess))
	float CurrentMP;
};
