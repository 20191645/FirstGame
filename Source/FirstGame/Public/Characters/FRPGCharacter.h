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

	// AnimNotify(AN_CheckHit_Skill) 클래스에서 FRPGCharacter 클래스의 CheckHit_Skill() 멤버함수에 접근하기 위함
	friend class UAN_CheckHit_Skill;
	
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

	UFUNCTION()
	class UFBuffComponent* GetBuffComponent() { return BuffComponent; }

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

	// 'IA_Menu' 액션에 바인드할 함수
	void Menu(const FInputActionValue& InValue);

	// 'IA_Run' 액션 실행중에 바인드할 함수
	void Running(const FInputActionValue& InValue);
	// 'IA_Run' 액션 중단시에 바인드할 함수
	void StopRunning(const FInputActionValue& InValue);

	// 'OnCurrentStackChangeDelegate' 델리게이트에 바운드될 함수
	UFUNCTION()
	void OnCurrentStackChange(int32 InCurrentStack);

	// 버프 지속 시간 종료 시 호출될 함수
	void BuffDurationEnd();
	// Poison 디버프일 때 호출될 함수
	void PoisonBuff();

	// 'OnOutOfCurrentHPDelegate' 델리게이트에 바운드될 함수
	UFUNCTION()
	void OnCharacterDeath();
	// 플레이어 캐릭터 리스폰 함수
	void Respawn();

	// 'IA_Skill' 액션에 바인드할 함수
	void Skill();

	// CheckHit 델리게이트와 바운드할 함수
	UFUNCTION()
	void CheckHit_Skill();

	// Animation Montage[Skill]가 끝났을 때 호출되어 바인드할 함수
	UFUNCTION()
	void OnSkillAnimMontageEnded(class UAnimMontage* Montage, bool bIsInterrupt);

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

	// bIsAttacking: Animation Montage('IM_Attack_RPG')가 재생중인지 확인하는 데이터
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

	// 일반 공격 충돌 탐색 길이
	float AttackRange = 100.f;
	// 일반 공격 충돌 탐색 구체 반지름
	float AttackRadius = 50.f;

	// 클래스에 부착할 BuffComponent 데이터를 담을 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UFBuffComponent> BuffComponent;

	// 버프 효과를 수행하기 위해 사용할 타이머
	FTimerHandle myTimerHandle;
	// 버프 지속 시간동안 남은 호출 횟수
	int32 BuffCallsRemaining;

	// 리스폰 시 실행할 Particle 효과 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UParticleSystemComponent> RespawnParticleSystemComponent;

	// 스킬 사용 시 터트릴 Particle 효과 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UParticleSystem> SkillParticleSystem;

	// 스킬 공격 범위 길이
	float SkillRange = 350.f;
	// 스킬 공격 범위 구체 반지름
	float SkillRadius = 200.f;

	// bIsUsingSkill: Animation Montage('IM_Skill_RPG')가 재생중인지 확인하는 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AFRPGCharacter", Meta = (AllowPrivateAccess))
	bool bIsUsingSkill = false;
};
