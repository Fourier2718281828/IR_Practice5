#ifndef KGRAM_INDEX_STANDARD_FILLER_
#define KGRAM_INDEX_STANDARD_FILLER_
#include "AStandardFiller.h"

template<typename Dictionary, size_t K>
class KGramIndexStandardFiller : public AStandardFiller<Dictionary>
{
private:
	using base_type = AStandardFiller<Dictionary>;
public:
	using dict_type = Dictionary;
	using word_type = typename base_type::word_type;
	using reader_ptr = typename base_type::reader_ptr;

public:

	constexpr KGramIndexStandardFiller(const reader_ptr reader) :
		AStandardFiller<Dictionary>(reader) {}

	~KGramIndexStandardFiller() override = default;

private:

	bool add_word(dict_type& dict, const word_type&, const word_type& nxt) const override
	{
		if (nxt.empty()) return false;
		for (auto&& permutation : kgrams_of_word(nxt, K))
		{
			dict.add_word(permutation, nxt);
		}
		return true;
	}
};

#endif // !KGRAM_INDEX_STANDARD_FILLER_
