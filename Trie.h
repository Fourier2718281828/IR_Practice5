#ifndef TRIE_H
#define TRIE_H
#include "Dictionary.h"
#include "USetPolicy.h"
#include "TrieContainer.h"
#include <string>

template<typename CharType>
using Trie = OptimizedDitctionary
<
	std::basic_string<CharType>,
	AnySetPolicy,
	TrieContainer
>;

#endif // !TRIE_H
