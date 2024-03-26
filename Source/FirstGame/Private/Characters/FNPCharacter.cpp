// FNPCharacter.cpp

#include "Characters/FNPCharacter.h"
#include "Controllers/FAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animations/FAnimInstance.h"

AFNPCharacter::AFNPCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 레벨에 새롭게 배치되거나 생성되면 FAIController 빙의가 자동으로 이뤄진다
    AIControllerClass = AFAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AFNPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// IsPlayerControlled(): 플레이어가 컨트롤하고 있는 폰인지 확인
	if (false == IsPlayerControlled()) {
		// bUseControllerRotation***: 컨트롤러 움직임에 따라 캐릭터가 회전하지 않는다 [false]
		bUseControllerRotationYaw = false;

		// 회전 속도를 지정하여 이동 방향으로 캐릭터가 부드럽게 회전하는 기능 추가
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		// NPC 이동 속도 너프
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
	if (true == ::IsValid(AnimInstance) && false == AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::OnAttackAnimMontageEnded))
	{
		// AnimationMontageEnded 델리게이트에 OnAttackAnimMontageEnded() 멤버 함수 바인드
        // -- Animation Montage('AM_Attack_NPC')가 끝나면 함수 호출
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
		// Animation Notify(CheckHit)의 델리게이트에 CheckHit() 멤버 함수 바인드
		AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        // Animation Notify(CheckCanNextAttack)의 델리게이트에 CheckCanNextAttack() 멤버 함수 바인드
        AnimInstance->OnCheckCanNextAttackDelegate.AddDynamic(this, &ThisClass::CheckCanNextAttack);
	}
}

void AFNPCharacter::Attack()
{
    // 공격키를 눌렀을 때 Montage Section 위치가 0 -> BeginAttack() 호출
    if (0 == CurrentSectionCount) {
        BeginAttack();
        return;
    }
    // 공격키를 눌렀을 때 Montage Section 위치가 0이 아님 -> 다른 섹션으로 넘어간다
    else {
        // CurrentSectionCount의 값이 "1 ~ 5" 사이에 있는지 체크한다
        ensure(FMath::IsWithinInclusive<int32>(CurrentSectionCount, 1, 5));
        // 다음 Montage Section으로 넘어갈 때 확인한다
        bIsAttackKeyPressed = true;
    }
}

void AFNPCharacter::OnAttackAnimMontageEnded(UAnimMontage* Montage, bool bIsInterrupt)
{
	bIsAttacking = false;
}

void AFNPCharacter::CheckHit()
{
    // 대상을 하나만 감지할 예정(Single)이라 단수 정의
    FHitResult HitResult;
    // Params(..., bInTraceComplex, *InIgnoreActor) 
    // -- bInTraceComplex: Static Mesh 모양 그대로 충돌체를 만들지 선택 (T/F)
    // -- InIgnoreActor: 대상 감지에서 해당 액터는 뺀다 (Ignore)
    FCollisionQueryParams Params(NAME_None, false, this);

    // SweepSingleByChannel(): Trace channel을 사용해 물리적 충돌 여부를 조사하는 함수
    bool bResult = GetWorld()->SweepSingleByChannel(
        HitResult,  // 물리적 충돌이 탐지되면 정보를 담을 구조체
        GetActorLocation(), // 탐색을 시작할 위치
        GetActorLocation() + AttackRange * GetActorForwardVector(),   // 탐색을 끝낼 위치
        FQuat::Identity,    // 탐색에 사용할 도형의 회전 = ZeroRotator
        ECC_GameTraceChannel12, // 물리 충돌 감지에 사용할 Trace channel 정보 = Attack Channel의 Enum값
        FCollisionShape::MakeSphere(AttackRadius),  // 탐색에 사용할 기본 도형 정보 = 반지름 50인 구체
        Params // 탐색 방법에 대한 설정 값을 모은 구조체
    );

    // Hit한 대상 감지 성공
    if (true == bResult) {
        // HitResult의 Actor 속성은 TWeakObjectPtr 자료형 선언 
        // -> IsValid() 함수로 유효성 검사 후 사용
        // <- GetActor() 내부에서 IsActorValid() 함수 호출됨
        if (true == ::IsValid(HitResult.GetActor())) {
            UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[NPC] Hit Actor Name: %s"), *HitResult.GetActor()->GetName()));
        }
    }

#pragma region DebugDrawing
    // 캡슐 탐색 시작 위치에서 끝 위치로 향하는 벡터 (캐릭터의 Forward 방향)
    FVector TraceVec = GetActorForwardVector() * AttackRange;
    // 캡슐 벡터의 중심 위치
    FVector Center = GetActorLocation() + TraceVec * 0.5f;
    // 캡슐 벡터 길이의 절반
    float HalfHeight = AttackRange * 0.5f + AttackRadius;
    // 캐릭터의 Forward 방향으로 회전 행렬 적용하여 캡슐을 눕힌다
    FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
    // 캡슐 색깔 -- bResult(Hit 성공)
    FColor DrawColor = true == bResult ? FColor::Green : FColor::Red;
    // 캡슐이 살아있는 시간
    float DebugLifeTime = 5.f;

    DrawDebugCapsule(
        GetWorld(),
        Center,
        HalfHeight,
        AttackRadius,
        CapsuleRot,
        DrawColor,
        false,
        DebugLifeTime
    );
#pragma endregion 
}

void AFNPCharacter::BeginAttack()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    CurrentSectionCount = 1;

    // 움직이지 않고 멈춰서 Animation Montage를 재생할 수 있도록 한다
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // FAnimInstance에서 정의한 Animation Montage를 재생시켜줄 함수
    AnimInstance->PlayAttackAnimMontage();

    // 공격이 끝났는지(bIsAttacking) 확인할 델리게이트 추가
    FOnMontageEnded OnMontageEndedDelegate;
    // 위 델리게이트에 EndAttack() 함수와 바인드 -> Montage가 끝나면 EndAttack() 호출
    OnMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
    // 특정한 Montage(AttackAnimMontage)가 끝날때만 델리게이트가 호출되게끔 설정
    AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, AnimInstance->AttackAnimMontage);
}

void AFNPCharacter::CheckCanNextAttack()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // 공격 키 입력 X -- 콤보 멈춤
    if (false == bIsAttackKeyPressed) {
        CurrentSectionCount += 1;
    }
    // 공격 키 입력 O -- 콤보 시작
    else {
        CurrentSectionCount += 2;
    }

    // NextSectionName: 넘어가고자 하는 Montage Section의 이름
    FName NextSectionName =
        *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentSectionCount);
    // Motage_JumpToSection(Section, Montage): Montage의 Section으로 넘어간다
    AnimInstance->Montage_JumpToSection(NextSectionName, AnimInstance->AttackAnimMontage);
}

void AFNPCharacter::EndAttack(UAnimMontage* InAnimMontage, bool bInterrupted)
{
    // CurrentSectionCount 값이 0이 아닌지 체크한다
    ensure(0 != CurrentSectionCount);
    CurrentSectionCount = 0;
    bIsAttackKeyPressed = false;
    // 공격 액션이 끝났으므로 다시 움직이는 모드로 변경
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
