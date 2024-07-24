#pragma once

#if defined(TRNT_SUPPORT_NULL_RHI) 

#include "Trinity/RHI/RHI.h"

class TRNT_API TNullRHI : public TGenericRHI
{
public:
	TNullRHI();
	virtual ~TNullRHI();

	virtual TBool Initialize() override;
	virtual void Shutdown() override;
	virtual const TChar* GetName() override;
	virtual TVersion GetVersion() const override;
	virtual TGraphicsAPI GetGraphicsAPI() override;
};

#endif 