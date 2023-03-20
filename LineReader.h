#ifndef LINE_READER_
#define LINE_READER_
#include "IFiller.h"
#include "Dictionary.h"
#include "Utilities.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>


bool invalidChar(char c) noexcept
{
	return !(c >= 0 && c < 128);
}

void stripUnicode(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), &invalidChar), str.end());
}

template<typename Dictionary>
class LineReader : public IFiller<Dictionary>
{
public:
	using dict_type = Dictionary;

public:

	constexpr LineReader(const filename_t& filename, const std::string& delims) noexcept : 
		filename_(filename), delims_(delims), is_over_(false) {}

public:

	size_t read(dict_type& res) const override
	{
		std::string line;
		std::ifstream file(filename_);
		size_t words_total_count = 0u;
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				::stripUnicode(line);
				to_lower_case(line);
				auto words = tokenize(line, delims_);
				for (auto&& word : words)
				{
					if (!word.empty())
					{
						res.add_word(word); 
						++words_total_count;
					}
				}
			}
			return words_total_count;
		}
		else
		{
			file.close();
			throw std::runtime_error("Could not open the file: " + filename_);
		}
		file.close();
	}

	void finish() noexcept
	{
		is_over_ = true;
	}

	virtual bool is_over() const noexcept
	{
		return is_over_;
	}


private:

	void to_lower_case(std::string& data) const noexcept
	{
		std::transform
		(
			data.begin(), 
			data.end(), 
			data.begin(),
			[](unsigned char c) { return std::tolower(c); }
		);
	}

private:
	const filename_t filename_;
	const std::string delims_;
	bool is_over_;
};

#include "InvertedIndexer.h"

template<>
class LineReader<InvertedIndexer> : public IFiller<InvertedIndexer>
{
public:
	using dict_type = InvertedIndexer;

public:

	constexpr LineReader(const filename_t& filename, const std::string& delims) noexcept :
		filename_(filename), delims_(delims), is_over_(false) {}

public:

	size_t read(dict_type& res) override
	{
		std::string line;
		std::ifstream file(filename_);
		size_t words_total_count = 0u;
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				::stripUnicode(line);
				to_lower_case(line);
				auto words = tokenize(line, delims_);
				for (auto&& word : words)
				{
					if (!word.empty())
					{
						res.add_word(std::forward<std::string>(word), (filename_));
						++words_total_count;
					}
				}
			}
			finish();
			return words_total_count;
		}
		else
		{
			file.close();
			throw std::runtime_error("Could not open the file: " + filename_);
		}
		file.close();
	}


	void finish() noexcept
	{
		is_over_ = true;
	}

	virtual bool is_over() const noexcept
	{
		return is_over_;
	}

private:

	void to_lower_case(std::string& data) const noexcept
	{
		std::transform
		(
			data.begin(),
			data.end(),
			data.begin(),
			[](unsigned char c) { return std::tolower(c); }
		);
	}

private:
	const filename_t filename_;
	const std::string delims_;
	bool is_over_;
};


#include "IncidenceMatrix.h"

template<>
class LineReader<IncidenceMatrix> : public IFiller<IncidenceMatrix>
{
public:
	using dict_type = IncidenceMatrix;

public:

	constexpr LineReader(const filename_t& filename, const std::string& delims) noexcept :
		filename_(filename), delims_(delims), is_over_(false) {}

public:

	size_t read(dict_type& res) override
	{
		std::string line;
		std::ifstream file(filename_);
		size_t words_total_count = 0u;
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				::stripUnicode(line);
				to_lower_case(line);
				auto words = tokenize(line, delims_);
				for (auto&& word : words)
				{
					if (!word.empty())
					{
						res.add_word(std::forward<std::string>(word), (filename_));
						++words_total_count;
					}
				}
			}
			finish();
			return words_total_count;
		}
		else
		{
			file.close();
			throw std::runtime_error("Could not open the file: " + filename_);
		}
		file.close();
	}

	void finish() noexcept
	{
		is_over_ = true;
	}

	virtual bool is_over() const noexcept
	{
		return is_over_;
	}

private:

	void to_lower_case(std::string& data) const noexcept
	{
		std::transform
		(
			data.begin(),
			data.end(),
			data.begin(),
			[](unsigned char c) { return std::tolower(c); }
		);
	}

private:
	const filename_t filename_;
	const std::string delims_;
	bool is_over_;
};


#endif // !LINE_READER_
