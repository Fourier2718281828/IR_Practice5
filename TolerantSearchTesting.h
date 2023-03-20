#ifndef TOLERANT_SEARCH_TESTING_
#define TOLERANT_SEARCH_TESTING_
#include <iostream>
#include "Trie.h"
#include "OptimizedDictionaryStandardFiller.h"
#include "TextFileSerializer.h"
#include "LineFileReader.h"
#include "ReversedReaderDecorator.h"
#include "TrieAsteriskHandler.h"
#include "PermutermIndexStandardFiller.h"
#include "PermutermIndexAsteriskHandler.h"
#include "KGramIndexer.h"
#include "KGramIndexStandardFiller.h"
#include "KGramAsteriskHandler.h"
#include <filesystem>
#include <fstream>

namespace testing
{

#define PREPARE_THE_TRIES 0

	void serialize_tree(const Trie<char>& trie, const std::string& filename)
	{
		using std::cout;
		using std::cin;
		TextFileSerializer<std::ofstream, std::ifstream, Trie<char>> serializer;
		std::ofstream file("Output Files/" + filename);
		if (!file.is_open())
		{
			cout << "could not open the file\n";
			return;
		}
		cout << "Serializing...\n";
		serializer.serialize(file, trie);
		cout << "Serialization finished.\n";
	}

	template<size_t K>
	void serialize_kgram(const KGramIndexer<std::string, K>& ind, const std::string& filename)
	{
		using std::cout;
		using std::cin;
		TextFileSerializer<std::ofstream, std::ifstream, KGramIndexer<std::string, K>> serializer;
		std::ofstream file("Output Files/" + filename);
		if (!file.is_open())
		{
			cout << "could not open the file\n";
			return;
		}
		cout << "Serializing...\n";
		serializer.serialize(file, ind);
		cout << "Serialization finished.\n";
	}

	[[nodiscard]] std::pair<Trie<char>, Trie<char>> build_trees(const std::string& folder)
	{
		using std::cout;
		using std::cin;
		Trie<char> trie, rev_trie;
		size_t total_words1 = 0u;
		size_t total_words2 = 0u;
		for (const auto& path : std::filesystem::directory_iterator(folder))
		{
			std::shared_ptr<IFileReader<std::string>> reader1 =
				std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());
			IFiller<Trie<char>>* filler1 = new OptimizedDictionaryStandardFiller<Trie<char>>(reader1);

			std::shared_ptr<IFileReader<std::string>> readernd2 =
				std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());

			std::shared_ptr<IFileReader<std::string>> reader2 =
				std::make_shared<ReversedFileReaderDecorator>(*readernd2);

			IFiller<Trie<char>>* filler2 = new OptimizedDictionaryStandardFiller<Trie<char>>(reader2);

			cout << "# \"" << path.path().string() << "\" processing...\n";
			auto start = std::chrono::steady_clock::now();
			total_words1 += filler1->read(trie);
			total_words2 += filler2->read(rev_trie);
			auto end = std::chrono::steady_clock::now();
			auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			cout << "\tread in " << time1 << " ms\n";

