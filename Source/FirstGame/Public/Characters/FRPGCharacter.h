// FRPGCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "InputActionValue.h"
#include "FRPGCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFRPGCharacter : public AFCharacter
{
	GENERATED_BODY()
	
public:
	AFRPGCharacter();

	virtual void BeginPlay() override;

	// Input Value 속성(Forward, Right) Getter 함수
	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

	// Animation Montage가 끝났을 때 호출되어 바인드할 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// 'IA_Move' 액션에 바인드할 함수
	void Move(const FInputActionValue& InValue);
	// 'IA_Look' 액션에 바인드할 함수
	void Look(const FInputActionValue& InValue);
	// 'IA_Attack' 액션에 바인드할 함수
	void Attack(const FInputActionValue& InValue);

	// CheckHit 델리게이트와 바운드할 함수
	// -- 언리얼 실행 환경이 찾을 수 있도록 UFUNCTION() 매크로 붙임
	UFUNCTION()
	void CheckHit();

private:
	// Input Config Data의 액션들과 캐릭터를 바인드 시켜줄 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UFInputConfigData> PlayerCharacterInputConfigData;

	// Input Mapping Context와 캐릭터를 바인드 시켜줄 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> PlayerCharacterInputMappingContext;

	// 애니메이션(AnimInstance)를 위해서 Input Value 속성(Forward, Right) 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess = true))
	float ForwardInputValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess = true))
	float RightInputValue;

	// bIsAttacking: Animation Montage('IM_Attack')가 재생중인지 확인하는 데이터
	// UPROPERTY()가 아니므로 생성자에서 초기화 필수
	uint8 bIsAttacking : 1;
};
