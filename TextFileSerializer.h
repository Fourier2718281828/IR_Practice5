#ifndef TEXT_FILE_SERIALIZER_
#define TEXT_FILE_SERIALIZER_
#include <fstream>
#include "ISerializer.h"
#include "Dictionary.h"

template<typename FOutStream>
void text_serialize_string(FOutStream& fout, const std::string& str)
{
	if (fout.is_open())
	{
		fout << str << '\n';
	}
	else
	{
		throw std::runtime_error("Only open stream should be passed");
	}
}

template<typename FInStream>
std::string text_deserialize_string(FInStream& fin)
{
	if (fin.is_open())
	{
		std::string res;
		std::getline(fin, res);
		return res;
	}
	else
	{
		throw std::runtime_error("Only open stream should be passed");
	}
}

template<typename FOutStream, typename FInStream, typename Serializable>
class TextFileSerializer : public ISerializer<FOutStream, FInStream, Serializable>
{
public:
	void serialize(FOutStream& fout, const Serializable& dict) const override
	{
		if (fout.is_open())
		{
			fout << Serializable::is_optimized() << ' ' << dict.size() << '\n';
			if constexpr (Serializable::is_optimized())
			{
				for (auto&& word : dict)
				{
					fout << word << '\n';
				}
			}
			else
			{
				for (auto&& [word, list] : dict)
				{
					fout << word << ' ' << list.size() << ' ';
					for (auto&& node : list)
					{
						fout << node << ' ';
					}
					fout << '\n';
				}
			}

		}
		else
		{
			throw std::runtime_error("Only open stream should be passed");
		}
	}

	Serializable deserialize(FInStream& fin) const override
	{
		if (fin.is_open())
		{
			using container_type = typename Serializable::container_type;
			using size_type = typename container_type::size_type;
			using word_type = typename Serializable::word_type;

			container_type res{};
			size_type size;
			bool is_optimized;

			fin >> is_optimized >> size;

			if (is_optimized != Serializable::is_optimized())
				throw std::runtime_error("Dictionary optimization mode mismatch.");

			for (decltype(size) i = 0u; i < size; ++i)
			{
				word_type read_word;
				if constexpr (Serializable::is_optimized())
				{
					fin >> read_word;
					res.add(std::move(read_word));
				}
				else
				{
					using list_type = typename Serializable::list_type;
					using listnode_type = typename Serializable::listnode_type;
					using list_size_type = typename list_type::size_type;

					list_type ids_list{};
					list_size_type list_size;
					fin >> read_word >> list_size;

					for (list_size_type i = 0u; i < list_size; ++i)
					{
						listnode_type read_node;
						fin >> read_node;
						res[read_word].add(std::move(read_node));
					}
				}
			}

			return Serializable{ std::move(res) };
		}
		else
		{
			throw std::runtime_error("Only open stream should be passed");
		}
	}
};




//
//
//class TwoWordIndexer : public ISerializer<std::ofstream, std::ifstream, TwoWordInvertedIndexer>
//{
//public:
//	void serialize(std::ofstream& fout, const TwoWordInvertedIndexer& dict) const override
//	{
//		if (fout.is_open())
//		{
//			for (auto&& [word, list] : dict.get_dict())
//			{
//				fout << word.first << ' ' << word.second << ' ' << list.size() << ' ';
//				for (auto&& node : list)
//				{
//					fout << node << ' ';
//				}
//				fout << '\n';
//			}
//
//		}
//		else
//		{
//			throw std::runtime_error("Only open stream should be passed");
//		}
//	}
//
//	TwoWordInvertedIndexer deserialize(std::ifstream& fin) const override
//	{
//		if (fin.is_open())
//		{
//			using container_type = typename TwoWordInvertedIndexer::dict_type::container_type;
//			using size_type = typename container_type::size_type;
//			using word_type = typename TwoWordInvertedIndexer::word_type;
//
//			container_type res{};
//			size_type size;
//			bool is_optimized;
//
//			fin >> is_optimized >> size;
//
//			for (decltype(size) i = 0u; i < size; ++i)
//			{
//				std::string fword, sword;
//				using list_type = typename TwoWordInvertedIndexer::dict_type::list_type;
//				using listnode_type = typename TwoWordInvertedIndexer::dict_type::listnode_type;
//				using list_size_type = typename list_type::size_type;
//
//				list_type ids_list{};
//				list_size_type list_size;
//				fin >> fword >> sword >> list_size;
//
//				for (list_size_type i = 0u; i < list_size; ++i)
//				{
//					listnode_type read_node;
//					fin >> read_node;
//					word_type pair{ fword, sword };
//					res[pair].add(read_node);
//				}
//			}
//
//			return TwoWordInvertedIndexer{ std::move(res) };
//		}
//		else
//		{
//			throw std::runtime_error("Only open stream should be passed");
//		}
//	}
//};

#endif // !TEXT_FILE_SERIALIZER_
