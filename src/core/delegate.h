#pragma once

template<typename TReturn, typename... TArgs>
struct Delegate
{
	class Impl
	{
	public:
		virtual TReturn execute(TArgs... args) = 0;
		virtual Impl* clone() = 0;
	};

	template<typename TLambda>
	class Impl_Lambda : public Impl
	{
	public:
		Impl_Lambda(const TLambda& lambda) : lambda(lambda) {}
		TReturn execute(TArgs... args) override
		{
			return lambda(args...);
		}

		Impl* clone() override
		{
			return new Impl_Lambda<TLambda>(lambda);
		}

	private:
		TLambda lambda;
	};

	Delegate() {}
	template<typename TLambda>
	Delegate(const TLambda& lambda)
	{
		bind_lambda<TLambda>(lambda);
	}
	Delegate(const Delegate& other)
	{
		if (other.impl)
			impl = other.impl->clone();
	}
	Delegate(Delegate&& other)
	{
		impl = other.impl;
		other.impl = nullptr;
	}
	~Delegate() { clear(); }

	template<typename TLambda>
	void bind_lambda(const TLambda& lambda)
	{
		clear();
		impl = new Impl_Lambda<TLambda>(lambda);
	}

	void clear()
	{
		if (impl)
		{
			delete impl;
			impl = nullptr;
		}
	}

	bool is_bound() { return impl != nullptr; }

	TReturn execute(TArgs... args)
	{
		if (!is_bound())
		{
			fatal("Executing unbound delegate");
			return TReturn();
		}

		impl->execute(args...);
	}

private:
	Impl* impl = nullptr;
};