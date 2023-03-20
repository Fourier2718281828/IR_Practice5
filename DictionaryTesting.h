//#ifndef DICTIONARY_TESTING_
//#define DICTIONARY_TESTING_
//#include <iostream>
//#include <chrono>
//#include <filesystem>
//#include "ISerializer.h"
//#include "LineReader.h"
//#include "TextFileSerializer.h"
//#include "USetPolicy.h"
//#include "DefaultAddPolicy.h"
//#include "PairedVectorAdapted.h"
//#include "UMapAdapter.h"
//#include "VectorAdapted.h"
//#include "SetAdapted.h"
//#include "USetAdapted.h"
//#include "Dictionary.h"
//
//namespace testing
//{
//	using USetDict = OptimizedDitctionary
//		<
//		std::string,
//		AnySetPolicy,
//		USetAdapted
//		>;
//
//	using SetDict = OptimizedDitctionary
//		<
//		std::string,
//		AnySetPolicy,
//		SetAdapted
//		>;
//
//	using ArrayDict = OptimizedDitctionary
//		<
//		std::string,
//		ArrayPolicy,
//		VectorAdapted
//		>;
//
//	std::ifstream::pos_type filesize(const std::string& filename)
//	{
//		std::ifstream in(filename);
//		return in.tellg();
//	}
//
//	template<typename Dictionary>
//	bool check_for_equality(const Dictionary& dict1, const Dictionary& dict2)
//	{
//		using std::cout;
//
//		for (auto&& a : dict1)
//		{
//			if (!dict2.has_word(a))
//			{
//				cout << "No word: " << a << '\n';
//				return false;
//			}
//		}
//
//		for (auto&& a : dict2)
//		{
//			if (!dict1.has_word(a))
//			{
//				cout << "No word: " << a << '\n';
//				return false;
//			}
//		}
//
//		return true;
//	}
//
//	template<typename Dictionary>
//	void test_dict(const std::string& dirpath, const std::string& outpath)
//	{
//		using std::cout;
//		cout << "<------------------------------------------>\n";
//		cout << "Testing " << typeid(Dictionary::policy_type).name() << '\n';
//		cout << ">------------------------------------------<\n";
//		Dictionary dict;
//		IFiller<decltype(dict)>* reader = new LineReader<Dictionary>(" ,.;:!?*[]()\"\"0123456789/\\-");
//		try
//		{
//			size_t total_words = 0u;
//			auto file_collection_size = 2342u;
//			for (const auto& path : std::filesystem::directory_iterator(dirpath))
//			{
//				cout << "# \"" << path.path().string() << "\" processing...\n";
//				auto start = std::chrono::steady_clock::now();
//				total_words += reader->read(path.path().string(), dict);
//				auto end = std::chrono::steady_clock::now();
//				auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//				cout << "\tread in " << time1 << " ms\n";
//			}
//
//			cout << ">------------------------------------------<\n";
//
//			ISerializer<Dictionary>* ser;
//			if constexpr (std::is_same_v<Dictionary, ArrayDict>)
//			{
//				ser = new TextFileSerializer<ArrayDict>();
//			}
//			else if constexpr (std::is_same_v<Dictionary, SetDict>)
//			{
//				ser = new TextFileSerializer<SetDict>();
//			}
//			else
//			{
//				ser = new TextFileSerializer<USetDict>();
//			}
//			cout << "# Serialization processing...\n";
//			auto start = std::chrono::steady_clock::now();
//			ser->serialize(outpath, dict);
//			auto end = std::chrono::steady_clock::now();
//			auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//			cout << "\tdone in " << time1 << " ms\n";
//
//
//			cout << ">------------------------------------------<\n";
//			cout << "# Decerialization processing...\n";
//			auto start2 = std::chrono::steady_clock::now();
//			auto data = ser->deserialize(outpath);
//			auto end2 = std::chrono::steady_clock::now();
//			auto time12 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
//			cout << "\tdone in " << time12 << " ms\n";
//
//			cout << ">------------------------------------------<\n";
//			{
//				cout << "# Checking for equality of deceralized and original dict...\n";
//				auto start2 = std::chrono::steady_clock::now();
//				bool check = check_for_equality<Dictionary>(dict, data);
//				auto end2 = std::chrono::steady_clock::now();
//				auto time12 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
//				cout << "\tdone in " << time12 << " ms\n";
//				cout << "RESULT : " << std::boolalpha << check << '\n';
//			}
//			cout << ">------------------------------------------<\n";
//
//			cout << "STATISTICS:\n";
//			cout << "\tGeneral file collection size \t= " << file_collection_size << '\n';
//			cout << "\tGeneral number of words \t= " << total_words << '\n';
//			cout << "\tTotal size of produced dict \t= " << dict.size() << '\n';
//
//			cout << "<------------------------------------------>\n";
//
//			delete reader; reader = nullptr;
//			delete ser; ser = nullptr;
//		}
//		catch (std::exception& ex)
//		{
//			cout << "ERROR : " << ex.what() << '\n';
//		}
//	}
//
//	namespace dictionary
//	{
//		inline void execute_testing()
//		{
//			const std::string path = "Input Files/";
//			const std::string outputArray = "Output Files/outputArray.txt";
//			const std::string outputMap = "Output Files/outputMap.txt";
//			test_dict<SetDict>(path, outputMap);
//			test_dict<USetDict>(path, outputMap);
//			test_dict<ArrayDict>(path, outputArray);
//		}
//	}
//}
//
//#endif // !DICTIONARY_TESTING_
