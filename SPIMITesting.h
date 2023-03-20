#ifndef SPIMI_TESTING_
#define SPIMI_TESTING_
#include <iostream>
#include <filesystem>
#include "SPIMIBlock.h"
#include "SPIMIFiller.h"
#include "LineFileReader.h"


namespace testing
{
	namespace spimi
	{
		SPIMIBlocks build_blocks(const filename_t& folder, const size_t block_size, const size_t word_gen_limit)
		{
			using std::cout;
			using reader_ptr = typename SPIMIBlock::reader_ptr;

			std::vector<reader_ptr> readers{};
			SPIMIBlocks res{};

			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				reader_ptr reader = std::make_shared<LineFileReader<>>(path.path().string());
				readers.push_back(reader);
			}

			SPIMIFiller filler(readers, block_size, word_gen_limit);
			while (!filler.is_over())
			{
				filler.read(res);
				break;
				filler.refresh();
				cout << "res.size: " << res.size() << '\n';
			}
			return res;
		}

		void execute_testing()
		{
			const size_t block_size = 4;
			const size_t word_gen_limit = 100;
			auto blocks = build_blocks("Input Files/i/", block_size, word_gen_limit);
		}
	}
}

#endif // !SPIMI_TESTING_
