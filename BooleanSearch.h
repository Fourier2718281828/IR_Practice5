#ifndef BOOLEAN_SEARCH_
#define BOOLEAN_SEARCH_
#include <iostream>
#include "IndexersTesting.h"
#include "IncidenceMatrix.h"
#include "InvertedIndexer.h"
#include "DictionaryTesting.h"
#include "TolerantSearchTesting.h"


namespace testing
{
	void boolean_search_test_func(const std::string& dirpath)
	{
		using std::cout;
		using std::cin;
		cout << ">------------------------------------------<\n";
		InvertedIndexer dict;
		IncidenceMatrix matr;
		try
		{
			size_t total_words = 0u;
			for (const auto& path : std::filesystem::directory_iterator(dirpath))
			{
				IFiller<decltype(dict)>* reader = new LineReader<InvertedIndexer>(path.path().string(), " ,.;:!?*[]()\"\"0123456789/'{}|@#$&=%\\-");
				cout << "# \"" << path.path().string() << "\" processing...\n";
				auto start = std::chrono::steady_clock::now();
				total_words += reader->read(dict);
				auto end = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				cout << "\tread in " << time1 << " ms\n";
				delete reader; reader = nullptr;
			}
			cout << ">------------------------------------------<\n";

			cout << "STATISTICS:\n";
			cout << "\tGeneral number of words \t= " << total_words << '\n';
			cout << "\tTotal size of produced dict \t= " << dict.size() << '\n';

			cout << "<------------------------------------------>\n";

		}
		catch (std::exception& ex)
		{
			cout << "***ERROR : " << ex.what() << '\n';
		}

		try
		{
			size_t total_words = 0u;
			for (const auto& path : std::filesystem::directory_iterator(dirpath))
			{
				IFiller<decltype(matr)>* reader2 = new LineReader<IncidenceMatrix>(path.path().string(), " ,.;:!?*[]()\"\"0123456789/'{}|@#$&=%\\-");
				cout << "# \"" << path.path().string() << "\" processing...\n";
				auto start = std::chrono::steady_clock::now();
				total_words += reader2->read(matr);
				auto end = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				cout << "\tread in " << time1 << " ms\n";
				delete reader2; reader2 = nullptr;
			}
			cout << ">------------------------------------------<\n";

			cout << "STATISTICS:\n";
			cout << "\tGeneral number of words \t= " << total_words << '\n';
			cout << "\tTotal size of produced dict \t= " << matr.size() << '\n';

			cout << "<------------------------------------------>\n";

		}
		catch (std::exception& ex)
		{
			cout << "***ERROR : " << ex.what() << '\n';
		}
		matr.stabilize_lists();
		std::string input, w1, w2, operation;
		do
		{
			try
			{
				cout << "> ";
				cin >> input;
				if (input == "!!!") break;
				if (input == "!")
				{
					cin >> w1;
					cout << "RESULT:\n";
					cout << "Inversed Indexer:\n";
					for (auto&& w : not_query(dict, w1))
					{
						cout << '\t' << dict.get_docids()[w] << '\n';
					}
					cout << "Incidence Matrix:\n";
					const auto list = not_query(matr, w1);
					for (size_t i = 0; i < list.size(); ++i)
					{
						if (list[i])
							cout << '\t' << matr.get_docids()[i] << '\n';
					}
				}
				else if (input == "$")
				{
					cin >> w1;
					cout << "RESULT:\n";
					cout << "Inversed Indexer:\n";
					for (auto&& w : dict.get_list(w1))
					{
						cout << '\t' << dict.get_docids()[w] << '\n';
					}
					cout << "Incidence Matrix:\n";
					const auto& list = matr.get_list(w1);
					for (size_t i = 0; i < list.size(); ++i)
					{
						if (list[i])
							cout << '\t' << matr.get_docids()[i] << '\n';
					}
				}
				else
				{
					w1 = input;
					cin >> operation >> w2;
					cout << "RESULT:\n";

					if (operation == "&&")
					{
						cout << "Inversed Indexer:\n";
						for (auto&& w : and_query(dict, w1, w2))
						{
							cout << '\t' << dict.get_docids()[w] << '\n';
						}
						cout << "Incidence Matrix:\n";
						const auto& list = and_query(matr, w1, w2);
						for (size_t i = 0; i < list.size(); ++i)
						{
							if (list[i])
								cout << '\t' << matr.get_docids()[i] << '\n';
						}
					}
					else if (operation == "||")
					{
						cout << "Inversed Indexer:\n";
						for (auto&& w : or_query(dict, w1, w2))
						{
							cout << '\t' << dict.get_docids()[w] << '\n';
						}
						cout << "Incidence Matrix:\n";
						const auto& list = or_query(matr, w1, w2);
						for (size_t i = 0; i < list.size(); ++i)
						{
							if (list[i])
								cout << '\t' << matr.get_docids()[i] << '\n';
						}
					}
					else
					{
						throw std::runtime_error("No operation detected.");
					}
				}
			}
			catch (const std::exception& ex)
			{
				cout << "***Error: " << "no such a word found" << '\n';
			}
		} while (input != "!!!");
	}


	namespace boolean_search
	{
		void execute_boolean_search()
		{
			const std::string path = "Input Files/";
			boolean_search_test_func(path);
		}

		/*int execute_lab_2()
		{
			using std::cout;
			using std::cin;
			int input;
			do
			{
				cout << "1. Testing Lab2\n2. Testing Practice1\nYour choice: ";
				cin >> input;
				if (input == 1)
				{
					dictionary::execute_testing();
					break;
				}
				else if (input == 2)
				{
					execute_boolean_search();
					break;
				}
				cout << "***Error. No such a choice.\n";
			} while (input != 1 && input != 2);
			return 0;
		}*/
	}
}

#endif // !BOOLEAN_SEARCH_
