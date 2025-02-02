#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"

#define XXH_INLINE_ALL
#include <xxhash.h>

class TXxHash32
{
public:
	TUInt32 HashCode{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash32 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		return TXxHash32{ XXH32(Buffer, Size, 0) };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash32 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt32 Seed)
	{
		return TXxHash32{ XXH32(Buffer, Size, Seed) };
	}

	TRNT_FORCE_INLINE TBool operator==(const TXxHash32& Other) const
	{
		return HashCode == Other.HashCode;
	}

	TRNT_FORCE_INLINE TBool operator!=(const TXxHash32& Other) const
	{
		return HashCode != Other.HashCode;
	}
};

class TXxHash64
{
public:
	TUInt64 HashCode{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash64 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		return TXxHash64{ XXH3_64bits(Buffer, Size) };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash64 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt64 Seed)
	{
		return TXxHash64{ XXH3_64bits_withSeed(Buffer, Size, Seed) };
	}

	TRNT_FORCE_INLINE TBool operator==(const TXxHash64& Other) const
	{
		return HashCode == Other.HashCode;
	}

	TRNT_FORCE_INLINE TBool operator!=(const TXxHash64& Other) const
	{
		return HashCode != Other.HashCode;
	}
};

class TXxHash128
{
public:
	TUInt64 HashLow64{};
	TUInt64 HashHigh64{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash128 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		const XXH128_hash_t HashCode = XXH3_128bits(Buffer, Size);
		return TXxHash128{ HashCode.low64, HashCode.high64 };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE TXxHash128 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt64 Seed)
	{
		const XXH128_hash_t HashCode = XXH3_128bits_withSeed(Buffer, Size, Seed);
		return TXxHash128{ HashCode.low64, HashCode.high64 };
	}

	TRNT_FORCE_INLINE TBool operator==(const TXxHash128& Other) const
	{
		return HashLow64 == Other.HashLow64 && HashHigh64 == Other.HashHigh64;
	}

	TRNT_FORCE_INLINE TBool operator!=(const TXxHash128& Other) const
	{
		return HashLow64 == Other.HashLow64 || HashHigh64 == Other.HashHigh64;
	}
};