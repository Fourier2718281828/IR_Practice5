#ifndef TWO_WORD_INVERTED_INDEXER_
#define TWO_WORD_INVERTED_INDEXER_
#include "DocumentIndexer.h"
#include "MapAdapted.h"
#include "SetAdapted.h"
#include "USetPolicy.h"
#include <utility>

using TwoWordInvertedIndexer = DocumentIndexer
<
	std::pair<std::string, std::string>,
	std::string,
	MapAdapted,
	SetAdapted,
	AnySetPolicy
>;

template<typename Indexer>
concept is_two_word_indexer = std::is_constructible_v<typename Indexer::word_type, std::string, std::string>;


using query_result = typename TwoWordInvertedIndexer::dictlist_type<typename TwoWordInvertedIndexer::mapped_type>;

query_result and_query
(
	const TwoWordInvertedIndexer& indexer,
	const query_result& q1,
	const query_result& q2
)
{
	return intersect(q1, q2);
}

query_result not_query
(
	const TwoWordInvertedIndexer& indexer,
	const query_result& q
)
{
	query_result res{};
	for (query_result::size_type i = 0u; i < indexer.get_docids().size(); ++i)
	{
		if (!q.contains(i))
			res.add(i);
	}
	return res;
}

query_result or_query
(
	const TwoWordInvertedIndexer& indexer,
	const query_result& q1,
	const query_result& q2
)
{
	return not_query
	(
		indexer,
		and_query
		(
			indexer,
			not_query(indexer, q1),
			not_query(indexer, q2)
		)
	);
}



query_result and_query
(
	const TwoWordInvertedIndexer& indexer,
	const TwoWordInvertedIndexer::word_type::first_type& q1,
	const TwoWordInvertedIndexer::word_type::first_type& q2
)
{
	if (!indexer.get_dict().get_container().contains({ q1, q2 })) return {};
	return indexer.get_list({ q1, q2 });
}

query_result not_query
(
	const TwoWordInvertedIndexer& indexer,
	const TwoWordInvertedIndexer::word_type& q
)
{
	return not_query(indexer, indexer.get_list(q));
}

query_result or_query
(
	const TwoWordInvertedIndexer& indexer,
	const TwoWordInvertedIndexer::word_type& q1,
	const TwoWordInvertedIndexer::word_type& q2
)
{
	return or_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
}

#endif // !TWO_WORD_INVERTED_INDEXER_
