#ifndef OPTIMIZED_DICTIONARY_STANDARD_FILLER_
#define OPTIMIZED_DICTIONARY_STANDARD_FILLER_
#include "AStandardFiller.h"

template<typename Dictionary>
	requires is_optimized_dict<Dictionary>
class OptimizedDictionaryStandardFiller : public AStandardFiller<Dictionary>
{
private:
	using base_type = AStandardFiller<Dictionary>;
public:
	using dict_type = Dictionary;
	using word_type = typename base_type::word_type;
	using reader_ptr = typename base_type::reader_ptr;

public:

	OptimizedDictionaryStandardFiller(const reader_ptr reader) :
		AStandardFiller<Dictionary>(reader) {}

private:

	bool add_word(dict_type& dict, const word_type&, const word_type& nxt) const override
	{
		if (nxt.empty()) return false;
		dict.add_word(nxt);
		return true;
	}
};

#endif // !OPTIMIZED_DICTIONARY_STANDARD_FILLER_
