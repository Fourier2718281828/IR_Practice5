#ifndef VECTOR_ADAPTED_
#define VECTOR_ADAPTED_
#include "SingularAdapters.h"
#include <vector>
//
//ADDAPT_APPANDABLE_CONTAINER(VectorAdapted, std::vector, push_back)

template<typename T>
class VectorAdapted : public SingularContainerAdapter<T, VectorAdapted>
{
public:
	using container_type = typename AdapterTraits<VectorAdapted<T>>::container_type;
	using const_iterator = container_type::const_iterator;
	using iterator = container_type::iterator;
	using size_type = container_type::size_type;
	using value_type = container_type::value_type;
public:
	constexpr VectorAdapted() : container_{} {}
	constexpr VectorAdapted(std::initializer_list<value_type> init) : container_{ init } {}
	constexpr VectorAdapted(VectorAdapted&& container) noexcept :
		container_{ std::move(container.container_) } {}
	~VectorAdapted() override = default;
public:
	void add(const T& elem)
	{
		container_.push_back(elem);
	}
	void add(T&& elem)
	{
		container_.push_back(std::move(elem));
	}

	template<typename T>
	constexpr iterator insert(const_iterator pos, T&& value)
	{
		return container_.insert(pos, std::forward<T>(value));
	}

	void reserve(const size_type size)
	{
		container_.reserve(size);
	}

	constexpr bool contains(const T& elem) const noexcept;

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
struct AdapterTraits<VectorAdapted<T>>
{
	using container_type = std::vector<T>;
	using const_iterator = typename container_type::const_iterator;
	using iterator = typename container_type::iterator;
	using size_type = typename container_type::size_type;
	using value_type = typename container_type::value_type;
};

template<typename T>
inline constexpr bool VectorAdapted<T>::contains(const T& elem) const noexcept
{
	return std::find_if
	(
		begin(),
		end(),
		[&elem](const auto& pair) { return pair == elem; }
	) != end();
}

#endif // !VECTOR_ADAPTED_
