#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_NULL_RHI)

#include "NullRHI.h"

TNullRHI::TNullRHI()
{
}

TNullRHI::~TNullRHI()
{
}

TBool TNullRHI::Initialize()
{
	return true;
}

void TNullRHI::Shutdown()
{
}

const TChar* TNullRHI::GetName()
{
	return TRNT_NULL_RHI_NAME;
}

TVersion TNullRHI::GetVersion()	const
{
	return TVersion(1, 0, 0);
}

TGraphicsAPI TNullRHI::GetGraphicsAPI()
{
	return TGraphicsAPI::ENull;
}

#endif