// FNPCharacter.cpp

#include "Characters/FNPCharacter.h"
#include "Controllers/FAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animations/FAnimInstance.h"
#include "Characters/FRPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Components/FStatComponent.h"
#include "Components/FWidgetComponent.h"
#include "UI/FirstUserWidget.h"
#include "UI/SW_HPBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AFNPCharacter::AFNPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 레벨에 새롭게 배치되거나 생성되면 FAIController 빙의가 자동으로 이뤄진다
    AIControllerClass = AFAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    HPBar = CreateDefaultSubobject<UFWidgetComponent>(TEXT("HPBar"));
    HPBar->SetupAttachment(GetRootComponent());
    // 위젯의 위치를 캐릭터 머리 위로 설정
    HPBar->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
    HPBar->SetDrawSize(FVector2D(180.0f, 50.0f));
    HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 레벨에 놓인 액터를 무시하고 표시되지 않도록 Screen -> World 수정
    HPBar->SetWidgetSpace(EWidgetSpace::World);

    // Collision Preset('FCharacter') 설정
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("FCharacter"));

    // ParticleSystemComponent 오브젝트 할당
    SkillParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillParticleSystemComponent"));
    SkillParticleSystemComponent->SetupAttachment(GetRootComponent());
    // 처음부터 Particle을 터트리지 않는다 [false]
    SkillParticleSystemComponent->SetAutoActivate(false);

    // 'DamageDisplay' 오브젝트 할당
    DamageDisplay = CreateDefaultSubobject<UFWidgetComponent>(TEXT("DamageDisplay"));
    DamageDisplay->SetupAttachment(GetMesh());
    DamageDisplay->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageDisplay->SetWidgetSpace(EWidgetSpace::World);
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
	if (true == ::IsValid(AnimInstance))
	{
		// AnimationMontageEnded 델리게이트에 OnAttackAnimMontageEnded() 멤버 함수 바인드
        // -- Animation Montage('AM_Attack_NPC')가 끝나면 함수 호출
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
		// Animation Notify(CheckHit)의 델리게이트에 CheckHit() 멤버 함수 바인드
		AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        // Animation Notify(CheckCanNextAttack)의 델리게이트에 CheckCanNextAttack() 멤버 함수 바인드
        AnimInstance->OnCheckCanNextAttackDelegate.AddDynamic(this, &ThisClass::CheckCanNextAttack);
        // AnimationMontageEnded 델리게이트에 OnSkillAnimMontageEnded() 멤버 함수 바인드
        // -- Animation Montage('AM_Skill_NPC')가 끝나면 함수 호출
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnSkillAnimMontageEnded);
        // Animation Notify(CheckHit_Skill)의 델리게이트에 CheckHit_Skill() 멤버 함수 바인드
        AnimInstance->OnCheckHitSkillDelegate.AddDynamic(this, &ThisClass::CheckHit_Skill);
	}
}

float AFNPCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // FinalDamageAmount: 받은 데미지 최종값
    float FinalDamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    if (StatComponent->GetCurrentHP() < KINDA_SMALL_NUMBER)
    {
        AFAIController* AIController = Cast<AFAIController>(GetController());
        if (true == ::IsValid(AIController))
        {
            AIController->EndAI();
        }
    }

    return FinalDamageAmount;
}

void AFNPCharacter::SetWidget(UFirstUserWidget* InFirstUserWidget)
{
    // HPBar 세팅
    USW_HPBar* HPBarWidget = Cast<USW_HPBar>(InFirstUserWidget);
    if (true == ::IsValid(HPBarWidget)) {
        HPBarWidget->SetMaxHP(StatComponent->GetMaxHP());
        HPBarWidget->InitializeHPBarWidget(StatComponent);
        // 'OnCurrentHPChange' 델리게이트에 SW_HPBar 클래스의 멤버함수 OnCurrentHPChange()를 바인드
        StatComponent->OnCurrentHPChangeDelegate.AddDynamic(HPBarWidget, &USW_HPBar::OnCurrentHPChange);
    }
}

void AFNPCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 위젯이 플레이어가 보는 방향으로 보여지도록 수정
    APlayerCameraManager* CM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (CM) {
        // HP 위젯
        FVector Start = HPBar->GetComponentLocation();
        FVector Target = CM->GetTransformComponent()->GetComponentLocation();
        FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
        HPBar->SetWorldRotation(Rotation);

        // 데미지 위젯
        Start = DamageDisplay->GetComponentLocation();
        Rotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
        DamageDisplay->SetWorldRotation(Rotation);
    }
}

