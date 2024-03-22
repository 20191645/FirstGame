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

	// AnimNotify(AN_CheckHit) 클래스에서 FRPGCharacter 클래스의 CheckHit() 멤버함수에 접근하기 위함
	friend class UAN_CheckHit;
	
public:
	AFRPGCharacter();

	virtual void BeginPlay() override;

	// Input Value 속성(Forward, Right) Getter 함수
	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

	// Animation Montage가 끝났을 때 호출되어 바인드할 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// TakeDamage(): 데미지 전달 구현 함수
	// Damage: 데미지 세기, DamageEvent: 데미지 종류, EventInstigator: 공격 명령 내린 컨트롤러, DamageCauser: 데미지 전달을 위해 사용한 액터
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;;

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

	// 함수 순서: Begin(공격 시작) - Check(공격키를 누름) - End(공격 끝)
	void BeginAttack();

	// CheckCanNextAttack 델리게이트와 바운드할 함수
	UFUNCTION()
	void CheckCanNextAttack();

	UFUNCTION()
	void EndAttack(class UAnimMontage* InAnimMontage, bool bInterrupted);

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

	// 직접 설정한 Montage Section 이름이 필요함 -> Attack1, Attack2, Attack3...
	// Attack1: 첫 번째 공격 시작 -> Attack2 / Attack3-4 [공격 키 입력X, 공격 키 입력O]
	// Attack2: 첫 번째 공격 끝
	// Attack3-4: 두 번째 공격 시작 ~ 끝
	FString AttackAnimMontageSectionName = FString(TEXT("Attack"));

	// 현재 Montage Section 위치 [1~3]
	int32 CurrentSectionCount = 0;

	// bIsAttackKeyPressed: 공격키를 눌렀는지
	// 에디터에서 관리되거나 Serialize될 필요 없으므로 uint8 대신 bool 자료형 사용
	bool bIsAttackKeyPressed = false;

	// 충돌 탐색 길이
	float AttackRange = 100.f;
	// 충돌 탐색 구체 반지름
	float AttackRadius = 50.f;
};
