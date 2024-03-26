// FNPCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "FNPCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFNPCharacter : public AFCharacter
{
	GENERATED_BODY()

	// AnimNotify(AN_CheckHit) 클래스에서 FNPCharacter 클래스의 CheckHit() 멤버함수에 접근하기 위함
	friend class UAN_CheckHit;

	// BTTask_Attack 클래스에서 Attack() 멤버함수를 호출하기 위해
	friend class UBTTask_Attack;
	
public:
	AFNPCharacter();

	virtual void BeginPlay() override;

	bool IsNowAttacking() const { return bIsAttacking; }

	// NPC가 데미지를 받기 위한 함수
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	// NPC 공격 Task 실행 시 호출될 함수
	void Attack();

	// Animation Montage가 끝났을 때 호출되어 바인드할 함수
	UFUNCTION()
	void OnAttackAnimMontageEnded(class UAnimMontage* Montage, bool bIsInterrupt);

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
	// bIsAttacking: Animation Montage('IM_Attack_NPC')가 재생중인지 확인하는 데이터
	bool bIsAttacking = false;

	// 충돌 탐색 길이
	float AttackRange = 100.f;
	// 충돌 탐색 구체 반지름
	float AttackRadius = 50.f;

	// 직접 설정한 Montage Section 이름이 필요함 -> Attack1, Attack2, Attack3...
	// Attack1: 첫 번째 공격 시작
	// Attack2: 첫 번째 공격 끝
	// Attack3: 두 번째 공격 시작
	// Attack4: 두 번째 공격 끝
	// Attack5-6: 세 번째 공격
	FString AttackAnimMontageSectionName = FString(TEXT("Attack"));

	// 현재 Montage Section 위치 [1~5]
	int32 CurrentSectionCount = 0;

	// bResult: NPC 공격에 타격된 대상이 있는지
	bool bResult = false;
};
