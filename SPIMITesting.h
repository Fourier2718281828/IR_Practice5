#ifndef SPIMI_TESTING_
#define SPIMI_TESTING_
#include <iostream>
#include <filesystem>
#include "SPIMIBlock.h"
#include "SPIMIFiller.h"
#include "LineFileReader.h"
#include "DocumentIndexerSerializer.h"

namespace testing
{
	namespace spimi
	{
		filename_t next_filename(const filename_t out_folder)
		{
			static std::vector<int> block_names{1};
			/*if (block_names.empty())
			{
				for (const auto& path : std::filesystem::directory_iterator(out_folder))
				{
					filename_t nxt = path.path().string();
					filename_t base_name{};
					nxt.pop_back();
					nxt.pop_back();
					nxt.pop_back();
					nxt.pop_back();
					while (nxt[nxt.size() - 1] != '/')
					{
						base_name.push_back(nxt.back());
						nxt.pop_back();
					}
					block_names.push_back(std::stoi(base_name));
				}
			}*/

			/*if (block_names.empty())
			{
				return out_folder + "1.txt";
			}*/

			auto max = block_names.back();//*std::max_element(block_names.begin(), block_names.end());
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
			TextFileSerializer<std::ofstream, std::ifstream, typename SPIMIBlock::indexer_type> serializer;
			

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
					for (auto&& block : blocks)
					{
						auto name = next_filename(out_folder);
						cout << "name = " << name << '\n';
						std::ofstream fout(name);
						if (!fout.is_open())
							throw "out folder not open";
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
			prepare_blocks("Input Files/", "Output Files/SPIMI/Blocks/", block_size, word_gen_limit);
		}
	}
}

#endif // !SPIMI_TESTING_
