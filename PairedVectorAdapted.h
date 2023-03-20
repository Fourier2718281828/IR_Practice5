#ifndef PAIRED_VECTOR_ADAPTER_
#define PAIRED_VECTOR_ADAPTER_
#include "DichotomicAdapters.h"
#include <vector>

template<typename T, typename U>
using paired_vector = std::vector<std::pair<T, U>>;

template<typename T, typename U>
class PairedVectorAdapted : public DichotomicContainerAdapter<T, U, PairedVectorAdapted>
{
public:
	using container_type = typename DichotomicAdapterTraits<PairedVectorAdapted<T, U>>::container_type;
	using const_iterator = container_type::const_iterator;
	using iterator = container_type::iterator;
	using size_type = container_type::size_type;
	using value_type = container_type::value_type;
public:
	constexpr PairedVectorAdapted() : container_{} {}
	constexpr PairedVectorAdapted(PairedVectorAdapted&& container) noexcept :
		container_{ std::move(container.container_) } {}
	~PairedVectorAdapted() override = default;
public:

	template<typename TT, typename UU>
	void add(TT&& key, UU&& val)
	{
		container_.push_back({ std::forward<TT>(key), std::forward<UU>(val) });
	}

	template<typename T>
	constexpr iterator insert(const_iterator pos, T&& value)
	{
		return container_.insert(pos, std::forward<T>(value));
	}

	U& operator[] (const T& key)
	{
		auto found = std::find_if
		(
			begin(),
			end(),
			[&key](const auto& pair) { return pair.first == key; }
		);

		if (found == end())
		{
			add(key, U());
			return container_[size() - 1].second;
		}
		else
		{
			return found->second;
		}
	}

	const U& operator[] (const T& key) const
	{
		auto found = std::find_if
		(
			begin(),
			end(),
			[&key](const auto& pair) { return pair.first == key; }
		);

		return found == end()
			? throw std::runtime_error("No such a key element in the container.")
			: found->second;
	}

	constexpr bool contains(const T& elem) const noexcept
	{
		return std::find_if
		(
			begin(),
			end(),
			[&elem](const auto& pair) { return pair.first == elem; }
		) != end();
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

	void resize(const size_type n) const
	{
		container_.resize(n);
	}

private:
	container_type container_;
public:

	static constexpr const inline bool s_IS_SORTED_POLICY = false;
};

template<typename T, typename U>
struct DichotomicAdapterTraits<PairedVectorAdapted<T, U>>
{
	using container_type = paired_vector<T, U>;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = typename container_type::value_type;
};

#endif // !PAIRED_VECTOR_ADAPTER_
