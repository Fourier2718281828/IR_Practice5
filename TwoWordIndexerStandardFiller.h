#ifndef TWO_WORD_INDEX_STANDARD_FILLER
#define TWO_WORD_INDEX_STANDARD_FILLER
#include "AStandardFiller.h"
#include "TwoWordIndexer.h"

template<typename Dictionary>
	requires (is_two_word_indexer<Dictionary>)
class TwoWordIndexerStandardFiller : public AStandardFiller<Dictionary>
{
private:
	using base_type = AStandardFiller<Dictionary>;
public:
	using dict_type = Dictionary;
	using word_type = typename base_type::word_type;
	using reader_ptr = typename base_type::reader_ptr;

public:

	TwoWordIndexerStandardFiller(const reader_ptr reader) :
		AStandardFiller<Dictionary>(reader) {}

private:

	bool add_word(dict_type& dict, const word_type& prev, const word_type& nxt) const override
	{
		if (prev.empty() || nxt.empty()) return false;
		typename dict_type::word_type pair{ prev, nxt };
		dict.add_word(std::move(pair), base_type::get_filename());
		return true;
	}
};

#endif // !TWO_WORD_INDEX_STANDARD_FILLER
