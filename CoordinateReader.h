#ifndef COORDINATE_READER_
#define COORDINATE_READER_
#include "IFiller.h"
#include "IFileReader.h"
#include "Dictionary.h"
#include "CoordinateInvertedIndex.h"
#include <memory>

class CoordinateReader : public IFiller<CoordinateInvertedIndexer>
{
public:
	using reader_ptr = std::shared_ptr<IFileReader<std::string>>;
	using word_type = reader_ptr::element_type::word_type;

public:

	CoordinateReader(const reader_ptr reader) :
		reader_(reader), is_over_(false) {}

public:

	size_t read(CoordinateInvertedIndexer& dict)  override
	{
		size_t words_count = 0u;
		coordinate_t position = static_cast<coordinate_t>(-1);
		while (!reader_->is_eof())
		{
			word_type read_word = reader_->next_word();
			if (read_word.empty()) continue;
			dict.add_word(read_word, get_filename(), ++position);
			++words_count;
		}
		finish();
		
		return words_count;
	}

	void finish() noexcept
	{
		is_over_ = true;
	}

	virtual bool is_over() const noexcept
	{
		return is_over_;
	}

protected:

	const filename_t& get_filename() const noexcept
	{
		return reader_->get_openfile_name();
	}

private:
	reader_ptr reader_;
	bool is_over_;
};

#endif // !COORDINATE_READER_
