// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "STUPlayerCharacter.h"
#include "ExtendedPlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Engine/EngineTypes.h"
//#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitFX();

    GetWorldTimerManager().ClearTimer(DecreaseBulletSpreadTimerHandle);
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    
    if(BulletSpread)
    {
        GetWorldTimerManager().SetTimer(IncreaseBulletSpreadTimerHandle, this, &ASTURifleWeapon::IncreaseBulletSpreadModifier, BulletSpreadIncreaseTime, true);
    }
    
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    if (BulletSpread)
    {
        GetWorldTimerManager().ClearTimer(IncreaseBulletSpreadTimerHandle);
        GetWorldTimerManager().SetTimer(ResetBulletSpreadTimerHandle, this, &ASTURifleWeapon::ResetBulletSpreadModifier, BulletSpreadResetTime, false);
    }
    
    SetFXActive(false);
}

void ASTURifleWeapon::ResetBulletSpreadModifier()
{
    GetWorldTimerManager().ClearTimer(ResetBulletSpreadTimerHandle);
    GetWorldTimerManager().SetTimer(DecreaseBulletSpreadTimerHandle, this, &ASTURifleWeapon::DecreaseBulletSpreadModifier, BulletSpreadDecreaseTime, true);
}

void ASTURifleWeapon::IncreaseBulletSpreadModifier()
{
    if (CurrentBulletSpreadModifier < MaxBulletSpreadModifier)
    {
        CurrentBulletSpreadModifier += BulletSpreadIncreaseTime;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(IncreaseBulletSpreadTimerHandle);
    }
}

void ASTURifleWeapon::DecreaseBulletSpreadModifier()
{
    if (CurrentBulletSpreadModifier > InitialBulletSpreadModifier)
    {
        CurrentBulletSpreadModifier -= BulletSpreadDecreaseTime;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(DecreaseBulletSpreadTimerHandle);
    }
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty() || Cast<ASTUBaseCharacter>(GetOwner())->IsRunning())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    DecreaseAmmo();
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
    CurrentBulletSpreadModifier = InitialBulletSpreadModifier;
}

void ASTURifleWeapon::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    if(!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if(MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }

    if(FireAudioComponent)
    {
        //TODO: Исправить баг с SpawnSoundAttached! 
        FireAudioComponent->SetPaused(!IsActive);
        UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetMuzzleWorldLocation(), 1.0f, GetOwner(), 30000.0f, FName("Fire"));
        //TODO: IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop(); СЛОМАНО
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if(TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();

    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    DamagedActor->TakeDamage(Damage, PointDamageEvent, GetController(), this);
}


AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const auto HalfRad = BulletSpread ? FMath::DegreesToRadians(CurrentBulletSpreadModifier) : FMath::DegreesToRadians(BulletSpreadIncreaseTime);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::Zoom(bool Enabled)
{
    const auto Controller = Cast<APlayerController>(GetController());
    if(!Controller || !Controller->PlayerCameraManager) return;

    if(Enabled)
    {
        DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
    }
    Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}
