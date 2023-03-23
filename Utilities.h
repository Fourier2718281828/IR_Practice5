#ifndef UTILITIES_
#define UTILITIES_
#include <vector>
#include <memory>

template<typename Word>
using Iterable = std::vector<Word>;

template<typename Word, typename DelimWord>
Iterable<Word> tokenize(const Word& word, const DelimWord& delims)
{
	using _Itor = typename Word::const_iterator;
	Iterable<Word> res;
	_Itor it1 = word.begin();
	_Itor it2 = word.begin();
	_Itor end = word.end();

	auto is_delim = [&delims](Word::value_type c) -> bool
	{
		return std::find(delims.begin(), delims.end(), c) != delims.end();
	};

	for (; it2 != end; ++it2)
	{
		if (is_delim(*it2))
		{
			res.emplace_back(it1, it2);
			it1 = it2;
			++it1;
		}
	}

	if (it1 != end)
	{
		res.emplace_back(it1, it2);
		it1 = it2;
	}

	return res;
}

#include <queue>

template<typename Word, typename DelimWord>
std::queue<Word> tokenizeQueue(const Word& word, const DelimWord& delims)
{
	using _Itor = typename Word::const_iterator;
	std::queue<Word> res;
	_Itor it1 = word.begin();
	_Itor it2 = word.begin();
	_Itor end = word.end();

	auto is_delim = [&delims](Word::value_type c) -> bool
	{
		return std::find(delims.begin(), delims.end(), c) != delims.end();
	};

	for (; it2 != end; ++it2)
	{
		if (is_delim(*it2))
		{
			res.emplace(it1, it2);
			it1 = it2;
			++it1;
		}
	}

	if (it1 != end)
	{
		res.emplace(it1, it2);
		it1 = it2;
	}

	return res;
}


class StandardTokenizer
{
public:
	
	template<typename Word, typename DelimWord>
	static std::queue<Word> tokenize(const Word& word, const DelimWord& delims)
	{
		return ::tokenizeQueue(word, delims);
	}
};

//template<typename Derived, typename Base>
//	requires std::is_convertible_v<Derived&, Base&>&&
//std::is_polymorphic_v<Base>
//inline std::unique_ptr<Derived> cast_to(std::unique_ptr<Base>&& ptr)
//{
//	Derived* d = dynamic_cast<Derived*>(ptr.get());
//	if (d)
//	{
//		ptr.release();
//		return std::unique_ptr<Derived>(d);
//	}
//	return nullptr;
//}

struct NullType0 {};

template<typename T>
struct NullType1 { using value_type = T; };

template<typename T1, typename T2>
struct NullType2 {};

template
<
	typename T,
	template<typename> typename Iterable
>
Iterable<T> intersect(const Iterable<T>& p1, const Iterable<T>& p2)
{
	Iterable<T> res{};
	auto it1 = p1.begin();
	auto it2 = p2.begin();

	while (it1 != p1.end() && it2 != p2.end())
	{
		if (*it1 == *it2)
		{
			res.add(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return res;
}

template
<
	typename T,
	template<typename> typename Iterable
>
Iterable<T> intersect_set(const Iterable<T>& p1, const Iterable<T>& p2)
{
	Iterable<T> res{};
	auto it1 = p1.begin();
	auto it2 = p2.begin();

	while (it1 != p1.end() && it2 != p2.end())
	{
		if (*it1 == *it2)
		{
			res.insert(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return res;
}

template
<
	typename Iterable1,
	typename Iterable2
>
Iterable1 intersect_special(const Iterable1& p1, const Iterable2& p2)
{
	Iterable1 res{};
	auto it1 = p1.begin();
	auto it2 = p2.begin();

	while (it1 != p1.end() && it2 != p2.end())
	{
		if (*it1 == *it2)
		{
			res.insert(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return res;
}



template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T>
using sptr = std::shared_ptr<T>;

//template<typename T>
//concept Iterable = requires(T t) {
//	typename T::iterator;  
//	typename T::const_iterator;  
//	{ begin(t) } -> std::same_as<typename T::iterator>;
//	{ end(t) } -> std::same_as<typename T::iterator>;
//	{ cbegin(t) } -> std::same_as<typename T::const_iterator>; 
//	{ cend(t) } -> std::same_as<typename T::const_iterator>;
//};

template<typename WordType>
Iterable<WordType> permutations(WordType word) noexcept
{
	const typename WordType::value_type special_char = '$';
	word.push_back(special_char);
	Iterable<WordType> res{ word };

	while (word[0] != special_char)
	{
		word.push_back(word[0]);
		word.erase(0, 1);
		res.push_back(word);
	}

	return res;
}

template<typename WordType>
Iterable<WordType> bare_kgrams_of_word(const WordType& dollared_word, const typename WordType::size_type K) noexcept
{
	using size_type = typename WordType::size_type;
	Iterable<WordType> res{};
	size_type iterations = dollared_word.size() + 1 >= K
		? dollared_word.size() - K + 1
		: 0u;

	for (size_type i = 0; i < iterations; ++i)
	{
		res.push_back(dollared_word.substr(i, K));
	}

	return res;
}

template<typename WordType>
Iterable<WordType> kgrams_of_word(const WordType& word0, const typename WordType::size_type K) noexcept
{
	const typename WordType::value_type special_char = '$';
	const WordType dollared_word = special_char + word0 + special_char;
	return bare_kgrams_of_word(dollared_word, K);
}

#endif // !UTILITIES_
