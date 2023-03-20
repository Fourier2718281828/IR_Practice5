#ifndef REVERSED_INDEX_STANDARD_FILLER_
#define REVERSED_INDEX_STANDARD_FILLER_
#include "AStandardFiller.h"

template<typename Dictionary>
class PermutermIndexStandardFiller : public AStandardFiller<Dictionary>
{
private:
	using base_type = AStandardFiller<Dictionary>;
public:
	using dict_type = Dictionary;
	using word_type = typename base_type::word_type;
	using reader_ptr = typename base_type::reader_ptr;

public:

	constexpr PermutermIndexStandardFiller(const reader_ptr reader) :
		AStandardFiller<Dictionary>(reader) {}

	~PermutermIndexStandardFiller() override = default;

private:

	bool add_word(dict_type& dict, const word_type&, const word_type& nxt) const override
	{
		if (nxt.empty()) return false;
		for (auto&& permutation : permutations(nxt))
		{
			dict.add_word(permutation);
		}
		return true;
	}
};

#endif // !REVERSED_INDEX_STANDARD_FILLER_
