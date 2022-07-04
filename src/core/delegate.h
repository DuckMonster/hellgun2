#pragma once

template<typename TAllocator, typename TReturn, typename... TArgs>
struct Delegate_Base
{
	class Impl
	{
	public:
		virtual TReturn execute(TArgs... args) = 0;
		virtual Impl* clone() = 0;
	};

	class Impl_Static : public Impl
	{
	public:
		using Func_Type = TReturn (*)(TArgs...);

		Impl_Static(Func_Type func) : func(func) {}

		TReturn execute(TArgs... args) override { return func(args...); }
		Impl* clone() override { return TAllocator::malloc_new<Impl_Static>(func); }

	private:
		Func_Type func;
	};

	template<typename TObject>
	class Impl_Method : public Impl
	{
	public:
		using Func_Type = TReturn (TObject::*)(TArgs...);

		Impl_Method(TObject* obj, Func_Type func) : obj(obj), func(func) {}

		TReturn execute(TArgs... args) override
		{
			return ((*obj).*func)(args...);
		}
		Impl* clone() override { return TAllocator::malloc_new<Impl_Method<TObject>>(obj, func); }

	private:
		TObject* obj;
		Func_Type func;
	};

	template<typename TLambda>
	class Impl_Lambda : public Impl
	{
	public:
		Impl_Lambda(const TLambda& lambda) : lambda(lambda) {}

		TReturn execute(TArgs... args) override { return lambda(args...); }
		Impl* clone() override { return TAllocator::malloc_new<Impl_Lambda<TLambda>>(lambda); }

	private:
		TLambda lambda;
	};

	static Delegate_Base make_static(typename Impl_Static::Func_Type func) { return Delegate_Base(func); }
	template<typename TLambda>
	static Delegate_Base make_lambda(const TLambda& lambda) { return Delegate_Base(lambda); }
	template<typename TObject>
	static Delegate_Base make_method(TObject* obj, typename Impl_Method<TObject>::Func_Type func) { return Delegate_Base(obj, func); }

	Delegate_Base() {}
	Delegate_Base(const Delegate_Base& other)
	{
		if (other.impl)
			impl = other.impl->clone();
	}
	Delegate_Base(Delegate_Base&& other)
	{
		impl = other.impl;
		other.impl = nullptr;
	}
	~Delegate_Base() { clear(); }

	Delegate_Base(typename Impl_Static::Func_Type func)
	{
		bind_static(func);
	}
	template<typename TLambda>
	Delegate_Base(const TLambda& lambda)
	{
		bind_lambda<TLambda>(lambda);
	}
	template<typename TClass>
	Delegate_Base(TClass* obj, typename Impl_Method<TClass>::Func_Type func)
	{
		bind_method(obj, func);
	}

	void operator=(const Delegate_Base& other)
	{
		clear();
		if (other.impl)
			impl = other.impl->clone();
	}
	void operator=(Delegate_Base&& other)
	{
		clear();
		impl = other.impl;
		other.impl = nullptr;
	}

	void bind_static(typename Impl_Static::Func_Type func)
	{
		clear();
		impl = TAllocator::malloc_new<Impl_Static>(func);
	}
	template<typename TLambda>
	void bind_lambda(const TLambda& lambda)
	{
		clear();
		impl = TAllocator::malloc_new<Impl_Lambda<TLambda>>(lambda);
	}
	template<typename TObject>
	void bind_method(TObject* obj, typename Impl_Method<TObject>::Func_Type func)
	{
		clear();
		impl = TAllocator::malloc_new<Impl_Method<TObject>>(obj, func);
	}

	void clear()
	{
		if (impl)
		{
			TAllocator::free_delete(impl);
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

template<typename... TArgs>
using Delegate = Delegate_Base<Heap_Allocator, TArgs...>;

template<typename... TArgs>
using TDelegate = Delegate_Base<Temp_Allocator, TArgs...>;