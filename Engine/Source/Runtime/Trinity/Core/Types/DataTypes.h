#pragma once

using TByte = unsigned char;

/*
	Unsigned int types
*/ 

using TUInt8	= unsigned char;

using TUInt16	= unsigned short;

using TUInt32	= unsigned int;

using TUInt64	= unsigned long long;

/*
	signed int types
*/

using TInt8		= signed char;

using TInt16	= signed short;

using TInt32	= signed int;

using TInt64	= signed long long;

/*
	floating point types
*/

using TFloat = float;

using TDouble = double;

/*
	boolean type
*/

using TBool = bool;

/*
	character types
*/

#include <version>
using TChar = char;

using TWChar = wchar_t;

#ifdef __cpp_char8_t
using TChar8 = char8_t;
#endif

using TChar16 = char16_t;

using TChar32 = char32_t;

#include <cstddef>
using TSize_T = std::size_t;

/*
	Test types
*/
//////////////////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(TByte) == 1, "The size of TByte must be equal to 1.");

static_assert(sizeof(TUInt8) == 1, "The size of TUInt8 must be equal to 1.");

static_assert(sizeof(TUInt16) == 2, "The size of TUInt16 must be equal to 2.");

static_assert(sizeof(TUInt32) == 4, "The size of TUInt32 must be equal to 4.");

static_assert(sizeof(TUInt64) == 8, "The size of TUInt64 must be equal to 8.");

//////////////////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(TInt8) == 1, "The size of TInt8 must be equal to 1.");

static_assert(sizeof(TInt16) == 2, "The size of TInt16 must be equal to 2.");

static_assert(sizeof(TInt32) == 4, "The size of TInt32 must be equal to 4.");

static_assert(sizeof(TInt64) == 8, "The size of TInt64 must be equal to 8.");

//////////////////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(TFloat) == 4, "The size of TFloat must be equal to 4.");

static_assert(sizeof(TDouble) == 8, "The size of TDouble must be equal to 8.");

//////////////////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(TChar) == 1, "The size of TChar must be equal to 1.");

static_assert(sizeof(TWChar) == 2 || sizeof(TWChar) == 4, "The size of TWChar must be equal to 2 or 4.");

static_assert(sizeof(TBool) == 1, "The size of TBool must be equal to 1.");

//////////////////////////////////////////////////////////////////////////////////////////////

using TNullPtr = std::nullptr_t;