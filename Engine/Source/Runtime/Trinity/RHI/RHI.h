#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/Version.h"

#include "RHIDefinitions.h"

class TRNT_API TRHI
{
public:
	virtual ~TRHI();

	virtual TBool Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual const TChar* GetName() = 0;
	virtual TVersion GetVersion() const = 0;
	virtual TRHIType GetType() = 0;

public:
	static TRHI* CreateRHI(const TRHIType RHIType);
	static void	DestroyRHI(TRHI* RHIPointer);

	static void SetGlobalInstance(TRHI* RHIPointer);

	static TRNT_FORCE_INLINE TRHI* GetGlobalInstance() { return GlobalRHIInstance; }
	
public:
	static const TChar* ConvertRHITypeToCString(const TRHIType RHIType);
	
protected:
	static TRHI* GlobalRHIInstance;
};