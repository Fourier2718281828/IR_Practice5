#ifndef KGRAM_INDEXER_
#define KGRAM_INDEXER_
#include "Dictionary.h"
#include "SetAdapted.h"
#include "USetPolicy.h"
#include "MapAdapted.h"

template<typename WordType, size_t K>
using KGramIndexer = UnoptimizedDict
<
	WordType,
	WordType,
	AnySetPolicy,
	MapAdapted,
	SetAdapted
>;

//template<typename WordType, size_t K>
//using query_result = typename KGramIndexer<WordType, K>::dictlist_type<typename KGramIndexer<WordType, K>::mapped_type>;
//
//template<typename WordType, size_t K>
//query_result<WordType, K> and_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const query_result<WordType, K>& q1,
//	const query_result<WordType, K>& q2
//)
//{
//	return intersect(q1, q2);
//}
//
//template<typename WordType, size_t K>
//query_result<WordType, K> not_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const query_result<WordType, K>& q
//)
//{
//	query_result res{};
//	for (typename query_result<WordType, K>::size_type i = 0u; i < indexer.get_docids().size(); ++i)
//	{
//		if (!q.contains(i))
//			res.add(i);
//	}
//	return res;
//}
//
//template<typename WordType, size_t K>
//query_result<WordType, K> or_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const query_result<WordType, K>& q1,
//	const query_result<WordType, K>& q2
//)
//{
//	return not_query
//	(
//		indexer,
//		and_query
//		(
//			indexer,
//			not_query(indexer, q1),
//			not_query(indexer, q2)
//		)
//	);
//}
//
//
//
//template<typename WordType, size_t K>
//query_result<WordType, K> and_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const typename KGramIndexer<WordType, K>::word_type& q1,
//	const typename KGramIndexer<WordType, K>::word_type& q2
//)
//{
//	return and_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
//}
//
//template<typename WordType, size_t K>
//query_result<WordType, K> not_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const typename KGramIndexer<WordType, K>::word_type& q
//)
//{
//	return not_query(indexer, indexer.get_list(q));
//}
//
//template<typename WordType, size_t K>
//query_result<WordType, K> or_query
//(
//	const KGramIndexer<WordType, K>& indexer,
//	const typename KGramIndexer<WordType, K>::word_type& q1,
//	const typename KGramIndexer<WordType, K>::word_type& q2
//)
//{
//	return or_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
//}


#endif // !KGRAM_INDEXER_
