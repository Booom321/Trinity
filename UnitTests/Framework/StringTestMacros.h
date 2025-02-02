#undef TRNT_TEXT
#undef TRNT_CHAR_TYPE
#undef TRNT_TEST_CHAR_TYPE
#undef TRNT_TEST_WCHAR_TYPE

#if defined(TRNT_TEST_CHAR)
	#define TRNT_TEXT(Str) Str
	#define TRNT_CHAR_TYPE char
	#define TRNT_TEST_CHAR_TYPE
	#undef TRNT_TEST_CHAR
#elif defined(TRNT_TEST_WCHAR)
	#define TRNT_TEXT(Str) L##Str
	#define TRNT_CHAR_TYPE wchar_t
	#define TRNT_TEST_WCHAR_TYPE
	#undef TRNT_TEST_WCHAR
#endif
