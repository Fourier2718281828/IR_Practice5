#ifndef ENCODING_
#define ENCODING_
#include <string>
#include <codecvt>

enum class Encodings
{
	ANSI,
};

template<Encodings>
struct EncodingTrates;

template<>
struct EncodingTrates<Encodings::ANSI>
{
	using word_type = std::string;
	//using codecvt_t = std::codecvt_utf8 <word_type::value_type>;
};

#endif // !ENCODING_
