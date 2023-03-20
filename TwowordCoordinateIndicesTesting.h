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

namespace testing
{
	namespace two_word_index
	{
		//void serialize_two_word(const TwoWordInvertedIndexer& trie, const std::string& filename)
		//{
		//	using std::cout;
		//	using std::cin;
		//	//TextFileSerializer<std::ofstream, std::ifstream, TwoWordInvertedIndexer> serializer;
		//	std::ofstream file("Output Files/" + filename);
		//	if (!file.is_open())
		//	{
		//		cout << "could not open the file\n";
		//		return;
		//	}
		//	cout << "Serializing...\n";
		//	//serializer.serialize(file, trie);
		//	cout << "Serialization finished.\n";
		//}

		void prepare_index(const std::string folder)
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

			//serialize_two_word(dict, "TwoWordIndexer.txt");
		}

		void execute_testing()
		{
			const std::string path = "Input Files/";
			prepare_index(path);
		}
	}

	namespace coordinate_index
	{
		/*void serialize_coord(const CoordinateInvertedIndexer& trie, const std::string& filename)
		{
			using std::cout;
			using std::cin;
			TextFileSerializer<std::ofstream, std::ifstream, CoordinateInvertedIndexer> serializer;
			std::ofstream file("Output Files/" + filename);
			if (!file.is_open())
			{
				cout << "could not open the file\n";
				return;
			}
			cout << "Serializing...\n";
			serializer.serialize(file, trie);
			cout << "Serialization finished.\n";
		}*/

		void prepare_index(const std::string folder)
		{
			using std::cout;
			CoordinateInvertedIndexer dict;

			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				std::shared_ptr<IFileReader<std::string>> reader =
					std::make_shared<LineFileReader<Encodings::ANSI>>(path.path().string());

				CoordinateReader filler(reader);
				filler.read(dict);
			}

			//serialize_coord(dict, "CoordinateIndex.txt");
		}

		void execute_testing()
		{
			const std::string path = "Input Files/";
			prepare_index(path);
		}
	}
}

#endif // !TW_RD_COORD_TESTING_
