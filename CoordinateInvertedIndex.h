#ifndef COORDINATE_INVERTED_INDEX_
#define COORDINATE_INVERTED_INDEX_
#include "DocumentIndexer.h"
#include <string>
#include "MapAdapted.h"
#include "CoordMapAdapted.h"
#include "USetPolicy.h"
#include "FileTypes.h"


class CoordinateInvertedIndexer
{
public:
	using word_type = std::string;
	using filename_type = std::string;
	using filename_idmapper_type = std::vector<filename_type>;
	using mapped_type = typename filename_idmapper_type::size_type;
public:
	using policy_type = AnySetPolicy;
	template<typename T, typename U>
	using container_type = MapAdapted<T, U>;
	template<typename T>
	using dictlist_type = CoordMapAdapted<T>;
public:
	using dict_type = UnoptimizedDict
		<
		word_type,
		std::pair<mapped_type, coordinate_t>,
		policy_type,
		container_type,
		dictlist_type
		>;
public:

	CoordinateInvertedIndexer() noexcept {};

	CoordinateInvertedIndexer(dict_type&& dict, filename_idmapper_type&& mapper) noexcept :
		_dict(std::move(dict)), _doc_mapper(std::move(mapper))
	{}

	CoordinateInvertedIndexer(const CoordinateInvertedIndexer&) = delete;

	CoordinateInvertedIndexer(CoordinateInvertedIndexer&&) = default;

	~CoordinateInvertedIndexer() = default;

public:

	CoordinateInvertedIndexer& operator=(const CoordinateInvertedIndexer&) = delete;

	CoordinateInvertedIndexer& operator=(CoordinateInvertedIndexer&&) = default;

public:

	template<typename WW, typename FF, typename Pos>
	void add_word(WW&& word, FF&& filename, Pos&& position)
	{
		if (!contains_docname(filename))
		{
			std::pair<mapped_type, coordinate_t> pair(_doc_mapper.size(), std::forward<Pos>(position));
			_dict.add_word(std::forward<WW>(word), std::move(pair));
			_doc_mapper.push_back(std::forward<FF>(filename));
		}
		else
		{
			std::pair<mapped_type, coordinate_t> pair(get_docId(filename), std::forward<Pos>(position));
			_dict.add_word(std::forward<WW>(word), pair);
		}
	}

	size_t size() const noexcept
	{
		return _dict.size();
	}

	const dict_type& get_dict() const noexcept
	{
		return _dict;
	}

	const filename_idmapper_type& get_docids() const noexcept
	{
		return _doc_mapper;
	}

	mapped_type get_docId(const filename_type& filename) const
	{
		auto docid = find(filename);
		if (docid == _doc_mapper.cend())
			throw std::runtime_error("No such a filename");
		return docid - _doc_mapper.begin();
	}

	const typename dict_type::list_type& get_list(const word_type& word) const
	{
		return get_dict()[word];
	}

private:

	bool contains_docname(const filename_type& filename) const noexcept
	{
		return find(filename) != _doc_mapper.end();
	}

	typename filename_idmapper_type::const_iterator find(const filename_type& filename) const noexcept
	{
		return std::find_if
		(
			_doc_mapper.begin(),
			_doc_mapper.end(),
			[&filename](const auto& pair) { return pair == filename; }
		);
	}

private:
	dict_type _dict;
	filename_idmapper_type _doc_mapper;
};

using IterableContainer = typename CoordinateInvertedIndexer::filename_idmapper_type;

IterableContainer search_for_phrase
(
	const CoordinateInvertedIndexer& indexer,
	const std::vector<std::string>& words,
	const std::vector<size_t>& distances
)
{
	if (words.empty())
		throw std::runtime_error("There must be at least one imput word.");
	if (words.size() != distances.size() + 1)
		throw std::runtime_error("Number parameters count mismatch.");

	IterableContainer resfiles = indexer.get_docids();
	std::vector<size_t> toEraseIndexes{};
	for (typename CoordinateInvertedIndexer::mapped_type i = 0u; i < resfiles.size(); ++i)
	{
		std::set<coordinate_t> positions;
		const auto& wpos1 = indexer.get_list(words[0]);
		const auto& wpos2 = indexer.get_list(words[1]);
		const size_t distance = distances[0];
		const auto& coords1 = wpos1.get(i);
		const auto& coords2 = wpos2.get(i);
		for (auto&& coord1 : coords1)
		{
			for (auto&& coord2 : coords2)
			{
				auto c1 = static_cast<long>(coord1);
				auto c2 = static_cast<long>(coord2);
				if (std::abs(c1 - c2) <= distance)
				{
					positions.insert(c2);
				}
			}
		}

		for (size_t j = 1u; j < words.size(); ++j)
		{
			const auto& wpos1 = indexer.get_list(words[j]);
			const auto& wpos2 = indexer.get_list(words[j+1]);
			const size_t distance = distances[j];
			std::set<coordinate_t> coords1(std::move(positions));
			const auto& coords2 = wpos2.get(i);
			std::set<coordinate_t> positions;
			for (auto&& coord1 : coords1)
			{
				for (auto&& coord2 : coords2)
				{
					auto c1 = static_cast<long>(coord1);
					auto c2 = static_cast<long>(coord2);
					if (std::abs(c1 - c2) <= distance)
					{
						positions.insert(c2);
					}
				}
			}
			if (positions.empty())
				toEraseIndexes.push_back(i);
		}
	}
	return resfiles;
}

#endif // !COORDINATE_INVERTED_INDEX_
