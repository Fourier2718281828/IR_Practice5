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

#endif // !TWO_WORD_INVERTED_INDEXER_
