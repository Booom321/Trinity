#pragma once

#include "Trinity/Core/TypeTraits/TypeRelationships.h"
#include "Trinity/Core/Types/DataTypes.h"

class THashable
{
public:
	THashable() = default;
	~THashable() = default;

	virtual TSize_T CalculateHash() const = 0;
};