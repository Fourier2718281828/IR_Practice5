#ifndef ASTERISC_QUERY_HANDLER_
#define ASTERISC_QUERY_HANDLER_
#include "IQueryHandler.h"
#include "Utilities.h"
#include <string>
#include <regex>

class IAsteriskQueryHandler : public IQueryHandler<std::string>
{
public:
	using base_type = IQueryHandler<std::string>;
	using word_type = typename base_type::word_type;
	using iterable = typename base_type::iterable;
public:

	~IAsteriskQueryHandler() override = default;

	bool validate(const word_type& word) const noexcept override
	{
		return std::regex_match(word, s_QUERY_REGEX);
	}

private:

	iterable do_handle_query(const word_type& query) const override
	{
		Iterable<word_type> tokens = tokenize('$' + query + '$', std::string("*"));
		tokens[0].erase(0, 1);
		if (tokens.size() == 1)
			tokens[0].pop_back();
		else
			tokens[tokens.size() - 1].pop_back();
		auto res =  search_with_tokens(tokens);
		return  will_be_postfiltrated_if_tokens(tokens.size())
			? post_filtration(res, regex_from_asterisk_query(query))
			: res;
	}

	iterable post_filtration(const iterable& words, std::regex regex) const noexcept
	{
		iterable res{};
		for (auto&& word : words)
		{
			if (std::regex_match(word, regex))
			{
				res.insert(word);
			}
		}
		return res;
	}

	std::regex regex_from_asterisk_query(const word_type& query) const noexcept
	{
		word_type str;
		for (word_type::value_type ch : query) 
		{
			if (ch == '*') 
				str += ".*";
			else 
				str += ch;
		}

		return std::regex(str);
	}

	virtual iterable search_with_tokens(const Iterable<word_type>& tokens) const = 0;

	virtual bool will_be_postfiltrated_if_tokens
	(
		const typename Iterable<word_type>::size_type
	) const noexcept = 0;

private:
	static const std::regex s_QUERY_REGEX;
};

const std::regex IAsteriskQueryHandler::s_QUERY_REGEX("(\\*)?[^\\*]+(\\*[^\\*]+)*(\\*)?");

#endif // !ASTERISC_QUERY_HANDLER_
