// FRPGCharacter.cpp

#include "Characters/FRPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inputs/FInputConfigData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animations/FAnimInstance.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Controllers/FPlayerController.h"
#include "Components/FBuffComponent.h"
#include "Components/FStatComponent.h"
#include "Game/FGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FPlayerState.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AFRPGCharacter::AFRPGCharacter()
    :bIsAttacking(false)
{
    PrimaryActorTick.bCanEverTick = false;

    // CameraComponent, SpringArmComponent: Character의 속성을 상속받음
    // RPGCharacter의 시점: Back View Mode

    CameraComponent->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

    // bUseControllerRotation***: 컨트롤러 움직임(마우스)에 따라 캐릭터가 회전하지 않는다 [false]
    // -> 마우스 상하좌우 회전에 캐릭터 방향은 바뀌지 않는다
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // bUsePawnControlRotation: 컨트롤러 움직임에 따라 Spring Arm(카메라 지지대)가 같이 회전한다 [true]
    // -> ControlRotation 값과 Spring Arm 회전값이 동기화 => 카메라 방향이 ControlRotation 값
    SpringArmComponent->bUsePawnControlRotation = true;
    // bDoCollisionTest: 카메라가 벽에 부딪힐 때 뚫고 지나가지 않는다 [true]
    SpringArmComponent->bDoCollisionTest = true;
    // bInherit***: Root Component(Pawn)가 가져오는 Control Rotation 값을 반영한다 [true]
    SpringArmComponent->bInheritPitch = true;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bInheritRoll = false;

    // 여기까지 결과 => 캐릭터 자체는 회전하지 않지만 카메라 이동 방향에 맞춰 이동한다
    // 따라서 아래에 회전 속도를 지정하여 이동 방향으로 캐릭터가 부드럽게 회전하는 기능 추가

    // bOrientRotationToMovement: 캐릭터 이동 방향에 회전을 일치시킨다 [true]
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // bUseControllerDesiredRotation: ControlRotation을 목표 회전값으로 삼아서 회전하지 않는다 [false]
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    // RotationRate: 회전 속도
    GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

    // Collision Preset('FCharacter') 설정
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("FCharacter"));

    // BuffComponent 오브젝트 할당
    BuffComponent = CreateDefaultSubobject<UFBuffComponent>(TEXT("BuffComponent"));

    // ParticleSystemComponent 오브젝트 할당
    RespawnParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RespawnParticleSystemComponent"));
    RespawnParticleSystemComponent->SetupAttachment(GetRootComponent());
    // 처음부터 Particle을 터트리지 않는다 [false]
    RespawnParticleSystemComponent->SetAutoActivate(false);

    // ParticleSystem 오브젝트 할당
    SkillParticleSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("SkillParticleSystem"));
}

void AFRPGCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (true == ::IsValid(PlayerController))
    {
        // UEnhancedInputLocalPlayerSubsystem: Enhanced Input의 모든 내용을 관리하는 싱글톤 패턴
        // GetSubSystem('Player'): 플레이어 컨트롤러의 '플레이어(나)'에게 해당하는 서브 시스템을 반환
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (true == ::IsValid(Subsystem))
        {
            // 서브 시스템에 Mapping Context 추가
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }

    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (true == ::IsValid(AnimInstance)) {
        // AnimationMontageEnded 델리게이트에 OnAttackMontageEnded() 멤버 함수 바인드
        // -- Animation Montage('AM_Attack_RPG')가 끝나면 함수 호출
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackMontageEnded);
        // Animation Notify(CheckHit)의 델리게이트에 CheckHit() 멤버 함수 바인드
        AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        // Animation Notify(CheckCanNextAttack)의 델리게이트에 CheckCanNextAttack() 멤버 함수 바인드
        AnimInstance->OnCheckCanNextAttackDelegate.AddDynamic(this, &ThisClass::CheckCanNextAttack);
        // Animation Notify(CheckHit_Skill)의 델리게이트에 CheckHit_Skill() 멤버 함수 바인드
        AnimInstance->OnCheckHitSkillDelegate.AddDynamic(this, &ThisClass::CheckHit_Skill);
        // AnimationMontageEnded 델리게이트에 OnSkillAnimMontageEnded() 멤버 함수 바인드
        // -- Animation Montage('AM_Skill_RPG')가 끝나면 함수 호출
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnSkillAnimMontageEnded);
    }

    // 'OnOutOfCurrentHPDelegate'에 'OnCharacterDeath()' 멤버함수 바인드
    if (true == ::IsValid(BuffComponent)) {
        BuffComponent->OnCurrentStackChangeDelegate.AddDynamic(this, &ThisClass::OnCurrentStackChange);
    }

    // 'OnOutOfCurrentHPDelegate'에 'OnCharacterDeath()' 멤버함수 바인드
    UFStatComponent* MyStatComponent = Cast<UFStatComponent>(Super::GetStatComponent());
    if (true == ::IsValid(MyStatComponent)) {
        MyStatComponent->OnOutOfCurrentHPDelegate.AddDynamic(this, &ThisClass::OnCharacterDeath);
    }
}

void AFRPGCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 'AM_Attack_RPG' 재생이 끝난 게 맞는지 확인
    if (bIsAttacking) {
        // Animation Montage가 끝났으므로 다시 걷는 모드로 변경
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        bIsAttacking = false;
    }
}

float AFRPGCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    return FinalDamageAmount;
}

void AFRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced Input을 이용하여 Input Action 바인드 - Input Config Data 이용
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (true == ::IsValid(EnhancedInputComponent))
    {
        // Tiggered: Started, Ongoing 상태를 합친 상태
        // MoveAction('IA_Move')을 트리거 상태에서 Move 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
        // LookAction('IA_Look')을 트리거 상태에서 Look 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
        // JumpAction('IA_Jump')을 Started 상태에서 Jump 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        // AttackAction('IA_Attack')을 Started 상태에서 Attack 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
        // MenuAction('IA_Menu')을 Started 상태에서 Menu 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MenuAction, ETriggerEvent::Started, this, &ThisClass::Menu);
        // MenuAction('IA_Run')을 Triggered 상태에서 Running 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->RunAction, ETriggerEvent::Triggered, this, &ThisClass::Running);
        // MenuAction('IA_Run')을 Completed 상태에서 StopRunning 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->RunAction, ETriggerEvent::Completed, this, &ThisClass::StopRunning);
        // MenuAction('IA_Skill')을 Started 상태에서 Skill 함수와 바인드 시켜준다
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->SkillAction, ETriggerEvent::Started, this, &ThisClass::Skill);
    }
}

