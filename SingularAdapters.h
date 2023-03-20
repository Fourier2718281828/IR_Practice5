#ifndef SINGULAR_ADAPTERS_
#define SINGULAR_ADAPTERS_

template<typename Derived>
struct AdapterTraits;

template
<
	typename T,
	template<typename> typename Derived
>
class SingularContainerAdapter
{
public:
	using const_iterator = typename AdapterTraits<Derived<T>>::const_iterator;
	using iterator = typename AdapterTraits <Derived<T>>::iterator;
	using size_type = typename AdapterTraits <Derived<T>>::size_type;
	using value_type = typename AdapterTraits <Derived<T>>::value_type;
public:
	virtual ~SingularContainerAdapter() = default;
protected:
	constexpr SingularContainerAdapter() = default;
public:
	void add(const T& elem)
	{
		get_this().add(elem);
	}

	void add(T&& elem)
	{
		get_this().add(std::move(elem));
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
	Derived<T>& get_this() noexcept
	{
		return static_cast<Derived<T>&>(*this);
	}

	const Derived<T>& get_this() const noexcept
	{
		return static_cast<const Derived<T>&>(*this);
	}
};

#include <initializer_list>
#define ADDAPT_APPANDABLE_CONTAINER(_Name, _Container, _Method_name)					\
template<typename T>																	\
class _Name : public SingularContainerAdapter<T, _Name>									\
{																						\
public:																					\
	using container_type = typename AdapterTraits<_Name<T>>::container_type;			\
	using const_iterator = container_type::const_iterator;								\
	using iterator = container_type::iterator;											\
	using size_type = container_type::size_type;										\
	using value_type = container_type::value_type;										\
public:																					\
	constexpr _Name() : container_{} {}													\
	constexpr _Name(std::initializer_list<value_type> init) : container_{init} {}		\
	constexpr _Name(_Name&& container) noexcept :										\
		container_{ std::move(container.container_) } {}								\
	~_Name() override = default;														\
public:																					\
	void add(const T& elem)																\
	{																					\
		container_._Method_name(elem);													\
	}																					\
	void add(T&& elem)																	\
	{																					\
		container_._Method_name(std::move(elem));										\
	}																					\
																						\
	void reserve(const size_type size)													\
	{																					\
		container_.reserve(size);														\
	}																					\
																						\
	constexpr bool contains(const T& elem) const noexcept;								\
																						\
	[[nodiscard]] constexpr iterator begin() noexcept									\
	{																					\
		return container_.begin();														\
	}																					\
																						\
	[[nodiscard]] constexpr iterator end() noexcept										\
	{																					\
		return container_.end();														\
	}																					\
																						\
	[[nodiscard]] constexpr const_iterator begin() const noexcept						\
	{																					\
		return container_.begin();														\
	}																					\
																						\
	[[nodiscard]] constexpr const_iterator end() const noexcept							\
	{																					\
		return container_.end();														\
	}																					\
																						\
	[[nodiscard]] constexpr const_iterator cbegin() noexcept							\
	{																					\
		return container_.cbegin();														\
	}																					\
																						\
	[[nodiscard]] constexpr const_iterator cend() noexcept								\
	{																					\
		return container_.cend();														\
	}																					\
																						\
	constexpr size_type size() const noexcept											\
	{																					\
		return container_.size();														\
	}																					\
private:																				\
	container_type container_;															\
};																						\
																						\
template<typename T>																	\
struct AdapterTraits<_Name<T>>															\
{																						\
	using container_type = _Container<T>;												\
	using const_iterator = typename container_type::const_iterator;						\
	using iterator = typename container_type::iterator;									\
	using size_type = typename container_type::size_type;								\
	using value_type = typename container_type::value_type;								\
};

#endif // !SINGULAR_ADAPTERS_
