// FCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFCharacter();

	// HP, 죽음 속성의 Getter, Setter
	float GetMaxHP() const { return MaxHP; }
	float GetCurrentHP() const { return CurrentHP; }
	void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }
	void SetCurrentHP(float InCurrentHP) { CurrentHP = InCurrentHP; }
	bool IsDead() const { return bIsDead; }

protected:
	// SpringArm, Camera Component는 Character Class에 포함되어 있지 않기 때문에 직접 추가
	// SpringArmComponent: 3인칭 시점 카메라 구도 설정 돕는 컴포넌트 - 카메라 봉 길이, 컴포넌트 회전 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	// CameraComponenet: 가상 세계 모습을 플레이어 화면에 전송하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;

	// 최대 HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter", meta = (AllowPrivateAccess))
	float MaxHP = 100.f;
	// 현재 HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter", meta = (AllowPrivateAccess))
	float CurrentHP = 100.f;
	// 죽음 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter", meta = (AllowPrivateAccess))
	uint8 bIsDead : 1;
};
