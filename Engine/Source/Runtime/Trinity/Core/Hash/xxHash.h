#pragma once

#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Defines.h"

#define XXH_INLINE_ALL
#include <xxhash.h> 

class FXxHash32
{
public:
	TUInt32 HashCode{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash32 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		return FXxHash32{ XXH32(Buffer, Size, 0) };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash32 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt32 Seed)
	{
		return FXxHash32{ XXH32(Buffer, Size, Seed) };
	}

	TRNT_FORCE_INLINE TBool operator==(const FXxHash32& Other) const
	{
		return HashCode == Other.HashCode;
	}

	TRNT_FORCE_INLINE TBool operator!=(const FXxHash32& Other) const
	{
		return HashCode != Other.HashCode;
	}
};

class FXxHash64
{
public:
	TUInt64 HashCode{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash64 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		return FXxHash64{ XXH3_64bits(Buffer, Size) };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash64 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt64 Seed)
	{
		return FXxHash64{ XXH3_64bits_withSeed(Buffer, Size, Seed) };
	}

	TRNT_FORCE_INLINE TBool operator==(const FXxHash64& Other) const
	{
		return HashCode == Other.HashCode;
	}

	TRNT_FORCE_INLINE TBool operator!=(const FXxHash64& Other) const
	{
		return HashCode != Other.HashCode;
	}
};

class FXxHash128
{
public:
	TUInt64 HashLow64{};
	TUInt64 HashHigh64{};

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash128 HashBuffer(const void* Buffer, TUInt64 Size)
	{
		const XXH128_hash_t HashCode = XXH3_128bits(Buffer, Size);
		return FXxHash128{ HashCode.low64, HashCode.high64 };
	}

	TRNT_NODISCARD static TRNT_FORCE_INLINE FXxHash128 HashBufferWithSeed(const void* Buffer, TUInt64 Size, TUInt64 Seed)
	{
		const XXH128_hash_t HashCode = XXH3_128bits_withSeed(Buffer, Size, Seed);
		return FXxHash128{ HashCode.low64, HashCode.high64 };
	}

	TRNT_FORCE_INLINE TBool operator==(const FXxHash128& Other) const
	{
		return HashLow64 == Other.HashLow64 && HashHigh64 == Other.HashHigh64;
	}

	TRNT_FORCE_INLINE TBool operator!=(const FXxHash128& Other) const
	{
		return HashLow64 == Other.HashLow64 || HashHigh64 == Other.HashHigh64;
	}
};