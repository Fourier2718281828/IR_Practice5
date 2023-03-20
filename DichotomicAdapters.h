#ifndef DICHOTOMIC_ADAPTERS_
#define DICHOTOMIC_ADAPTERS_

#include <unordered_set>
#include <unordered_map>


template<typename Derived>
struct DichotomicAdapterTraits;

template
<
	typename T,
	typename U,
	template<typename, typename> typename Derived
>
class DichotomicContainerAdapter
{
public:
	using const_iterator = typename DichotomicAdapterTraits<Derived<T, U>>::const_iterator;
	using iterator = typename DichotomicAdapterTraits <Derived<T, U>>::iterator;
	using size_type = typename DichotomicAdapterTraits <Derived<T, U>>::size_type;
	using value_type = typename DichotomicAdapterTraits <Derived<T, U>>::value_type;
public:
	virtual ~DichotomicContainerAdapter() = default;
protected:
	constexpr DichotomicContainerAdapter() = default;
public:

	template<typename TT, typename UU>
	void add(TT&& key, UU&& elem)
	{
		get_this().add(std::forward<TT>(key), std::forward<UU>(elem));
	}

	template<typename TT>
	U& operator[] (TT&& key)
	{
		return get_this().operator[](std::forward<TT>(key));
	}

	const U& operator[] (const T& key) const
	{
		return get_this().operator[](key);
	}

	constexpr size_type size() const noexcept
	{
		return get_this().size();
	}

	constexpr bool contains(const T& elem) const noexcept
	{
		return get_this().contains(elem);
	}

	[[nodiscard]] constexpr iterator begin() noexcept
	{
		return get_this().begin();
	}

	[[nodiscard]] constexpr iterator end() noexcept
	{
		return get_this().end();
	}

	[[nodiscard]] constexpr const_iterator begin() const noexcept
	{
		return get_this().begin();
	}

	[[nodiscard]] constexpr const_iterator end() const noexcept
	{
		return get_this().end();
	}

	[[nodiscard]] constexpr const_iterator cbegin() noexcept
	{
		return get_this().cbegin();
	}

	[[nodiscard]] constexpr const_iterator cend() noexcept
	{
		return get_this().cend();
	}


private:
	Derived<T, U>& get_this() noexcept
	{
		return static_cast<Derived<T, U>&>(*this);
	}

	const Derived<T, U>& get_this() const noexcept
	{
		return static_cast<const Derived<T, U>&>(*this);
	}
};

#endif // !DICHOTOMIC_ADAPTERS_
