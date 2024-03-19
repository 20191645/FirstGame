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
         // -- Animation Montage('AM_Attack_Dagger')가 끝나면 함수 호출
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackMontageEnded);
        // Animation Notify(CheckHit)의 델리게이트에 CheckHit() 멤버 함수 바인드
        AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        // Animation Notify(CheckCanNextCombo)의 델리게이트에 CheckCanNextCombo() 멤버 함수 바인드
        AnimInstance->OnCheckCanNextAttackDelegate.AddDynamic(this, &ThisClass::CheckCanNextAttack);
    }
}

void AFRPGCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Animation Montage가 끝났으므로 다시 걷는 모드로 변경
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    bIsAttacking = false;
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
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CheckHit()")));
}

void AFRPGCharacter::BeginAttack()
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
    // 공격 액션이 끝났으므로 다시 움직이는 모드로 변경
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
