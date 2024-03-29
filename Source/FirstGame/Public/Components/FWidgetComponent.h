// FWidgetComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FWidgetComponent.generated.h"

UCLASS()
class FIRSTGAME_API UFWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	// 액터의 BeginPlay() 이후에 호출되는 함수
	// -- 해당 함수 호출 이후에 위젯에서 자신을 소유한 액터를 알 수 있다
	virtual void InitWidget() override;
};
