// Shoot Them Up Game, All Rights Reserved.


#include "Menu/UI/STUMenuHUD.h"
#include "STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MenuWidgetClass)
    {
        if (const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass))
        {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}