void AFRPGCharacter::Move(const FInputActionValue& InValue)
{
    // Input Action Value를 FVector2D(2차원) 형태로 해석하여 반환
    FVector2D MovementVector = InValue.Get<FVector2D>();
    ForwardInputValue = MovementVector.X;
    RightInputValue = MovementVector.Y;

    // ControlRotation: 플레이어 의지 속성(마우스 움직임)에 따라 폰이 회전해야 할 회전값
    // 플레이어의 회전 의지 중 Yaw 성분으로만 전진 방향 결정 -> 이동 방향은 마우스의 좌우 방향에만 영향 받는다.
    // Pitch 값 안 쓰는 이유 - 마우스가 위를 쳐다본다고 공중으로 이동하면 안되기 때문.
    const FRotator CurrentControlRotation = GetController()->GetControlRotation();
    const FRotator CurrentControlRotationYaw(0.f, CurrentControlRotation.Yaw, 0.f);

    // Rotator을 통해 Matrix 생성 
    // -> Matrix의 한 행이 각각 기즈모의 xyz축에 대응 
    // -> GetUnitAxis() 함수 사용하여 각 축을 Vector로 가져옴
    // Rotator: [pitch] [yaw]	[roll]
    // Matrix:	[pitch] [0]		[0]		- x축
    //			[0]		[yaw]	[0]		- y축
    //			[0]		[0]		[roll]	- z축
    FVector ForwardDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::X);
    FVector RightDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::Y);

    // 캐릭터 이동
    AddMovementInput(ForwardDirection, MovementVector.X);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void AFRPGCharacter::Look(const FInputActionValue& InValue)
{
    FVector2D LookAxisVector = InValue.Get<FVector2D>();

    // 'IMC_PlayerCharacter'에서 X값에는 마우스 좌우 값을, Y값에는 마우스 상하 값의 반대값을 설정
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void AFRPGCharacter::Attack(const FInputActionValue& InValue)
{
    // 공격키를 눌렀을 때 Montage Section 위치가 0 -> BeginAttack() 호출
    if (0 == CurrentSectionCount) {
        BeginAttack();
        return;
    }
    // 공격키를 눌렀을 때 Montage Section 위치가 0이 아님 -> 다른 섹션으로 넘어간다
    else {
        // CurrentSectionCount의 값이 "1 ~ 3" 사이에 있는지 체크한다
        ensure(FMath::IsWithinInclusive<int32>(CurrentSectionCount, 1, 3));
        // 다음 Montage Section으로 넘어갈 때 확인한다
        bIsAttackKeyPressed = true;
    }
}

void AFRPGCharacter::CheckHit()
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
        ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 Trace channel 정보 = Attack Channel의 Enum값
        FCollisionShape::MakeSphere(AttackRadius),  // 탐색에 사용할 기본 도형 정보 = 반지름 50인 구체
        Params // 탐색 방법에 대한 설정 값을 모은 구조체
    );

    // Hit한 대상 감지 성공
    if (true == bResult) {
        // HitResult의 Actor 속성은 TWeakObjectPtr 자료형 선언 
        // -> IsValid() 함수로 유효성 검사 후 사용
        // <- GetActor() 내부에서 IsActorValid() 함수 호출됨
        if (true == ::IsValid(HitResult.GetActor())) {
            // 현재 액터가 Hit한 대상의 데미지 전달 함수 호출
            FDamageEvent DamageEvent;
            HitResult.GetActor()->TakeDamage(10.f, DamageEvent, GetController(), this);
        }
    }

    /*
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
    */
}

void AFRPGCharacter::BeginAttack()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // 점프 액션, 스킬 액션 중 공격 불가
    if (AnimInstance->bIsFalling || bIsUsingSkill) {
        return;
    }

    CurrentSectionCount = 1;
    bIsAttacking = true;

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

void AFRPGCharacter::CheckCanNextAttack()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // Attack2 (Attack1에서 공격 키 입력 X -- 콤보 멈춤)
    if (false == bIsAttackKeyPressed) {
        CurrentSectionCount = 2;
    }
    // Attack3 (Attack1에서 공격 키 입력 O -- 콤보 시작)
    else {
        CurrentSectionCount = 3;
    }

    // NextSectionName: 넘어가고자 하는 Montage Section의 이름
    FName NextSectionName =
        *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentSectionCount);
    // Motage_JumpToSection(Section, Montage): Montage의 Section으로 넘어간다
    AnimInstance->Montage_JumpToSection(NextSectionName, AnimInstance->AttackAnimMontage);
}

