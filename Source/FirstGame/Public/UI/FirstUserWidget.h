// FirstUserWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstUserWidget.generated.h"

UCLASS()
class FIRSTGAME_API UFirstUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 'OwningActor' 속성의 Getter, Setter
	AActor * GetOwningActor() const { return OwningActor; }
	void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

protected:
	// 현재 UI를 소유한 액터 정보를 담을 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFirstUserWidget")
	TObjectPtr<AActor> OwningActor;
};
