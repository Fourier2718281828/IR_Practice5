#ifndef INCIDENCE_MATRIX_
#define INCIDENCE_MATRIX_
#include "Dictionary.h"
#include "DefaultAddPolicy.h"
#include "USetPolicy.h"
#include "PairedVectorAdapted.h"
#include "MapAdapted.h"
#include "BitVectorAdapted.h"
#include <stdexcept>
#include "Utilities.h"
#include "DocumentIndexer.h"
#include <bitset>


using IncidenceMatrix = DocumentIndexer
<
	std::string,
	std::string,
	MapAdapted,
	BitVectorAdapted,
	AnySetPolicy
>;

using matrix_query_result = typename IncidenceMatrix::dictlist_type<typename IncidenceMatrix::mapped_type>;

matrix_query_result bitwise_and(const matrix_query_result& v1, const matrix_query_result& v2)
{
	const size_t chunk_size = sizeof(unsigned long) * 8;
	const size_t num_chunks = (v1.size() + chunk_size - 1) / chunk_size;
	matrix_query_result result(v1.size());

	for (size_t i = 0; i < num_chunks; i++)
	{
		size_t start = i * chunk_size;
		size_t end = std::min((i + 1) * chunk_size, v1.size());

		std::bitset<chunk_size> b1;
		for (size_t j = start; j < end; j++)
		{
			b1[j - start] = v1[j];
		}
		std::bitset<chunk_size> b2;
		for (size_t j = start; j < end; j++)
		{
			b2[j - start] = v2[j];
		}

		std::bitset<chunk_size> result_chunk = b1 & b2;

		for (size_t j = start; j < end; j++)
		{
			result[j] = result_chunk[j - start];
		}
	}

	return result;
}


matrix_query_result and_query
(
	const IncidenceMatrix& indexer,
	const matrix_query_result& q1,
	const matrix_query_result& q2
)
{
	return bitwise_and(q1, q2);
}

matrix_query_result not_query
(
	const IncidenceMatrix& indexer,
	const matrix_query_result& q
)
{
	matrix_query_result res{};
	for (matrix_query_result::size_type i = 0u; i < indexer.get_docids().size(); ++i)
	{
		if (!q.contains(i))
			res.add(i);
	}
	return res;
}

matrix_query_result or_query
(
	const IncidenceMatrix& indexer,
	const matrix_query_result& q1,
	const matrix_query_result& q2
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



matrix_query_result and_query
(
	const IncidenceMatrix& indexer,
	const IncidenceMatrix::word_type& q1,
	const IncidenceMatrix::word_type& q2
)
{
	return and_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
}

matrix_query_result not_query
(
	const IncidenceMatrix& indexer,
	const IncidenceMatrix::word_type& q
)
{
	return not_query(indexer, indexer.get_list(q));
}

matrix_query_result or_query
(
	const IncidenceMatrix& indexer,
	const IncidenceMatrix::word_type& q1,
	const IncidenceMatrix::word_type& q2
)
{
	return or_query(indexer, indexer.get_list(q1), indexer.get_list(q2));
}

#endif // !INCIDENCE_MATRIX_