void AFRPGCharacter::EndAttack(UAnimMontage* InAnimMontage, bool bInterrupted)
{
    // CurrentSectionCount 값이 0이 아닌지 체크한다
    ensure(0 != CurrentSectionCount);
    CurrentSectionCount = 0;
    bIsAttackKeyPressed = false;
    bIsAttacking = false;
    // 공격 액션이 끝났으므로 다시 움직이는 모드로 변경
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AFRPGCharacter::Menu(const FInputActionValue& InValue)
{
    // PlayerController 클래스에서 메뉴 토글
    AFPlayerController* PlayerController = GetController<AFPlayerController>();
    if (true == ::IsValid(PlayerController)) {
        PlayerController->ToggleMenu();
    }
}

void AFRPGCharacter::Running(const FInputActionValue& InValue)
{
    // 속도 디버프 적용 중이면 입력 무시
    if (BuffComponent->GetBuffName() == "Slow") {
        return;
    }

    // 캐릭터 속력을 높인다
    GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

void AFRPGCharacter::StopRunning(const FInputActionValue& InValue)
{
    // 속도 디버프 적용 중이면 입력 무시
    if (BuffComponent->GetBuffName() == "Slow") {
        return;
    }

    // 캐릭터 속력을 원래대로 돌아오게 한다
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AFRPGCharacter::OnCurrentStackChange(int32 InCurrentStack)
{
    FString BuffName = BuffComponent->GetBuffName();
    float Duration = BuffComponent->GetDuration();
    // 버프 호출 횟수 설정
    BuffCallsRemaining = Duration;

    // Stack 초기화 시 버프 상태 초기화
    if (InCurrentStack <= 0) {
        if (BuffName == "Slow") {
            GetCharacterMovement()->MaxWalkSpeed = 500.f;
        }
    }
    else {
        if (BuffName == "Slow") {
            // 캐릭터 속도 느리게 하기
            GetCharacterMovement()->MaxWalkSpeed = 
                FMath::Clamp<float>(GetCharacterMovement()->MaxWalkSpeed - InCurrentStack * 50.f, 0.f, 500.f);
            // 지속 시간 이후 버프 초기화
            GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
            {
                BuffDurationEnd();
            }), Duration, false);
        }
        else if (BuffName == "Poison") {
            // 지속 시간 동안 1초에 한 번씩 틱 데미지 입히기
            GetWorld()->GetTimerManager().SetTimer(myTimerHandle, this, &ThisClass::PoisonBuff, 1.0f, true);
        }
    }
}

void AFRPGCharacter::BuffDurationEnd()
{
    // 타이머 초기화
    GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
    // 버프 스택 초기화
    BuffComponent->SetCurrentStack(0);
    // 버프 이름 초기화
    BuffComponent->SetBuffName("Debuff");
}

void AFRPGCharacter::PoisonBuff()
{
    // 이 함수를 충분히 호출했으면 버프 초기화 <- 함수 호출마다 횟수 차감
    if (BuffCallsRemaining-- <= 0)
    {
        BuffDurationEnd();
    }

    // 데미지 입기
    FDamageEvent DamageEvent;
    int32 CurrentStack = BuffComponent->GetCurrentStack();
    TakeDamage(10.f * CurrentStack, DamageEvent, GetController(), this);
}

void AFRPGCharacter::OnCharacterDeath()
{
    // 캐릭터 움직임 막기
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // 2초 후 플레이어 리스폰 <- 캐릭터 죽는 애니메이션 재생 시간 기다리기
    FTimerHandle respawnTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, FTimerDelegate::CreateLambda([&]()
    {
        Respawn();
    }), 2.0f, false);
}

