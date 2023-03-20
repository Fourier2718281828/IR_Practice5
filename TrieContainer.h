#ifndef TRIE_CONTAINER_
#define TRIE_CONTAINER_
#include <unordered_map>
#include "SingularAdapters.h"
#include "TrieIterator.h"
#include "TrieNode.h"
#include "Utilities.h"
#include "UMapAdapter.h"

#include <iostream>

template<typename T>
class TrieContainer : public SingularContainerAdapter<T, TrieContainer>
{
public:
	using const_iterator = typename AdapterTraits<TrieContainer<T>>::const_iterator;
	using iterator = typename AdapterTraits<TrieContainer<T>>::iterator;
	using size_type = typename AdapterTraits<TrieContainer<T>>::size_type;
	using value_type = typename AdapterTraits<TrieContainer<T>>::value_type;
public:
	using node_type = typename AdapterTraits<TrieContainer<T>>::node_type;
	using node_ptr = node_type*;
public:
	constexpr TrieContainer() : root_{ new node_type() }, size_{ 0u } {}
	constexpr TrieContainer(std::initializer_list<value_type> init) : TrieContainer() 
	{
		for (auto&& str : init)
		{
			add(str);
		}
	}
	constexpr TrieContainer(TrieContainer&& container) noexcept :
		root_{ std::move(container.root_) }, size_{ container.size_ } 
	{
		container.root_ = nullptr;
		container.size_ = 0u;
	}

	~TrieContainer() override
	{
		delete root_;
		root_ = nullptr;
	}

public:

	void add(const T& str)
	{
		if (str.empty()) std::cout << "EMPTY STRING!\n";
		node_type* curr = root_;
		for (const auto ch : str)
		{
			if (!curr->children().contains(ch))
			{
				curr->add_child(ch);
			}
			curr = (*curr)[ch];
		}
		if (!curr->is_endof_word())
		{
			++size_;
		}
		curr->is_endof_word() = true;
	}

	constexpr bool contains(const T& str) const noexcept
	{
		auto curr = find_prefix_node(str);
		return curr && curr->is_endof_word();
	}

	[[nodiscard]] constexpr iterator begin() noexcept
	{
		return iterator(root_);
	}

	[[nodiscard]] constexpr iterator end() noexcept
	{
		return iterator(nullptr);
	}

	[[nodiscard]] constexpr const_iterator begin() const noexcept
	{
		return const_iterator(root_);
	}

	[[nodiscard]] constexpr const_iterator end() const noexcept
	{
		return const_iterator(nullptr);
	}

	[[nodiscard]] constexpr const_iterator cbegin() noexcept
	{
		return const_iterator(root_);
	}

	[[nodiscard]] constexpr const_iterator cend() noexcept
	{
		return const_iterator(nullptr);
	}

	[[nodiscard]] constexpr iterator begin(const value_type& prefix) noexcept
	{
		node_ptr ptr = find_prefix_node(prefix);
		return iterator(ptr, prefix);
	}

	[[nodiscard]] constexpr const_iterator begin(const value_type& prefix) const noexcept
	{
		node_ptr ptr = find_prefix_node(prefix);
		return const_iterator(ptr, prefix);
	}

	constexpr size_type size() const noexcept
	{
		return size_;
	}

private:
	
	node_ptr find_prefix_node(const value_type& prefix) const 
	{
		node_type* curr = root_;
		for (const auto ch : prefix)
		{
			if (!curr->children().contains(ch))
			{
				return nullptr;
			}
			curr = (*curr)[ch];
		}
		return curr;
	}

private:
	node_ptr  root_;
	size_type size_;
};

template<typename T>
struct AdapterTraits<TrieContainer<T>>
{
	using node_type = TrieNode<T, UMapAdapted>;
	using const_iterator = ConstTrieIterator<TrieContainer<T>>;
	using iterator = TrieIterator<TrieContainer<T>>;
	using size_type = std::size_t;
	using value_type = T;
};

#endif // !TRIE_CONTAINER_
