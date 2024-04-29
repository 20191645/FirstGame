// FCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

UCLASS()
class FIRSTGAME_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFCharacter();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	class UFStatComponent* GetStatComponent() { return StatComponent; }

	// 캐릭터에게 위젯 세팅 -- 자식 클래스에서 각각 알아서 정의할 수 있도록 virtual 정의
	virtual void SetWidget(class UFirstUserWidget* InFirstUserWidget) {}

protected:
	// SpringArm, Camera Component는 Character Class에 포함되어 있지 않기 때문에 직접 추가
	// SpringArmComponent: 3인칭 시점 카메라 구도 설정 돕는 컴포넌트 - 카메라 봉 길이, 컴포넌트 회전 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	// CameraComponenet: 가상 세계 모습을 플레이어 화면에 전송하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AFCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;

	// Character 클래스에 부착할 Stat Component 데이터를 담을 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AFCharacter", Meta = (AllowPrivateAccess))
	TObjectPtr<class UFStatComponent> StatComponent;
};
