#include "hash.h"

u64 Hash::fnv64(const void* data, u32 size)
{
	static u64 offset = 0xcbf29ce484222325;
	static u64 prime = 0x100000001b3;

	u64 hash = offset;
	u8* bytes = (u8*)data;

	for(u32 i = 0; i < size; ++i)
	{
		hash = hash ^ bytes[i];
		hash = hash * prime;
	}

	return hash;
}

template<>
u64 Hash::fnv64<String>(const String& string) { return fnv64(string.data(), string.length()); }
template<>
u64 Hash::fnv64<TString>(const TString& string) { return fnv64(string.data(), string.length()); }