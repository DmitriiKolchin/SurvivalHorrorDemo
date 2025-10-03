#pragma once

#include "CoreMinimal.h"

#define GET_FUNCTION_NAME_STRING() *FString(__FUNCTION__)

#define THROW_ERROR_IN_NON_SHIPPING_BUILDS(FunctionName, ErrorMessage) \
	ensureAlwaysMsgf( false, TEXT("%s(): %s"), *FString(FunctionName), *FString(ErrorMessage) );