#ifndef K_GRAM_ASTERISK_HANDLER_
#define K_GRAM_ASTERISK_HANDLER_
#include "IAsteriskQueryHandler.h"
#include "KGramIndexer.h"

class KGramAsteriskQueryHandler : public IAsteriskQueryHandler
{
public:
	using base_type = IAsteriskQueryHandler;
	using word_type = typename base_type::word_type;
	using char_type = typename word_type::value_type;
	template<size_t K>
	using indexer_type = KGramIndexer<word_type, K>;
	using iterable = typename base_type::iterable;
public:

	constexpr KGramAsteriskQueryHandler
	(
		const indexer_type<2>& bigram,
		const indexer_type<3>& threegram
	) noexcept :
		bigram_(bigram),
		threegram_(threegram)
	{

	}

	~KGramAsteriskQueryHandler() override = default;

private:

	iterable search_with_tokens(const Iterable<word_type>& tokens) const override
	{
		using size_type = typename Iterable<word_type>::size_type;
		
		size_t K;
		auto grams = split_tokens_to_kgrams(tokens, K);
		
		switch (K)
		{
		case 3:
			return search_using_kgrams<3>(grams, threegram_);
		case 2:
			return search_using_kgrams<2>(grams, bigram_);
		default:
			return {};
		}
		
	}

	template<size_t K>
	iterable search_using_kgrams(Iterable<word_type>& grams, const indexer_type<K>& indexer) const
	{
		using size_type = typename Iterable<word_type>::size_type;

		while (!grams.empty() && !indexer.get_container().contains(grams.back()))
		{
			grams.pop_back();
		}
		if (grams.empty())
		{
			return {};
		}

		const auto& lst = indexer.operator[](grams.back());
		iterable res{ lst.begin(), lst.end() };
		grams.pop_back();
		for (size_type i = 0; i < grams.size(); ++i)
		{
			if (indexer.get_container().contains(grams[i]))
				res = intersect_special(res, indexer[grams[i]]);
		}

		return res;
	}

	Iterable<word_type> split_tokens_to_kgrams(const Iterable<word_type>& tokens, size_t& K) const noexcept
	{
		auto res = do_split_tokens_to_kgrams(tokens, 3);
		K = 3;
		if (res.empty())
		{
			res = do_split_tokens_to_kgrams(tokens, 2);
			K = 2;
		}
		return res;
	}

	Iterable<word_type> do_split_tokens_to_kgrams(const Iterable<word_type>& tokens, const size_t K) const noexcept
	{
		using size_type = Iterable<word_type>::size_type;
		Iterable<word_type> res{};
		for (size_type i = 0u; i < tokens.size(); ++i)
		{
			word_type token = tokens[i];

			if (token.empty()) continue;
			if (i == 0u)
				token = '$' + token;
			else if (i == tokens.size() - 1)
				token += '$';
			
			auto kgrams = bare_kgrams_of_word(token, K);
			for (auto&& kgram : kgrams)
			{
				res.push_back(kgram);
			}
		}
		return res;
	}


	template<size_t K>
	const indexer_type<K>& get_indexer()
	{
		if constexpr (K == 3) return threegram_;
		if constexpr (K == 2) return bigram_;
		throw std::runtime_error("Invalid K value.");
	}
	
	bool will_be_postfiltrated_if_tokens
	(
		const typename Iterable<word_type>::size_type token_count
	) const noexcept override
	{
		return true;
	}

private:
	const indexer_type<2>& bigram_;
	const indexer_type<3>& threegram_;

};


#endif // !K_GRAM_ASTERISK_HANDLER_