void AFRPGCharacter::Respawn()
{
    UFStatComponent* MyStatComponent = Cast<UFStatComponent>(Super::GetStatComponent());
    if (false == ::IsValid(MyStatComponent)) {
        return;
    }
    AGameModeBase* MyMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (false == ::IsValid(MyMode)) {
        return;
    }
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (false == ::IsValid(PC)) {
        return;
    }
    AFPlayerState* FPlayerState = GetPlayerState<AFPlayerState>();
    if (false == ::IsValid(FPlayerState)) {
        return;
    }
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // PlayerStart Tag(==CurrentStage)로 현재 스테이지 단계의 시작 위치 찾기
    int32 CurrentStage = FPlayerState->GetCurrentStage();
    FString SpotName = FString::FromInt(CurrentStage);
    AActor* StartSpot = MyMode->FindPlayerStart(PC, SpotName);

    // 캐릭터 죽음 상태 초기화
    AnimInstance->bIsDead = false;

    // 플레이어 캐릭터 리스폰 <- 현재 스테이지 단계의 시작 위치로 캐릭터 이동
    SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
    RespawnParticleSystemComponent->ActivateSystem(true);

    // 캐릭터 HP, MP 리셋
    MyStatComponent->SetCurrentHP(MyStatComponent->GetMaxHP());
    MyStatComponent->SetCurrentMP(MyStatComponent->GetMaxMP());

    // 캐릭터 움직임 허용
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AFRPGCharacter::Skill()
{
    UFAnimInstance* AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    // 공격 액션 중이거나 점프 중, 스킬 사용중이면 스킬 시전 불가
    if (bIsAttacking || AnimInstance->bIsFalling || bIsUsingSkill) {
        return;
    }

    // MP 부족 시 스킬 시전 불가
    float CurrentMP = StatComponent->GetCurrentMP();
    if (CurrentMP < 80.0f) {
        return;
    }

    // 스킬 사용 시 MP 감소
    StatComponent->SetCurrentMP(CurrentMP - 80.0f);

    // 움직이지 않고 멈춰서 애니메이션 재생
    bIsUsingSkill = true;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // FAnimInstance에서 정의한 Animation Montage를 재생시켜줄 함수
    AnimInstance->PlaySkillAnimMontage();
}

void AFRPGCharacter::CheckHit_Skill()
{
    // 스킬 이펙트 생성
    UGameplayStatics::SpawnEmitterAtLocation(
        GetWorld(), SkillParticleSystem, GetActorLocation() + SkillRange * GetActorForwardVector(), GetActorRotation());

    // 대상을 하나만 감지할 예정(Single)이라 단수 정의
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);

    // SweepSingleByChannel(): Trace channel을 사용해 물리적 충돌 여부를 조사하는 함수
    bool bSkillResult = GetWorld()->SweepSingleByChannel(
        HitResult,  // 물리적 충돌이 탐지되면 정보를 담을 구조체
        GetActorLocation() + SkillRange * GetActorForwardVector(), // 탐색을 시작할 위치
        GetActorLocation() + SkillRange * GetActorForwardVector(),   // 탐색을 끝낼 위치
        FQuat::Identity,    // 탐색에 사용할 도형의 회전 = ZeroRotator
        ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 Trace channel 정보 = Attack Channel의 Enum값
        FCollisionShape::MakeSphere(SkillRadius),  // 탐색에 사용할 기본 도형 정보
        Params // 탐색 방법에 대한 설정 값을 모은 구조체
    );

    // Hit한 대상 감지 성공
    if (true == bSkillResult) {
        // HitResult의 Actor 속성은 TWeakObjectPtr 자료형 선언 
        // -> IsValid() 함수로 유효성 검사 후 사용
        // <- GetActor() 내부에서 IsActorValid() 함수 호출됨
        if (true == ::IsValid(HitResult.GetActor())) {
            // 현재 액터가 Hit한 대상의 데미지 전달 함수 호출
            FDamageEvent DamageEvent;
            HitResult.GetActor()->TakeDamage(25.f, DamageEvent, GetController(), this);
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
        GetActorLocation() + SkillRange * GetActorForwardVector(),
        SkillRadius,
        26,
        DrawColor,
        false,
        DebugLifeTime
    );
#pragma endregion
    */
}

void AFRPGCharacter::OnSkillAnimMontageEnded(UAnimMontage* Montage, bool bIsInterrupt)
{
    // 'AM_Skill_RPG' 재생이 끝난 게 맞는지 확인
    if (bIsUsingSkill) {
        // 캐릭터 움직임 초기화
        bIsUsingSkill = false;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}
