#ifndef APPEND_POLICY_
#define APPEND_POLICY_

struct ArrayPolicy
{
public:

	template<typename container_type, typename Word>
	static void add_word(container_type& container, Word&& word)
	{
		if (!has_word(container, word))
		{
			container.add(std::forward<Word>(word));
		}
	}

	template<typename container_type, typename Word, typename ListNode>
	static void add_word(container_type& container, Word&& word, ListNode&& list_node)
	{
		container[(std::forward<Word>(word))].add(std::forward<ListNode>(list_node));
	}

	template<typename container_type, typename Word>
	static bool has_word(const container_type& container, const Word& word) noexcept
	{
		return container.contains(word);
	}

};

#endif // !APPEND_POLICY_
