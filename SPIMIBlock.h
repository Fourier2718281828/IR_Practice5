#ifndef SPIMI_BLOCK_
#define SPIMI_BLOCK_
#include "InvertedIndexer.h"
#include "FileTypes.h"
#include "IFileReader.h"
#include <memory>

class SPIMIBlock
{
public:
	using indexer_type = InvertedIndexer;
	using idmapper_ptr = typename indexer_type::idmapper_ptr;
	using reader_ptr = std::shared_ptr<IFileReader<std::string>>;

public:

	SPIMIBlock(const size_t max_size, idmapper_ptr idmapper = nullptr) :
		max_size_(max_size),
		indexer_ { idmapper }
	{
	}

	SPIMIBlock(SPIMIBlock&&) noexcept = default;
	SPIMIBlock& operator=(SPIMIBlock&&) noexcept = default;

	size_t get_filled(const reader_ptr reader) noexcept
	{
		size_t words_count = 0u;
		while (!is_fool() && !reader->is_eof())
		{
			auto nxt_word = reader->next_word();
			auto document = reader->get_openfile_name();
			if (!nxt_word.empty())
			{
				auto prev_size = indexer_.size();
				indexer_.add_word(std::move(nxt_word), std::move(document));
				words_count += indexer_.size() - prev_size;
			}
		}

		return words_count;
	}

	bool is_fool() const noexcept
	{
		return indexer_.size() >= max_size_;
	}

	void set_max_size(size_t new_max_size) noexcept
	{
		max_size_ = new_max_size;
	}

	size_t current_size() const noexcept
	{
		return indexer_.size();
	}

	size_t max_size() const noexcept
	{
		return max_size_;
	}

	const indexer_type& get_indexer() const noexcept
	{
		return indexer_;
	}

private:
	indexer_type  indexer_;
	size_t max_size_;
};

#endif // !SPIMI_BLOCK_
