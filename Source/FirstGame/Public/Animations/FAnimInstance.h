// FAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FAnimInstance.generated.h"

UCLASS()
class FIRSTGAME_API UFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	// FRPGCharacter 클래스에서 PlayAttackAnimMontage() 함수를 호출하기 위하여 friend 정의
	friend class AFRPGCharacter;
	
public:
	UFAnimInstance();

	// NativeInitializeAnimation(): 초기화 함수
	virtual void NativeInitializeAnimation() override;

	// NativeUpdateAnimation(): AnimInstance의 Tick() 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// Animation Montage('AM_Attack')를 재생시켜줄 함수
	void PlayAttackAnimMontage();

protected:
	// CurrentSpeed: 프레임마다 폰의 속력을 연동하기 위한 속성
	// 유지보수 편의를 위해 폰의 Tick() 함수가 아닌 애님 인스턴스의 Tick() 함수에서 폰의 정보를 가져와서,
	// CurrentSpeed 속성 업데이트하는 방식 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UFAnimInstance")
	float CurrentSpeed;

	// Velocity: 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UFAnimInstance")
	FVector Velocity;

	// bIsFalling: 현재 공중에 떠있는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UFAnimInstance")
	uint8 bIsFalling : 1;

	// bIsCrouching: 현재 쭈그려 앉아있는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UFAnimInstance")
	uint8 bIsCrouching : 1;

	// Animation Montage('AM_Attack')를 가져올 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UFAnimInstance")
	TObjectPtr<class UAnimMontage> AttackAnimMontage;
};