void AFNPCharacter::Attack()
{
    // 공격할 때 Montage Section 위치가 0 -> BeginAttack() 호출
    if (0 == CurrentSectionCount) {
        BeginAttack();
        return;
    }
    // 공격할 때 Montage Section 위치가 0이 아님 -> 다른 섹션으로 넘어간다
    else {
        // CurrentSectionCount의 값이 "1 ~ 5" 사이에 있는지 체크한다
        ensure(FMath::IsWithinInclusive<int32>(CurrentSectionCount, 1, 5));
    }
}

void AFNPCharacter::OnAttackAnimMontageEnded(UAnimMontage* Montage, bool bIsInterrupt)
{
	// 'AM_Attack_NPC' 재생이 끝난 게 맞는지 확인
    if (bIsAttacking) {
        bIsAttacking = false;
    }
}

void AFNPCharacter::CheckHit()
{
    // 하나 이상의 오브젝트를 가져올거기 때문에 TArray 사용 -- {Multi}
    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params(NAME_None, false, this);

    // SweepSingleByChannel(): Trace channel을 사용해 물리적 충돌 여부를 조사하는 함수
    bResult = GetWorld()->SweepMultiByChannel(
        HitResults,  // 물리적 충돌이 탐지되면 정보를 담을 구조체
        GetActorLocation(), // 탐색을 시작할 위치
        GetActorLocation() + AttackRange * GetActorForwardVector(),   // 탐색을 끝낼 위치
        FQuat::Identity,    // 탐색에 사용할 도형의 회전 = ZeroRotator
        ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 Trace channel 정보 = Attack Channel의 Enum값
        FCollisionShape::MakeSphere(AttackRadius),  // 탐색에 사용할 기본 도형 정보 = 반지름 50인 구체
        Params // 탐색 방법에 대한 설정 값을 모은 구조체
    );

    // Hit한 대상 감지 성공
    if (true == bResult) {
        for (auto const& HitResult : HitResults) {
            AFCharacter* HitCharacter = Cast<AFCharacter>(HitResult.GetActor());
            // HitCharacter가 FRPGCharacter인 경우
            if (HitCharacter->IsA(AFRPGCharacter::StaticClass())) {
                // 현재 액터가 Hit한 대상의 데미지 전달 함수 호출
                FDamageEvent DamageEvent;
                HitCharacter->TakeDamage(5.f, DamageEvent, GetController(), this);
                break;
            }
        }
    }
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

    // 캐릭터 타격 X -- 콤보 멈춤
    if (false == bResult) {
        CurrentSectionCount += 1;
    }
    // 캐릭터 타격 O -- 콤보 시작
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
    // 공격 액션이 끝났으므로 다시 움직이는 모드로 변경
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AFNPCharacter::Skill()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // 움직이지 않고 멈춰서 Animation Montage를 재생할 수 있도록 한다
    bIsUsingSkill = true;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // FAnimInstance에서 정의한 Animation Montage를 재생시켜줄 함수
    AnimInstance->PlaySkillAnimMontage();

    // 스킬 이펙트 생성
    SkillParticleSystemComponent->ActivateSystem(true);
}

void AFNPCharacter::OnSkillAnimMontageEnded(UAnimMontage* Montage, bool bIsInterrupt)
{
    if (bIsUsingSkill) {
        // 공격 액션이 끝났으므로 다시 움직이는 모드로 변경
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsUsingSkill = false;
        SkillParticleSystemComponent->DeactivateSystem();
    }
}

void AFNPCharacter::CheckHit_Skill()
{
    // 하나 이상의 오브젝트를 가져올거기 때문에 TArray 사용 -- {Multi}
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParams(NAME_None, false, this);

    // 주위 오브젝트들 감지 -- {Overlap}{Multi}{ByChannel}
    // -- 충돌 범위: 캐릭터 전방 구체
    bool bSkillResult = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_GameTraceChannel2,	// = Attack Channel
        FCollisionShape::MakeSphere(SkillRadius),
        CollisionQueryParams
    );

    // Hit한 대상 감지 성공
    if (true == bSkillResult) {
        for (auto const& OverlapResult : OverlapResults) {
            AFCharacter* HitCharacter = Cast<AFCharacter>(OverlapResult.GetActor());
            // HitCharacter가 FRPGCharacter인 경우
            if (HitCharacter->IsA(AFRPGCharacter::StaticClass())) {
                // 현재 액터가 Hit한 대상의 데미지 전달 함수 호출
                FDamageEvent DamageEvent;
                HitCharacter->TakeDamage(10.f, DamageEvent, GetController(), this);
                break;
            }
        }
    }

    /*
#pragma region DebugDrawing
    // 캡슐 색깔 -- bResult(Hit 성공)
    FColor DrawColor = true == bSkillResult ? FColor::Green : FColor::Red;
    // 캡슐이 살아있는 시간
    float DebugLifeTime = 2.f;

    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        SkillRadius,
        26,
        DrawColor,
        false,
        DebugLifeTime
    );
#pragma endregion 
    */
}