// Shoot Them Up Game, All Rights Reserved.


#include "Menu/UI/STUOptionsWidget.h"

#include "STUMenuGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


<<<<<<< HEAD
DEFINE_LOG_CATEGORY_STATIC(LogSTUOptionsWidget, All, All)
=======
//DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All)
>>>>>>> e11608d07719b8b88f80fdd76e7be55f8213d2e8

void USTUOptionsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USTUOptionsWidget::OnMainMenu);
    }
}

void USTUOptionsWidget::OnMainMenu()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    const auto GameMode = Cast<ASTUMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->MainMenu();
}