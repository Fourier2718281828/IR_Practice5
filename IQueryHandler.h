#ifndef I_QUERY_HANDLER_
#define I_QUERY_HANDLER_
#include <set>

template<typename WordType>
class IQueryHandler
{
public:
	using word_type = WordType;
	using iterable = std::set<word_type>;
public:
	virtual ~IQueryHandler() = default;

public:
	iterable handle_query(const word_type& query) const
	{
		if (!validate(query))
			throw std::runtime_error("Invalid query format.");
		return do_handle_query(query);
	}

	virtual bool validate(const word_type&) const noexcept = 0;

private:
	virtual iterable do_handle_query(const word_type&) const = 0;
};

#endif // !I_QUERY_HANDLER_
