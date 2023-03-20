#ifndef TRIE_ASTERISC_HANDLER_
#define TRIE_ASTERISC_HANDLER_
#include "IQueryHandler.h"
#include "Trie.h"
#include "Utilities.h"
#include "IAsteriskQueryHandler.h"
#include <string>


class TrieAsteriscHandler : public IAsteriskQueryHandler
{
public:
	using base_type = IAsteriskQueryHandler;
	using word_type = typename base_type::word_type;
	using char_type = typename word_type::value_type;
	using trie_type = Trie<char_type>;
	using iterable  = typename base_type::iterable;
public:

	constexpr TrieAsteriscHandler
	(
		const trie_type& original_trie,
		const trie_type& reversed_trie
	) noexcept :
		original_trie_(original_trie),
		reversed_trie_(reversed_trie)
	{

	}

private:

	iterable search_with_tokens(const Iterable<word_type>& tokens) const override
	{
		if (tokens.size() == 1)
		{
			return original_trie_.has_word(tokens[0])
				? iterable{ tokens[0] }
				: iterable{};
		}
		if(tokens.size() > 1)
		{
			iterable first_processed;
			iterable second_processed;
			auto first = tokens[0];
			auto last = tokens[tokens.size() - 1];

			if (!first.empty())
				first_processed = handle_closed_asterix(first);
			if (!last.empty())
				second_processed = handle_opened_asterix(last);
			
			if (!first.empty() && !last.empty())
				return intersect_set<word_type, std::set>(first_processed, second_processed);
			if (!first.empty())
				return first_processed;
			return second_processed;
		}
		return {};
	}


	iterable handle_closed_asterix(const word_type& prefix) const noexcept
	{
		const auto& trie = original_trie_.get_container();
		return { trie.begin(prefix), trie.end() };
	}

	iterable handle_opened_asterix(const word_type& suffix0) const noexcept
	{
		iterable res;
		const auto& trie = reversed_trie_.get_container();
		const auto reverter = [](word_type w) -> const word_type
		{ 
			std::reverse(w.begin(), w.end());
			return std::move(w);
		};
		const word_type suffix = reverter(suffix0);
		std::transform
		(
			trie.begin(suffix),
			trie.end(),
			std::inserter(res, res.end()),
			reverter
		);

		return res;
	}

	bool will_be_postfiltrated_if_tokens
	(
		const typename Iterable<word_type>::size_type token_count
	) const noexcept override
	{
		return token_count > 2;
	}

private:
	const trie_type& original_trie_;
	const trie_type& reversed_trie_;
};


#endif // !TRIE_ASTERISC_HANDLER_
