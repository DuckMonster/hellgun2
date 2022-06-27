#pragma once

class Hash
{
public:
	static u64 fnv64(const void* data, u32 size);

	template<typename T>
	static u64 fnv64(const T& value) { return fnv64(&value, sizeof(T)); }
};

template<>
u64 Hash::fnv64<String>(const String& string);
template<>
u64 Hash::fnv64<TString>(const TString& string);