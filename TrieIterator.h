#ifndef TRIE_ITERATOR_
#define TRIE_ITERATOR_
#include "Utilities.h"
#include <iterator>
#include <stack>
#include <cassert>

template<typename Trie>
class ConstTrieIterator
{
public:
	using trie = Trie;
	using base_val_type = typename trie::value_type;
	using projection_type = const base_val_type;
public:
	using node_type = typename trie::node_type;
	using node_ptr = typename trie::node_ptr;
private:
	using char_type = typename base_val_type::value_type;
	using stacked = std::pair<base_val_type, node_ptr>;
	using container = std::stack<stacked>;
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = base_val_type;
	using difference_type = std::size_t;
	using pointer = projection_type*;
	using reference = projection_type;
public:
	constexpr ConstTrieIterator(const node_ptr node, const base_val_type& prefix = "") :
		container_ {},
		cword_{},
		prefix_{ prefix },
		ended_(false)
	{
		if (node)
		{
			container_.push({ "", node });
			next_word();
		}
		else
		{
			ended_ = true;
		}
	}

	reference operator*() const
	{
		if (is_end())
			throw std::runtime_error("Reading on the ended iterator.");
		return prefix_ + cword_;
	}

	ConstTrieIterator& operator++()
	{
		if (is_end())
			throw std::runtime_error("Incrementing the ended iterator.");
		next_word();
		return *this;
	}

	ConstTrieIterator operator++(int)
	{
		ConstTrieIterator tmp = *this;
		++(*this);
		return tmp;
	}

	bool operator==(const ConstTrieIterator& other) const
	{
		return ended_ == other.ended_ 
			&& container_ == other.container_;
	}

	bool operator!=(const ConstTrieIterator& other) const
	{
		return !(*this == other);
	}

	bool is_end() const noexcept
	{
		return ended_;
	}

private:

	void next_word()
	{
		if (container_.empty())
		{
			stop();
			return;
		}

		node_ptr current_node;

		do
		{
			stacked nxt_stacked = container_.top();
			set_buffer(nxt_stacked.first);
			current_node = nxt_stacked.second;
			container_.pop();

			for (const auto& [ch, child] : current_node->children())
			{
				container_.push({ next_buffer_val(ch), child});
			}

		} while (!word_found(current_node) && !container_.empty());
	}

	const base_val_type next_buffer_val(const char_type to_append) const noexcept
	{
		return cword_ + to_append;
	}

	bool word_found(node_ptr node) const noexcept
	{
		return node->is_endof_word();
	}

	void clear_buffer()
	{
		cword_.clear();
	}

	void stop() noexcept
	{
		ended_ = true;
	}

	void set_buffer(const base_val_type& next)
	{
		cword_ = next;
	}

private:
	container container_;
	base_val_type cword_;
	const base_val_type prefix_;
	bool		  ended_;
};

template<typename Trie>
using TrieIterator = ConstTrieIterator<Trie>;

#endif // !TRIE_ITERATOR_
