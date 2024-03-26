#pragma once

#include "Trinity/Core/Types/DataTypes.h"

#include "Trinity/Core/TypeTraits/TypeRelationships.h"

class THashable
{
public:
	THashable() = default;
	~THashable() = default;

	virtual TSize_T CalculateHash() const = 0;
};