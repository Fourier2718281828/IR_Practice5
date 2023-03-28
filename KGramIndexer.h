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

#endif // !KGRAM_INDEXER_
