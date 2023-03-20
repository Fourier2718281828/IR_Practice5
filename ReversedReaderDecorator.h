#ifndef REVERSED_READER_DECORATOR_
#define REVERSED_READER_DECORATOR_
#include "Encoding.h"
#include "LineFileReader.h"


#include <iostream>

class ReversedFileReaderDecorator : public IFileReader<typename EncodingTrates<Encodings::ANSI>::word_type>
{
public:
	using base_type = IFileReader<typename EncodingTrates<Encodings::ANSI>::word_type>;
public:

	constexpr ReversedFileReaderDecorator(const base_type& filler) :
		inner_filler_(filler)
	{

	}

	~ReversedFileReaderDecorator() override = default;

public:

	[[nodiscard]] word_type next_word() const override
	{
		auto inner_word = inner_filler_.next_word();
		std::reverse(inner_word.begin(), inner_word.end());
		return inner_word;
	}

	[[nodiscard]] bool is_eof() const noexcept override
	{
		return inner_filler_.is_eof();
	}

	const filename_t& get_openfile_name() const noexcept override
	{
		return inner_filler_.get_openfile_name();
	}

private:
	const base_type& inner_filler_;
};

#endif // !REVERSED_READER_DECORATOR_
