// FCharacter.cpp

#include "Characters/FCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AFCharacter::AFCharacter()
{
 	PrimaryActorTick.bCanEverTick = false;

    float CharacterHalfHeight = 95.f;
    float CharacterRadius = 40.f;

    GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

    // PivotPosition, PivotRotation: 캡슐 컴포넌트와 에셋의 위치, 방향을 맞춰줌
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    // TargetArmLength: 카메라 봉 길이
    SpringArmComponent->TargetArmLength = 400.f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    // 카메라 봉이 회전하면 자식인 카메라도 함께 회전하도록
    CameraComponent->SetupAttachment(SpringArmComponent);

    // MaxWalkSpeed: 캐릭터가 Walk mode일 때 최대 속력
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    // MinAnalogWalkSpeed: 디지털과 아날로그(키보드 - 조이스틱) 입력 움직임 간극의 보완값
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    // JumpZVelocity: 캐릭터가 점프할 때 속도
    GetCharacterMovement()->JumpZVelocity = 700.f;
    // AirControl: 공중에 떠있을 때 움직일 수 있는 정도
    GetCharacterMovement()->AirControl = 0.35f;
    // BrakingDecelerationWalking: 움직임을 멈췄을 때 멈추는 속도
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}
