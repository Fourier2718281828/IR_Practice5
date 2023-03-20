#ifndef STANDARD_FILLER_
#define STANDARD_FILLER_
#include "IFiller.h"
#include "IFileReader.h"
#include "Dictionary.h"
#include <memory>

template<typename Dictionary>
class AStandardFiller : public IFiller<Dictionary>
{
public:
	using reader_ptr = std::shared_ptr<IFileReader<std::string>>;
	using word_type = reader_ptr::element_type::word_type;

public:

	AStandardFiller(const reader_ptr reader) :
		reader_(reader), is_over_(false) {}

public:

	size_t read(Dictionary& dict) override
	{
		size_t words_count = 0u;
		word_type prev_word;
		while (!reader_->is_eof())
		{
			word_type read_word = reader_->next_word();
			bool read = add_word(dict, prev_word, read_word);
			prev_word = read_word;
			if(read) ++words_count;
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

	virtual bool add_word(Dictionary& dict, const word_type&, const word_type&) const = 0;

private:
	reader_ptr reader_;
	bool      is_over_;
};





#endif // !STANDARD_FILLER_
