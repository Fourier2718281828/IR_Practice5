#ifndef DICTIONARY_
#define DICTIONARY_
#include <type_traits>
#include <memory>
#include "Utilities.h"
#include "DichotomicAdapters.h"
#include "SingularAdapters.h"


template
<
	typename WordType,
	typename ListNodeType,
	typename AddElementPolicy,
	template<class, class> typename DichotomicAdaptedContainer,
	template<class> typename List,
	template<class> typename DefaultSingularAdaptedContainer = NullType1
>
class Dictionary
{
public:
	using word_type = WordType;
	using listnode_type = ListNodeType;
	using list_type = List<ListNodeType>;
private:
	constexpr static const bool s_IS_OPTIMIZED = std::is_same_v
		< 
			list_type, 
			NullType1<listnode_type>
		>;
public:
	using container_type = std::conditional_t
	<
		s_IS_OPTIMIZED,
		DefaultSingularAdaptedContainer<word_type>,
		DichotomicAdaptedContainer<word_type, list_type>
	>;
public:
	using value_type = typename container_type::value_type;
	using policy_type = AddElementPolicy;

public:

	constexpr Dictionary() noexcept :
		container_{}
	{}

	constexpr Dictionary(container_type&& container) noexcept :
		container_(std::move(container))
	{}

	Dictionary(const Dictionary&) = delete;

	Dictionary& operator=(const Dictionary&) = delete;

	Dictionary(Dictionary&&) noexcept = default;

	Dictionary& operator=(Dictionary&&) noexcept = default;

public:

	constexpr const container_type& get_container() const noexcept
	{
		return container_;
	}

	constexpr static bool is_optimized() noexcept
	{
		return s_IS_OPTIMIZED;
	}

	template
	<
		typename = std::enable_if_t
		< 
			is_optimized()
		>
	>
	void add_word(const word_type& word)
	{
		policy_type::add_word(container_, word);
	}

	template
	<
		typename = std::enable_if_t
		< 
			is_optimized()
		>
	>
	void add_word(word_type&& word)
	{
		policy_type::add_word(container_, std::move(word));
	}

	template
	<
		typename WT, 
		typename LN, 
		typename = std::enable_if_t
		<
			!is_optimized()
		>
	>
	void add_word(WT&& word, LN&& list_node)
	{
		policy_type::add_word(container_, std::forward<WT>(word), std::forward<LN>(list_node));
	}

	typename container_type::size_type size() const noexcept
	{
		return container_.size();
	}

	template
	<
		typename = std::enable_if_t
		<
			!is_optimized()
		>
	>
	[[nodiscard]] const list_type& operator[] (const word_type& word) const
	{
		try
		{
			return container_[word];
		}
		catch (...)
		{
			throw;
		}
	}

	[[nodiscard]] bool has_word(const word_type& word) const noexcept
	{
		return policy_type::has_word(container_, word);
	}

	[[nodiscard]] constexpr container_type::iterator begin() noexcept
	{
		return container_.begin();
	}

	[[nodiscard]] constexpr container_type::iterator end() noexcept
	{
		return container_.end();
	}

	[[nodiscard]] constexpr container_type::const_iterator begin() const noexcept
	{
		return container_.begin();
	}

	[[nodiscard]] constexpr container_type::const_iterator end() const noexcept
	{
		return container_.end();
	}

	[[nodiscard]] constexpr container_type::const_iterator cbegin() noexcept
	{
		return container_.cbegin();
	}

	[[nodiscard]] constexpr container_type::const_iterator cend() noexcept
	{
		return container_.cend();
	}

	void reserve(const typename container_type::size_type size)
	{
		container_.reserve(size);
	}

private:
	container_type container_;
}; 

template
<
	typename WordType,
	typename AddElementPolicy,
	template<class> typename SingularAdaptedContainer
>
using OptimizedDitctionary = Dictionary
<
	WordType,
	NullType0,
	AddElementPolicy,
	NullType2,
	NullType1,
	SingularAdaptedContainer
>;

template
<
	typename WordType,
	typename ListNodeType,
	typename AddElementPolicy,
	template<class, class> typename DichotomicAdaptedContainer,
	template<class> typename dictlist_type
>
using UnoptimizedDict = Dictionary
<
	WordType,
	ListNodeType,
	AddElementPolicy,
	DichotomicAdaptedContainer,
	dictlist_type,
	NullType1
>;

template<typename Dictionary>
concept is_optimized_dict = Dictionary::is_optimized();

#endif // !DICTIONARY_
