#ifndef SINGULAR_MAP_
#define SINGULAR_MAP_
#include <map>
#include <set>
#include "SingularAdapters.h"

using coordinate_t = size_t;

template<typename T>
class CoordMapAdapted : public SingularContainerAdapter<T, CoordMapAdapted>
{
public:
	using container_type = typename AdapterTraits<CoordMapAdapted<T>>::container_type;
	using const_iterator = typename AdapterTraits<CoordMapAdapted<T>>::const_iterator;
	using iterator = typename AdapterTraits<CoordMapAdapted<T>>::iterator;
	using size_type = typename AdapterTraits<CoordMapAdapted<T>>::size_type;
	using value_type = typename AdapterTraits<CoordMapAdapted<T>>::value_type;
public:
	constexpr CoordMapAdapted() : container_{} {}
	constexpr CoordMapAdapted(std::initializer_list<value_type> init) : container_{ init } {}
	constexpr CoordMapAdapted(CoordMapAdapted&& container) noexcept :
		container_{ std::move(container.container_) } {}
	constexpr CoordMapAdapted(const CoordMapAdapted&) = default;
	constexpr CoordMapAdapted& operator=(const CoordMapAdapted&) = default;
	~CoordMapAdapted() override = default;
public:
	void add(const T& elem)
	{
		container_[elem.first].insert(elem.second);
	}

	void add(T&& elem)
	{
		container_[std::move(elem.first)].insert(std::move(elem.second));
	}

	const std::set<coordinate_t>& get(const typename T::first_type& word) const
	{
		return container_.at(word);
	}

	void reserve(const size_type size)
	{
		container_.reserve(size);
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
};

template<typename T>
struct AdapterTraits<CoordMapAdapted<T>>
{
	using first_type = typename T::first_type;
	using second_type = typename T::second_type;
	static_assert(std::is_same_v<second_type, coordinate_t>, "Cannot insert other types except for coordinate_t.");
	using container_type = std::map<first_type, std::set<coordinate_t>>;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = T;
};

#endif // !SINGULAR_MAP_
