// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;
class USTURespawnComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()
public:
    ASTUAIController();
    virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUAIPerceptionComponent* STUAIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent* STURespawnComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
    FName FocusOnKeyName = "EnemyActor";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    FVector EnemyHeight = FVector(0, 0, 88.0f);

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
};