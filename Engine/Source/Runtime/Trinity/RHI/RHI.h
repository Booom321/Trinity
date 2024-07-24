#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/Version.h"

#include "RHIDefinitions.h"

class TRNT_API TGenericRHI
{
public:
	virtual ~TGenericRHI();

	virtual TBool Initialize()	= 0;
	virtual void Shutdown()		= 0;

	virtual const TChar* GetName()			= 0;
	virtual TVersion GetVersion() const		= 0;
	virtual TGraphicsAPI GetGraphicsAPI()	= 0;

public:
	static TGenericRHI* CreateRHI(const TGraphicsAPI GraphicsAPI, TBool& Initialized);
	static void	DestroyRHI(TGenericRHI* RHIPointer);

	static void SetGlobalInstance(TGenericRHI* RHIPointer);
	static TRNT_FORCE_INLINE TGenericRHI* GetGlobalInstance() { return GlobalRHIInstance; }
	
public:
	static const TChar* ConvertGraphicsAPIToCString(const TGraphicsAPI GraphicsAPI);
	
protected:
	static TGenericRHI* GlobalRHIInstance;

	TGraphicsAPI GraphicsAPI;
	const TChar* RHIName;
};