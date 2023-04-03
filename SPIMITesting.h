#ifndef SPIMI_TESTING_
#define SPIMI_TESTING_
#include <iostream>
#include <filesystem>
#include <queue>
#include <functional>
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

		void recursive_traverse_files(const filename_t& folder, std::function<void(const filename_t&)> func)
		{
			for (const auto& path : std::filesystem::directory_iterator(folder))
			{
				if (std::filesystem::is_directory(path))
				{
					recursive_traverse_files(folder, func);
				}
				else
				{
					func(path.path().string());
				}
			}
		}

		void prepare_blocks(const filename_t& folder, const filename_t& out_folder, const filename_t& index_path,
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
			

			recursive_traverse_files
			(
				folder,
				[&readers](const filename_t& path) 
				{
					reader_ptr reader = std::make_shared<LineFileReader<>>(path);
					readers.push_back(reader);
				}
			);

			size_t words_count = 0u;
			SPIMIFiller filler(readers, block_size, word_gen_limit);
			while (!filler.is_over())
			{
				words_count += filler.read(blocks);
				if (filler.reached_limit() || filler.is_over())
				{
					filler.refresh();
					if (blocks.empty()) break;
					serializer_type serializer(blocks[0].get_indexer().get_docids_ptr(), index_path);
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

		std::pair<std::string, std::vector<size_t>> read_line(std::ifstream& fin)
		{
			std::string word;
			size_t list_size;
			std::vector<size_t> list{};
			size_t next_list_node;

			fin >> word;
			fin >> list_size;

			for (size_t i = 0u; i < list_size; ++i)
			{
				fin >> next_list_node;
				list.push_back(next_list_node);
			}

			return { std::move(word), std::move(list)};
		}

		void put_line_to_index(const std::string& word, const std::vector<size_t>& index, std::ofstream& fout)
		{
			fout << word << ' ' << index.size() << ' ';
			for (auto&& i : index)
			{
				fout << i << ' ';
			}
			fout << '\n';
		}

		std::vector<size_t> merge(const std::vector<size_t>& v1, const std::vector<size_t>& v2) {
			std::vector<size_t> result;
			size_t i = 0;
			size_t j = 0;

			while (i < v1.size() && j < v2.size()) {
				if (v1[i] < v2[j]) {
					result.push_back(v1[i]);
					i++;
				}
				else if (v1[i] > v2[j]) {
					result.push_back(v2[j]);
					j++;
				}
				else {
					result.push_back(v1[i]);
					i++;
					j++;
				}
			}

			while (i < v1.size()) {
				result.push_back(v1[i]);
				i++;
			}

			while (j < v2.size()) {
				result.push_back(v2[j]);
				j++;
			}

			return result;
		}


		void build_index(const filename_t& blocks_folder, const filename_t& output_file)
		{
			std::ofstream fout(output_file);
			if (!fout.is_open())
				throw std::runtime_error("Cannot create index file.");

			std::vector<std::ifstream> files{};

			for (const auto& block_path : std::filesystem::directory_iterator(blocks_folder))
			{
				files.emplace_back(block_path.path().string());
				if (!files.back().is_open())
					throw std::runtime_error("Cannot open file: " + block_path.path().string());
			}

			bool is_optimized;
			size_t block_size;

			std::vector<std::string> curr_words{};
			std::vector<std::vector<size_t>> curr_lists{};
			std::vector<size_t> file_sizes{};
			curr_lists.reserve(files.size());
			file_sizes.reserve(files.size());
			size_t open_files_left = files.size();

			for (size_t i = 0; i < files.size(); ++i)
			{
				auto& file = files[i];
				file >> is_optimized;
				file >> block_size;
				auto&& [word, list] = read_line(file);
				file_sizes.push_back(block_size);
				curr_words.push_back(std::move(word));
				curr_lists.push_back(std::move(list));
			}
			bool fuck = std::string("aword") < std::string("bword");
			auto find_min = [&curr_words]()
			{
				std::string min = curr_words[0];
				for (size_t i = 1; i < curr_words.size(); ++i)
				{
					if (!curr_words[i].empty() && (min.empty() || min > curr_words[i]))
						min = curr_words[i];
				}

				return min;
			};

			while (open_files_left > 0u)
			{
				std::string min_word = find_min();
				std::vector<size_t> min_word_indices{};

				for (size_t i = 0u; i < files.size(); ++i)
				{
					if (curr_words[i] == min_word && file_sizes[i] > 0)
					{
						min_word_indices = merge(min_word_indices, curr_lists[i]);

						--file_sizes[i];
						if (file_sizes[i] == 0u)
						{
							curr_words[i] = "";
							open_files_left -= 1;
							continue;
						}

						auto&& [word, list] = read_line(files[i]);
						curr_words[i] = std::move(word);
						curr_lists[i] = std::move(list);

					}
				}
				put_line_to_index(min_word, min_word_indices, fout);
			} 

			for (auto&& file : files)
			{
				file.close();
			}
		}

		void execute_testing()
		{
			const size_t block_size = 5000;
			const size_t word_gen_limit = 30000;
			const filename_t in_folder = "Input Files/";
			const filename_t out_folder = "Output Files/SPIMI/";
			const filename_t out_folder_blocks = out_folder + "Blocks/";
			
			prepare_blocks(in_folder, out_folder_blocks, out_folder + "idmapper.txt", block_size, word_gen_limit);
			build_index(out_folder_blocks, out_folder + "index.txt");
		}
	}
}

#endif // !SPIMI_TESTING_
