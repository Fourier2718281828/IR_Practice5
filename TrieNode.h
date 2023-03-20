#ifndef NODE_
#define NODE_
#include <memory>
#include "Utilities.h"

template
<
	typename T,
	template<typename, typename> typename MapType
>
class TrieNode
{
public:
	using value_type = T;
	using char_type = typename value_type::value_type;
	using node_ptr = TrieNode*;
	using container = MapType<char_type, node_ptr>;
public:

	constexpr TrieNode() noexcept :
		children_{},
		is_endof_word_(false)
	{}

	constexpr TrieNode(const TrieNode&) = delete;

	TrieNode& operator= (const TrieNode&) = delete;

	~TrieNode()
	{
		for (auto&& [_, ptr] : children_)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

public:

	void add_child(const char_type val, node_ptr child = new TrieNode())
	{
		children_.add(val, child);
	}

	const container& children() const noexcept
	{
		return children_;
	}

	bool& is_endof_word() noexcept
	{
		return is_endof_word_;
	}

	bool is_endof_word() const noexcept
	{
		return is_endof_word_;
	}

	node_ptr operator[] (const char_type val)
	{
		return children_[val];
	}

private:
	container children_;
	bool is_endof_word_;
};

#endif // !NODE_
