#ifndef BIT_VECTOR_ADAPTED_
#define BIT_VECTOR_ADAPTED_
#include "SingularAdapters.h"
#include <vector>
#include <concepts>

template<std::convertible_to<std::vector<bool>::size_type> T>
class BitVectorAdapted : public SingularContainerAdapter<T, BitVectorAdapted>
{
public:
	using container_type = typename AdapterTraits<BitVectorAdapted<T>>::container_type;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = typename container_type::value_type;
public:
	constexpr BitVectorAdapted() : container_{} {}
	constexpr BitVectorAdapted(const size_type size) : container_(size) {}
	constexpr BitVectorAdapted(std::initializer_list<value_type> init) : container_{ init } {}
	constexpr BitVectorAdapted(BitVectorAdapted&& container) noexcept :
		container_{ std::move(container.container_) } {}
	~BitVectorAdapted() override = default;
public:
	void add(const T& elem)
	{
		const size_type index = static_cast<size_type>(elem);
		if (index >= size())
			container_.resize(index + 1);
		container_[index] = true;
	}

	void reserve(const size_type size)
	{
		container_.reserve(size);
	}

	bool operator[] (const size_type ind) const
	{
		return container_[ind];
	}
	
	decltype(auto) operator[] (const size_type ind)
	{
		return container_[ind];
	}

	constexpr bool contains(const T& elem) const noexcept
	{
		const size_type index = static_cast<size_type>(elem);
		return index < size() && container_[index];
	}

	[[nodiscard]] constexpr iterator begin() noexcept
	{
		return container_.begin();
	}

	[[nodiscard]] constexpr iterator end() noexcept
	{
		return container_.end();
	}

	[[nodiscard]] constexpr const_iterator begin() const noexcept
	{
		return container_.begin();
	}

	[[nodiscard]] constexpr const_iterator end() const noexcept
	{
		return container_.end();
	}

	[[nodiscard]] constexpr const_iterator cbegin() noexcept
	{
		return container_.cbegin();
	}

	[[nodiscard]] constexpr const_iterator cend() noexcept
	{
		return container_.cend();
	}

	constexpr size_type size() const noexcept
	{
		return container_.size();
	}

	void resize(const size_type n)
	{
		container_.resize(n);
	}

private:
	container_type container_;
};

template<typename T>
struct AdapterTraits<BitVectorAdapted<T>>
{
	using container_type = std::vector<bool>;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = typename container_type::value_type;
};

#endif // !BIT_VECTOR_ADAPTED_
