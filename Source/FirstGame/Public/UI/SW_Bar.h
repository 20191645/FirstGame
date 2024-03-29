// SW_Bar.h

#pragma once

#include "CoreMinimal.h"
#include "UI/FirstUserWidget.h"
#include "SW_Bar.generated.h"

UCLASS()
class FIRSTGAME_API USW_Bar : public UFirstUserWidget
{
	GENERATED_BODY()
	
public:
	// 만약 Widget 클래스에서 생성자에 로직을 작성한다면,
	// 기본 생성자가 아닌 해당 생성자를 선언/정의해야 한다
	USW_Bar(const FObjectInitializer& ObjectInitializer);

	void SetMaxFigure(float InMaxFigure);

protected:
	virtual void NativeConstruct() override;

protected:
	// ProgreeBar 위젯을 바인드할 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USW_Bar")
	TObjectPtr<class UProgressBar> Bar;

	// Bar(HP, MP...)의 최대값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USW_Bar")
	float MaxFigure;
};
