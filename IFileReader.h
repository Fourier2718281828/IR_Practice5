#ifndef I_FILE_READER_
#define I_FILE_READER_
#include "FileTypes.h"

template<typename WordType>
class IFileReader
{
public:
	using word_type = WordType;
public:
	virtual ~IFileReader() = default;
public:
	[[nodiscard]] virtual word_type next_word() const			 = 0;
	[[nodiscard]] virtual bool is_eof() const noexcept			 = 0;
	virtual const filename_t& get_openfile_name() const noexcept = 0;
};

#endif // !I_FILE_READER_