			delete filler1; filler1 = nullptr;
			delete filler2; filler2 = nullptr;
		}

		cout << "--------------------------------------------\n";
		cout << "Total  words (original): " << total_words1 << '\n';
		cout << "Total  words (reversed): " << total_words2 << '\n';
		cout << "Unique words (original): " << trie.size() << '\n';
		cout << "Unique words (reversed): " << rev_trie.size() << '\n';

		return { std::move(trie), std::move(rev_trie) };
	}

	Trie<char> build_permutations_trie(const std::string& folder)
	{
		using std::cout;
		using std::cin;
		Trie<char> trie;
		size_t words_count = 0u;
		for (const auto& path : std::filesystem::directory_iterator(folder))
		{
			std::shared_ptr<IFileReader<std::string>> reader1 =
				std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());
			IFiller<Trie<char>>* filler1 = new PermutermIndexStandardFiller<Trie<char>>(reader1);

			cout << "# \"" << path.path().string() << "\" processing...\n";
			auto start = std::chrono::steady_clock::now();
			words_count += filler1->read(trie);
			auto end = std::chrono::steady_clock::now();
			auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			cout << "\tread in " << time1 << " ms\n";
		}
		cout << "--------------------------------------------\n";
		cout << "Total  words : " << words_count << '\n';
		cout << "Unique words : " << trie.size() << '\n';

		return trie;
	}

	template<size_t K>
	KGramIndexer<std::string, K> build_kgram(const std::string& folder)
	{
		using std::cout;
		using std::cin;
		KGramIndexer<std::string, K> trie;
		size_t words_count = 0u;
		for (const auto& path : std::filesystem::directory_iterator(folder))
		{
			std::shared_ptr<IFileReader<std::string>> reader1 =
				std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());
			IFiller<KGramIndexer<std::string, K>>* filler1 =
				new KGramIndexStandardFiller<KGramIndexer<std::string, K>, K>(reader1);

			cout << "# \"" << path.path().string() << "\" processing...\n";
			auto start = std::chrono::steady_clock::now();
			words_count += filler1->read(trie);
			auto end = std::chrono::steady_clock::now();
			auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			cout << "\tread in " << time1 << " ms\n";
		}
		cout << "--------------------------------------------\n";
		cout << "Total  words : " << words_count << '\n';
		cout << "Unique words : " << trie.size() << '\n';

		return trie;
	}

	void prepare_orig_and_reversed_tries()
	{
		const std::string folder = "Input Files/";
		auto&& [trie, rev_trie] = build_trees(folder);
		serialize_tree(trie, "Trie.txt");
		serialize_tree(rev_trie, "ReversedTrie.txt");
	}

	void prepare_permutations_trie()
	{
		const std::string folder = "Input Files/";
		const auto trie = build_permutations_trie(folder);
		serialize_tree(trie, "PermutationsTrie.txt");
	}

	template<size_t K>
	void prepare_kgram_indexer()
	{
		static_assert(K > 1);
		const std::string folder = "Input Files/";
		const auto trie = build_kgram<K>(folder);
		serialize_kgram<K>(trie, std::to_string(K) + "GramIndex.txt");
	}

	void prepare_the_all_files()
	{
		prepare_orig_and_reversed_tries();
		prepare_permutations_trie();
		prepare_kgram_indexer<2>();
		prepare_kgram_indexer<3>();
		prepare_kgram_indexer<4>();
	}

	[[nodiscard]] Trie<char> deserialize_trie(const std::string& filename)
	{
		using std::cout;
		using std::cin;
		TextFileSerializer<std::ofstream, std::ifstream, Trie<char>> serializer;
		std::ifstream file("Output Files/" + filename);
		if (!file.is_open())
		{
			cout << "could not open the file\n";
			throw std::runtime_error("could not open the file");
		}
		cout << "Deserializing from " << filename << "...\n";
		auto res = serializer.deserialize(file);
		cout << "Deserialization from " << filename << " finished.\n";
		return res;
	}

	template<size_t K>
	[[nodiscard]] KGramIndexer<std::string, K> deserialize_kgram(const std::string& filename)
	{
		using std::cout;
		using std::cin;
		TextFileSerializer<std::ofstream, std::ifstream, KGramIndexer<std::string, K>> serializer;
		std::ifstream file("Output Files/" + filename);
		if (!file.is_open())
		{
			cout << "could not open the file\n";
			throw std::runtime_error("could not open the file");
		}
		cout << "Deserializing from " << filename << "...\n";
		auto res = serializer.deserialize(file);
		cout << "Deserialization from " << filename << " finished.\n";
		return res;
	}

	void asterisk_queries_testing(IQueryHandler<std::string>& handler)
	{
		using std::cout;
		using std::cin;
		std::string input;
		do
		{
			cout << '>';
			cin >> input;
			if (handler.validate(input))
			{
				auto iterable = handler.handle_query(input);
				cout << "RESULT:\n";
				for (auto&& word : iterable)
				{
					cout << word << '\n';
				}
			}
			else
				cout << "\n\t***Invalid Query.\n";
		} while (true);
	}

	void test_trees()
	{
		auto trie = deserialize_trie("Trie.txt");
		auto rev_trie = deserialize_trie("ReversedTrie.txt");
		TrieAsteriscHandler handler(trie, rev_trie);
		asterisk_queries_testing(handler);
	}

	void test_permutation_trees()
	{
		auto trie = deserialize_trie("PermutationsTrie.txt");
		PermutermIndexAsteriskHandler handler(trie);
		asterisk_queries_testing(handler);
	}

	void test_kgram_indexers()
	{
		auto threegram = deserialize_kgram<3>("3GramIndex.txt");
		auto bigram = deserialize_kgram<2>("2GramIndex.txt");
		KGramAsteriskQueryHandler handler(bigram, threegram);
		asterisk_queries_testing(handler);
	}

	namespace tolerant_search
	{
		void execute_testing()
		{
			using std::cout;
			using std::cin;
#if PREPARE_THE_TRIES
			prepare_the_all_files();
#else
			int choice;
			cout << "1. Search based on two tries\n"
				<< "2. Search based on permuterm tries\n"
				<< "3. Search based on kgram indices\nYour choice: ";
			cin >> choice;
			cout << "choice: " << choice << '\n';


			while (choice != 1 && choice != 2 && choice != 3)
			{
				cout << "Invalid input!\nTry again: ";
				cin >> choice;
			}

			switch (choice)
			{
			case 1:
				test_trees();
				break;
			case 2:
				test_permutation_trees();
				break;
			case 3:
				test_kgram_indexers();
				break;
			default:
				cout << "Invalid input!\nTry again: ";
				cin >> choice;
			}
#endif
		}
	}
}

#endif // !TOLERANT_SEARCH_TESTING_
