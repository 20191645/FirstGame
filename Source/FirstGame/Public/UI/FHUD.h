// FHUD.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FHUD.generated.h"

UCLASS()
class FIRSTGAME_API UFHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 'StatComponent' 속성을 변수로 입력받아 바인드할 함수
	void BindStatComponent(class UFStatComponent* InStatComponent);

protected:
	// HP 속성 데이터를 가져오기 위해 바인드할 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFHUD")
	TWeakObjectPtr<class UFStatComponent> StatComponent;

	// 'WBP_HPBar' 위젯과 바인드할 속성
	// meta = (BindWidget): 하드코딩으로 속성과 위젯을 바인드하지 않아도 된다
	// => Hierarchy에서 'HPBar' 이름의 위젯을 찾아서 바인드해준다
	// -- 하드코딩: "Bar = Cast<UProgressBar>(GetWidgetFromName("BarWidget"));"
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UFHUD", meta = (BindWidget))
	TObjectPtr<class USW_HPBar> HPBar;
};