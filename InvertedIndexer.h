#ifndef INVERTED_INDEXER_
#define INVERTED_INDEXER_
#include <string>
#include "DefaultAddPolicy.h"
#include "SetAdapted.h"
#include "PairedVectorAdapted.h"
#include "MapAdapted.h"
#include "USetPolicy.h"
#include "VectorAdapted.h"
#include "Utilities.h"
#include "DocumentIndexer.h"

using InvertedIndexer = DocumentIndexer
<
	std::string,
	std::string,
	MapAdapted,
	SetAdapted,
	AnySetPolicy
>;

using query_result = typename InvertedIndexer::dictlist_type<typename InvertedIndexer::mapped_type>;

query_result and_query
(
	const InvertedIndexer& indexer,
	const query_result& q1,
	const query_result& q2
)
{
	return intersect(q1, q2);
}

query_result not_query
(
	const InvertedIndexer& indexer,
	const query_result& q
)
{
	query_result res{};
	for (query_result::size_type i = 0u; i < indexer.get_docids().size(); ++i)
	{
		if(!q.contains(i))
			res.add(i);
	}
	return res;
}

query_result or_query
(
	const InvertedIndexer& indexer,
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
	const InvertedIndexer& indexer,
	const InvertedIndexer::word_type& q1,
	const InvertedIndexer::word_type& q2
)
{
	return and_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
}

query_result not_query
(
	const InvertedIndexer& indexer,
	const InvertedIndexer::word_type& q
)
{
	return not_query(indexer, indexer.get_list(q));
}

query_result or_query
(
	const InvertedIndexer& indexer,
	const InvertedIndexer::word_type& q1,
	const InvertedIndexer::word_type& q2
)
{
	return or_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
}

#endif // !INVERTED_INDEXER_
