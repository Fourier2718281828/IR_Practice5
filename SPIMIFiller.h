#ifndef SPIMI_FILLER
#define SPIMI_FILLER
#include "IRefreshableFiller.h"
#include "IFileReader.h"
#include "SPIMIBlock.h"
#include <vector>

#include <cassert>

using SPIMIBlocks = std::vector<SPIMIBlock>;

class SPIMIFiller : public IRefreshableFiller<SPIMIBlocks>
{
public:
	using reader_ptr = typename SPIMIBlock::reader_ptr;
	using word_type = reader_ptr::element_type::word_type;
	using reader_container = std::vector<reader_ptr>;
	using iterator = typename reader_container::const_iterator;
private:
	using block_ptr = std::unique_ptr<SPIMIBlock>;
public:

	SPIMIFiller
	(
		const reader_container& readers, 
		const size_t block_size, 
		const size_t word_limit
	) :
		begin_itor_(readers.begin()),
		end_itor_(readers.end()),
		readers_(readers), 
		block_size_(block_size),
		word_limit_(word_limit),
		word_count_(0u)
	{}

public:

	size_t read(SPIMIBlocks& blocks) override
	{
		size_t words_count = 0u;
		std::unique_ptr<SPIMIBlock> block = new_block();

		while (!is_over())
		{
			auto&& reader = *begin_itor_;
			while (!reader->is_eof())
			{
				if (endangers_limit(*block))
					block->set_max_size(block->current_size() + left_to_reach_limit());
				auto read_words = block->get_filled(reader);
				words_count += read_words;
				word_count_ += read_words;
				if (block->is_fool() || reached_limit())
				{
					blocks.push_back(std::move(*block));
					block = new_block();
					if (reached_limit())
					{
						return words_count;
					}
				}
			}
			
			++begin_itor_;
			if (is_over())
			{
				blocks.push_back(std::move(*block));
			}
		}

		return words_count;
	}

	bool reached_limit() const noexcept
	{
		return word_limit_ == word_count_;
	}

	size_t left_to_reach_limit() const noexcept
	{
		assert(word_limit_ - word_count_ > 0);
		return word_limit_ - word_count_;
	}

	bool endangers_limit(const SPIMIBlock& block) const noexcept
	{
		assert(block.max_size() - block.current_size() > 0);
		return block.max_size() - block.current_size() > left_to_reach_limit();
	}

	void refresh() override
	{
		word_count_ = 0u;
	}

	bool is_over() const noexcept
	{
		return begin_itor_ == end_itor_;
	}

private:

	block_ptr new_block()
	{
		using idmapper_ptr = typename SPIMIBlock::idmapper_ptr;
		using idmapper_type = typename idmapper_ptr::element_type;
		using spimi_block_type = typename block_ptr::element_type;

		static idmapper_ptr common_idmapper = std::make_shared<idmapper_type>();
		return std::make_unique<spimi_block_type>(block_size_, common_idmapper);
	}

private:
	iterator			 begin_itor_;
	const iterator		   end_itor_;
	const reader_container& readers_;
	const size_t		 block_size_;
	const size_t		 word_limit_;
	size_t				 word_count_;
};

#endif // !SPIMI_FILLER
