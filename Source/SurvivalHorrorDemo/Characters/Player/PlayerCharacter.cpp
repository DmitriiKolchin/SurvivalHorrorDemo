// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SurvivalHorrorDemo/Utilities/Macros/UtilityMacros.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Checks
	{
		if (!IsValid( EnhancedInputComponent ))
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("EnhancedInputComponent is not valid.") );
			return;
		}
		if (!IsValid( PlayerInputs.MappingContext ))
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerInputs.MappingContext is not valid.") );
			return;
		}
		if (!IsValid( PlayerInputs.MoveInputAction ))
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerInputs.MoveInputAction is not valid.") );
			return;
		}
		if (!IsValid( PlayerInputs.TurnCameraInputAction ))
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerInputs.TurnCameraInputAction is not valid.") );
			return;
		}
	}

	EnhancedInputComponent->BindAction( PlayerInputs.MoveInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move );
	EnhancedInputComponent->BindAction( PlayerInputs.TurnCameraInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TurnCamera );
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy( NewController );

	{
		if (!IsValid( PlayerInputs.MappingContext ))
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerInputs.MappingContext is not valid.") );
			return;
		}

		if ( !IsValid( NewController ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("NewController is not valid.") );
			return;
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>( NewController );
	{
		if ( !IsValid( PlayerController ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerController is not valid.") );
			return;
		}
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	{
		if ( !IsValid( LocalPlayer ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("LocalPlayer is not valid.") );
			return;
		}
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer );
	{
		if ( !IsValid( EnhancedInputLocalPlayerSubsystem ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("EnhancedInputLocalPlayerSubsystem is not valid.") );
			return;
		}
	}

	EnhancedInputLocalPlayerSubsystem->AddMappingContext( PlayerInputs.MappingContext, 0 );
}

void APlayerCharacter::UnPossessed()
{
	if ( !IsValid( PlayerInputs.MappingContext ) )
	{
		THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerInputs.MappingContext is not valid.") );

		Super::UnPossessed();
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>( Controller );
	{
		if ( !IsValid( PlayerController ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("PlayerController is not valid.") );
			return;
		}
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	{
		if ( !IsValid( LocalPlayer ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("LocalPlayer is not valid.") );
			return;
		}
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer );
	{
		if ( !IsValid( EnhancedInputLocalPlayerSubsystem ) )
		{
			THROW_ERROR_IN_NON_SHIPPING_BUILDS( GET_FUNCTION_NAME_STRING(), TEXT("EnhancedInputLocalPlayerSubsystem is not valid.") );
			return;
		}
	}
	EnhancedInputLocalPlayerSubsystem->RemoveMappingContext( PlayerInputs.MappingContext );

	Super::UnPossessed();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector RightVector = CameraComponent->GetRightVector();

	FVector MovementDirection = ForwardVector * Value.Get<FVector2D>().Y + RightVector * Value.Get<FVector2D>().X;
	GetMovementComponent()->AddInputVector( MovementDirection );
}

void APlayerCharacter::TurnCamera(const FInputActionValue& Value)
{
	AddControllerYawInput( Value.Get<FVector2D>().X );
	AddControllerPitchInput( Value.Get<FVector2D>().Y );
}

