#ifndef MAP_ADAPTER_
#define MAP_ADAPTER_
#include "DichotomicAdapters.h"
#include <map>

template<typename T, typename U>
class MapAdapted : public DichotomicContainerAdapter<T, U, MapAdapted>
{
public:
	using container_type = typename DichotomicAdapterTraits<MapAdapted<T, U>>::container_type;
	using const_iterator = container_type::const_iterator;
	using iterator = container_type::iterator;
	using size_type = container_type::size_type;
	using value_type = container_type::value_type;
public:
	constexpr MapAdapted() : container_{} {}
	constexpr MapAdapted(MapAdapted&& container) noexcept :
		container_{ std::move(container.container_) } {}
	~MapAdapted() override = default;
public:

	void add(const T& key, const U& val)
	{
		container_.operator[](key) = val;
	}

	template<typename TT>
	U& operator[] (TT&& key)
	{
		return container_.operator[](std::forward<TT>(key));
	}

	const U& operator[] (const T& key) const
	{
		return container_.at(key);
	}

	constexpr bool contains(const T& elem) const noexcept
	{
		return container_.contains(elem);
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
private:
	container_type container_;
public:

	static constexpr const inline bool s_IS_SORTED_POLICY = false;
};

template<typename T, typename U>
struct DichotomicAdapterTraits<MapAdapted<T, U>>
{
	using container_type = std::map<T, U>;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = typename container_type::value_type;
};

#endif // !MAP_ADAPTER_
