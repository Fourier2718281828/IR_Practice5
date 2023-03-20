#ifndef INDEXERS_TESTING_
#define INDEXERS_TESTING_
#include <iostream>
#include "InvertedIndexer.h"
#include <chrono>
#include <filesystem>
#include "ISerializer.h"
#include "LineReader.h"
#include "TextFileSerializer.h"
#include "USetPolicy.h"
#include "DefaultAddPolicy.h"
#include "PairedVectorAdapted.h"
#include "UMapAdapter.h"
#include "MapAdapted.h"
#include "VectorAdapted.h"
#include "SetAdapted.h"
#include "USetAdapted.h"
#include "Dictionary.h"
#include "DocumentIndexerSerializer.h"
#include "IncidenceMatrix.h"

namespace testing
{
	template<typename Indexer>
	bool check_for_equality_unoptimized(const Indexer& dict1, const Indexer& dict2)
	{
		for (auto&& [a, _] : dict1.get_dict())
		{
			if (!dict2.get_dict().has_word(a) && a != "")
			{
				return false;
			}
		}

		for (auto&& [a, _] : dict2.get_dict())
		{
			if (!dict1.get_dict().has_word(a) && a != "")
			{
				return false;
			}
		}

		return true;
	}

	template<typename Dictionary>
	void test(const std::string& dirpath, const std::string& outpath)
	{
		using std::cout;
		cout << "<------------------------------------------>\n";
		cout << "Testing " << typeid(Dictionary::dict_type::policy_type).name() << '\n';
		cout << ">------------------------------------------<\n";
		Dictionary dict;
		try
		{
			size_t total_words = 0u;
			for (const auto& path : std::filesystem::directory_iterator(dirpath))
			{
				IFiller<decltype(dict)>* reader = new LineReader<Dictionary>(path.path().string(), " ,.;:!?*[]()\"\"0123456789/'{}|@#$&=%\\-");
				cout << "# \"" << path.path().string() << "\" processing...\n";
				auto start = std::chrono::steady_clock::now();
				total_words += reader->read(dict);
				auto end = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				cout << "\tread in " << time1 << " ms\n";
				delete reader; reader = nullptr;
			}

			cout << ">------------------------------------------<\n";

			ISerializer<std::ofstream, std::ifstream, Dictionary>* ser
				= new TextFileSerializer<std::ofstream, std::ifstream, Dictionary>();

			std::ofstream fon(outpath);
			try
			{
				cout << "# Serialization processing...\n";
				auto start = std::chrono::steady_clock::now();
				ser->serialize(fon, dict);
				auto end = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				cout << "\tdone in " << time1 << " ms\n";
			}
			catch (const std::runtime_error&)
			{
				cout << "Could not open the file: " << outpath << '\n';
			}
			fon.close();

			cout << ">------------------------------------------<\n";

			std::ifstream fin(outpath);
			Dictionary* deserialized = nullptr;
			try
			{
				cout << "# Decerialization processing...\n";
				auto start2 = std::chrono::steady_clock::now();
				deserialized = new Dictionary(ser->deserialize(fin));
				auto end2 = std::chrono::steady_clock::now();
				auto time12 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
				cout << "\tdone in " << time12 << " ms\n";
			}
			catch (const std::runtime_error&)
			{
				cout << "Could not open the file: " << outpath << '\n';
			}
			fin.close();

			cout << ">------------------------------------------<\n";
			{
				cout << "# Checking for equality of deceralized and original dict...\n";
				auto start2 = std::chrono::steady_clock::now();
				bool check = deserialized ? check_for_equality_unoptimized(dict, *deserialized) : false;
				auto end2 = std::chrono::steady_clock::now();
				auto time12 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
				cout << "\tdone in " << time12 << " ms\n";
				cout << "RESULT : " << std::boolalpha << check << '\n';
			}
			cout << ">------------------------------------------<\n";

			cout << "STATISTICS:\n";
			cout << "\tGeneral number of words \t= " << total_words << '\n';
			cout << "\tTotal size of produced dict \t= " << dict.size() << '\n';

			cout << "<------------------------------------------>\n";

			delete ser; ser = nullptr;
		}
		catch (std::exception& ex)
		{
			cout << "ERROR : " << ex.what() << '\n';
		}
	}

	namespace indexers
	{
		inline void execute_testing()
		{
			const std::string path = "Input Files/";
			const std::string outputMapMat = "Output Files/matrix.txt";
			const std::string outputMapInd = "Output Files/index.txt";
			test<IncidenceMatrix>(path, outputMapMat);
			test<InvertedIndexer>(path, outputMapInd);
		}
	}
}


#endif // !INDEXERS_TESTING_
