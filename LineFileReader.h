#ifndef LINE_FILE_READER_
#define LINE_FILE_READER_
#include <fstream>
#include <type_traits>
#include <algorithm>
#include "IFileReader.h"
#include "FileTypes.h"
#include "Encoding.h"
#include "Utilities.h"

template
<
	Encodings Encoding = Encodings::ANSI, 
	typename Tokenizer = StandardTokenizer
>
class LineFileReader : public IFileReader<typename EncodingTrates<Encoding>::word_type>
{
public:
	using encode_traits = EncodingTrates<Encoding>;
	using word_type = typename encode_traits::word_type;
	using delims_type = word_type;
	using token_container_type = std::queue<word_type>;

public:

	constexpr LineFileReader
	(
		const filename_t& filename,
		const delims_type& delims = s_DEFAULT_DELIMS
	) :
		buffer_(),
		delims_(delims),
		filename_(filename),
		fin_(filename)
	{
		if (!fin_.is_open())
		{
			fin_.close();
			throw std::runtime_error("File not found: " + filename);
		}
	}

	~LineFileReader()
	{
		fin_.close();
	}

public:

	[[nodiscard]] word_type next_word() const override
	{
		while (buffer_.empty() && !is_eof())
		{
			word_type line;
			std::getline(fin_, line);
			stripUnicode(line);
			to_lower_case(line);
			buffer_ = Tokenizer::tokenize(line, delims_);
		}

		if (is_eof())
			return word_type();

		auto next = buffer_.front();
		buffer_.pop();
		return next;
	}

	[[nodiscard]] bool is_eof() const noexcept override
	{
		return fin_.eof();
	}

	const filename_t& get_openfile_name() const noexcept override
	{
		return filename_;
	}

private:

	static void to_lower_case(word_type& data) noexcept
	{
		std::transform
		(
			data.begin(),
			data.end(),
			data.begin(),
			[](unsigned char c) { return std::tolower(c); }
		);
	}

	static bool invalidChar(char c) noexcept
	{
		return !(c >= 0 && c < 128);
	}

	static void stripUnicode(std::string& str)
	{
		str.erase(std::remove_if(str.begin(), str.end(), &invalidChar), str.end());
	}

private:
	mutable token_container_type buffer_;
	const delims_type delims_;
	const filename_t filename_;
	mutable std::ifstream fin_;
	
private:
	static const delims_type s_DEFAULT_DELIMS;
};

template
<
	Encodings Encoding,
	typename Tokenizer
>
const typename LineFileReader<Encoding, Tokenizer>::delims_type
LineFileReader<Encoding, Tokenizer>::s_DEFAULT_DELIMS = " \t\v\f\r\n,.;:!?*[]()\"\"0123456789/'{}|@#$&=%\\-";

#endif // !LINE_FILE_READER_
