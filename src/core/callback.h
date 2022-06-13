#pragma once
#include "container/array.h"

template<typename... TArgs>
struct Callback
{
	class Entry
	{
	public:
		virtual void execute(TArgs... args) {}
	};

	template<typename TLambda>
	class Entry_Lambda : public Entry
	{
	public:
		Entry_Lambda(const TLambda& lambda) : lambda(lambda) {}

		void execute(TArgs... args) override
		{
			lambda(args...);
		}

	private:
		TLambda lambda;
	};

public:
	Callback() {}
	Callback(const Callback& other) = delete;
	Callback(Callback&& other) = delete;
	~Callback()
	{
		for(Entry* entry : entries)
			delete entry;

		entries.empty();
	}

	template<typename TLambda>
	void bind_lambda(const TLambda& lambda)
	{
		entries.add(new Entry_Lambda<TLambda>(lambda));
	}

	void execute(TArgs... args)
	{
		for(auto* entry : entries)
			entry->execute(args...);
	}

private:
	Array<Entry*> entries;
};