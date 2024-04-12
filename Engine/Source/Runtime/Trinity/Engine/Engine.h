#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"
//#include "Trinity/Core/String/String.h"

class TRNT_API TEngine
{
public:
	static TBool Initialize(TInt32 Argc, TChar** Argv);

	static void Destroy();
};