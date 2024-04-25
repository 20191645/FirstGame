// FBuffComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FBuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentStackChangeDelegate, int32, InCurrentStack);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTGAME_API UFBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFBuffComponent();

	// CurrentStack 속성의 Getter, Setter
	int32 GetCurrentStack() const { return CurrentStack; }
	void SetCurrentStack(int32 InCurrentStack);

	// BuffName 속성의 Getter, Setter
	FString GetBuffName() const { return BuffName; }
	void SetBuffName(FString InBuffName) { BuffName = InBuffName; }

	// MaxStack, Duration, Cooldown 속성의 Getter
	int32 GetMaxStack() const { return MaxStack; }
	float GetDuration() const { return Duration; }

public:
	// 'CurrentStack' 값이 변화할 때 반응할 델리게이트
	FOnCurrentStackChangeDelegate OnCurrentStackChangeDelegate;

private:
	// 버프 이름
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFBuffComponent", meta = (AllowPrivateAccess))
	FString BuffName;
	// 지속 시간
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFBuffComponent", meta = (AllowPrivateAccess))
	float Duration;
	// 현재 스택
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFBuffComponent", meta = (AllowPrivateAccess))
	int32 CurrentStack;
	// 최대 스택
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UFBuffComponent", meta = (AllowPrivateAccess))
	int32 MaxStack;
};
