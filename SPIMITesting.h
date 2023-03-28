#ifndef SPIMI_TESTING_
#define SPIMI_TESTING_
#include <iostream>
#include <filesystem>
#include "SPIMIBlock.h"
#include "SPIMIFiller.h"
#include "LineFileReader.h"
#include "DocumentIndexerSerializer.h"
#include "SingleIDMapperDocIndexerSerializer.h"

namespace testing
{
	namespace spimi
	{
		filename_t next_filename(const filename_t out_folder)
		{
			static std::vector<int> block_names{1};
			auto max = block_names.back();
			block_names.push_back(max + 1);
			return out_folder + std::to_string(max) + ".txt";
		}
		void prepare_blocks(const filename_t& folder, const filename_t out_folder, 
			const size_t block_size, const size_t word_gen_limit)
		{
			using std::cout;
			using reader_ptr = typename SPIMIBlock::reader_ptr;

			std::vector<reader_ptr> readers{};
			SPIMIBlocks blocks{};
			using serializer_type = SingleIDMapperDocIndexerSerializer
				<
				std::ofstream, 
				std::ifstream, 
				typename SPIMIBlock::indexer_type
				>;
			

			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				reader_ptr reader = std::make_shared<LineFileReader<>>(path.path().string());
				readers.push_back(reader);
			}

			size_t words_count = 0u;
			SPIMIFiller filler(readers, block_size, word_gen_limit);
			while (!filler.is_over())
			{
				words_count += filler.read(blocks);
				if (filler.reached_limit() || filler.is_over())
				{
					filler.refresh();
					if (blocks.empty()) break;
					serializer_type serializer(blocks[0].get_indexer().get_docids_ptr(), out_folder + "index.txt");
					for (auto&& block : blocks)
					{
						auto name = next_filename(out_folder);
						cout << "name = " << name << '\n';
						std::ofstream fout(name);
						if (!fout.is_open())
							throw std::runtime_error("out folder not open");
						serializer.serialize(fout, block.get_indexer());
					}
					blocks.clear();
					cout << "limit\n";
				}
			}

			cout << "words read: " << words_count << '\n';
		}

		void execute_testing()
		{
			const size_t block_size = 5000;
			const size_t word_gen_limit = 30000;
			const filename_t in_folder = "Input Files/";
			const filename_t out_folder = "Output Files/SPIMI/Blocks/";
			prepare_blocks(in_folder, out_folder, block_size, word_gen_limit);
			
			SingleIDMapperDocIndexerSerializer
				<
				std::ofstream,
				std::ifstream,
				typename SPIMIBlock::indexer_type
				> serializer(nullptr, out_folder + "index.txt");

			std::ifstream fin(out_folder + "6.txt");
			auto block = serializer.deserialize(fin);
		}
	}
}

#endif // !SPIMI_TESTING_
