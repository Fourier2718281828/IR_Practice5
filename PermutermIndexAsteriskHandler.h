#ifndef PERMUTERM_INDEX_ASTERISK_HANDLER_
#define PERMUTERM_INDEX_ASTERISK_HANDLER_
#include "IAsteriskQueryHandler.h"
#include "Trie.h"

class PermutermIndexAsteriskHandler : public IAsteriskQueryHandler
{
public:
	using base_type = IAsteriskQueryHandler;
	using word_type = typename base_type::word_type;
	using char_type = typename word_type::value_type;
	using trie_type = Trie<char_type>;
	using iterable = typename base_type::iterable;
public:

	constexpr PermutermIndexAsteriskHandler
	(
		const trie_type& perm_trie
	) noexcept :
		perm_trie_(perm_trie)
	{

	}

	~PermutermIndexAsteriskHandler() override = default;

private:

	iterable search_with_tokens(const Iterable<word_type>& tokens) const override
	{
		if (tokens.size() == 1)
		{
			return perm_trie_.has_word(tokens[0])
				? iterable{ tokens[0] }
				: iterable{};
		}
		if (tokens.size() > 1)
		{
			iterable res{};
			const auto word_from_permutation = [](word_type word) -> word_type
			{
				while (word[0] != '$')
				{
					word.push_back(word[0]);
					word.erase(0, 1);
				}
				word.erase(0, 1);
				return word;
			};
			const auto first = tokens[0];
			auto last = tokens[tokens.size() - 1];

			last.push_back('$');
			last += first;
			iterable perm_words = handle_closed_asterix(last);
			std::transform
			(
				perm_words.begin(),
				perm_words.end(),
				std::inserter(res, res.end()),
				word_from_permutation
			);

			return res;
		}
		return {};
	}

	iterable handle_closed_asterix(const word_type& prefix) const noexcept
	{
		const auto& trie = perm_trie_.get_container();
		return { trie.begin(prefix), trie.end() };
	}

	bool will_be_postfiltrated_if_tokens
	(
		const typename Iterable<word_type>::size_type token_count
	) const noexcept override
	{
		return token_count > 2;
	}

private:
	const trie_type& perm_trie_;
};


#endif // !PERMUTERM_INDEX_ASTERISK_HANDLER_
