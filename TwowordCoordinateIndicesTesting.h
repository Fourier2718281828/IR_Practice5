#ifndef TW_RD_COORD_TESTING_
#define TW_RD_COORD_TESTING_
#include <iostream>
#include "LineFileReader.h"
#include "CoordinateReader.h"
#include "TwoWordIndexer.h"
#include "CoordMapAdapted.h"
#include "CoordinateReader.h"
#include "CoordinateInvertedIndex.h"
#include "TwoWordIndexerStandardFiller.h"
#include "TextFileSerializer.h"
#include "DocumentIndexerSerializer.h"
#include "Utilities.h"

namespace testing
{
	namespace two_word_index
	{

		void to_lower_case(std::string& data) noexcept
		{
			std::transform
			(
				data.begin(),
				data.end(),
				data.begin(),
				[](unsigned char c) { return std::tolower(c); }
			);
		}

		TwoWordInvertedIndexer prepare_index(const std::string folder)
		{
			using std::cout;
			TwoWordInvertedIndexer dict;

			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				std::shared_ptr<IFileReader<std::string>> reader =
					std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());

				TwoWordIndexerStandardFiller<TwoWordInvertedIndexer> filler(reader);
				filler.read(dict);
			}

			return dict;
		}

		void phrase_search(TwoWordInvertedIndexer& indexer)
		{
			using word_type = typename TwoWordInvertedIndexer::word_type::first_type;
			using std::cin;
			using std::cout;

			cout << "Phrase search (input \"_end\" to finish):\n";
			std::string input;
			while (true)
			{
				cout << "> ";
				std::getline(cin, input);
				if (input == "_end") break;
				Iterable<std::string> tokens = tokenize<std::string, std::string>
					(input, std::string(" \t\v\f\r\n,.;:!?*[]()\"\"0123456789/'{}|@#$&=%\\-"));
				query_result res;
				for (size_t i = 1u; i < tokens.size(); ++i)
				{
					word_type word1 = tokens[i - 1];
					word_type word2 = tokens[i];
					to_lower_case(word1);
					to_lower_case(word2);
					if (i == 1u)
						res = and_query(indexer, word1, word2);
					else
						res = and_query(indexer, res, and_query(indexer, word1, word2 ));
				}
				cout << "RESULT:\n";
				for (auto&& doc : res)
				{
					cout << indexer.get_docids()[doc] << '\n';
				}
			}
		}

		void execute_testing()
		{
			const std::string path = "Input Files/";
			auto two_word_index = prepare_index(path);
			phrase_search(two_word_index);
		}
	}

	namespace coordinate_index
	{
		
		CoordinateInvertedIndexer prepare_index(const std::string folder)
		{
			using std::cout;
			CoordinateInvertedIndexer dict;

			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				std::shared_ptr<IFileReader<std::string>> reader =
					std::make_shared<LineFileReader<>>(path.path().string());

				CoordinateReader filler(reader);
				filler.read(dict);
			}

			return dict;
		}

		bool is_valid_string(const std::string& s, const std::string& bad_symbols) 
		{
			return s.find_first_of(bad_symbols) == std::string::npos;
		}

		void distance_search(CoordinateInvertedIndexer& indexer)
		{
			using std::cin;
			using std::cout;

			cout << "Phrase search (input \"_end\" to finish):\n";
			std::string input;
			while (true)
			{
				cout << "> ";
				std::getline(cin, input);
				if (input == "_end") break;
				std::regex query_regex("(([a-zA-Z]+)\\s(\\d+)\\s)*([a-zA-Z]+)");
				if (!std::regex_match(input, query_regex))
				{
					cout << "Invalid input! Try again!\n";
					continue;
				}
				Iterable<std::string> tokens = tokenize<std::string, std::string>
					(input, std::string(" \t\v\f\r\n,.;:!?*[]()\"\"/'{}|@#$&=%\\-"));

				std::vector<std::string> words;
				std::vector<size_t>  distances;
				bool is_valid_input = !tokens.empty();

				for (auto&& token : tokens)
				{
					try
					{
						distances.push_back(std::stoi(token.c_str()));
					}
					catch (...)
					{
						words.push_back(token);
						is_valid_input = is_valid_string(token, "0123456789");
					}
				}

				is_valid_input = is_valid_input && words.size() == distances.size() + 1;

				if (!is_valid_input)
				{
					cout << "Invalid input! Try again!\n";
					continue;
				}

				SearchResult res = distance_search(indexer, words, distances);
				
				cout << "RESULT:\n";
				for (auto&& doc : res)
				{
					cout << indexer.get_docids()[doc] << '\n';
				}
			}
		}

		void execute_testing()
		{
			const std::string path = "Input Files/";
			auto coord_index = prepare_index(path);
			distance_search(coord_index);
			/*auto res = distance_search(coord_index, {"told", "could", "me"}, {3, 4});
			for (auto r : res)
			{
				std::cout << r << ' ' << '\n';
			}*/
		}
	}
}

#endif // !TW_RD_COORD_TESTING_
