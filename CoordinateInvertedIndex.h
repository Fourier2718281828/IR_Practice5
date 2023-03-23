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

#include <iostream>

std::map<size_t, std::set<coordinate_t>> distinguish(const CoordMapAdapted<std::pair<size_t, size_t>>& coord_map)
{
	std::map<size_t, std::set<coordinate_t>> res;
	for (auto&& a : coord_map)
	{
		res.insert({ a.first, a.second });
	}
	return res;
}


std::map<size_t, std::set<coordinate_t>> intersect_pred
(
	const std::map<size_t, std::set<coordinate_t>>& p1,
	const std::map<size_t, std::set<coordinate_t>>& p2
)
{
	std::map<size_t, std::set<coordinate_t>> res{};
	auto it1 = p1.begin();
	auto it2 = p2.begin();

	while (it1 != p1.end() && it2 != p2.end())
	{
		if (it1->first == it2->first)
		{
			res.insert(*it1);
			++it1;
			++it2;
		}
		else if (it1->first < it2->first)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return res;
}

using SearchResult = std::set<size_t>;

void find_all_sequences_rec
(
	const std::set<coordinate_t>& fset,
	typename std::vector<std::set<coordinate_t>>::const_iterator stor,
	typename std::vector<std::set<coordinate_t>>::const_iterator etor,
	typename std::vector<size_t>::const_iterator dist_itor,
	const std::vector<coordinate_t>& prefix,
	std::vector<std::vector<coordinate_t>>& res
)
{
	if (stor == etor)
	{
		res.push_back(prefix);
		return;
	}

	const auto& sset = *stor;
	for (const auto coord : fset)
	{
		const auto min_distance = coord + 1;
		const auto max_distance = *dist_itor;
		auto lower_bound_itor = sset.lower_bound(min_distance);
		std::set<coordinate_t> next_fset{};

		while (lower_bound_itor != sset.end() && *lower_bound_itor - coord <= max_distance)
		{
			next_fset.insert(*lower_bound_itor);
			++lower_bound_itor;
		}

		for (const auto next_coord : next_fset)
		{
			auto nxt_prefix = prefix;
			nxt_prefix.push_back(next_coord);

			find_all_sequences_rec
			(
				next_fset,
				stor + 1,
				etor,
				dist_itor + 1,
				nxt_prefix,
				res
			);
		}
	}
}

std::vector<std::vector<coordinate_t>> find_all_sequences
(
	const std::vector<std::set<coordinate_t>>& coords,
	const std::vector<size_t>& distances
)
{
	if (coords.size() != distances.size() + 1)
		throw std::runtime_error("Incompatable sizes of coords and distances.");
	if (coords.size() == 1)
		return { coords[0].begin(), coords[0].end()};

	const auto& fset = coords[0];
	std::vector<std::vector<coordinate_t>> res{};

	find_all_sequences_rec
	(
		fset,
		coords.begin() + 1,
		coords.end(),
		distances.begin(),
		{},
		res
	);

	return res;
}


SearchResult distance_search
(
	const CoordinateInvertedIndexer& indexer,
	const std::vector<std::string>& words,
	const std::vector<size_t>& distances
)
{
	using value_type = std::pair<size_t, size_t>;
	using index = std::map<size_t, std::set<coordinate_t>>;
	std::vector<index> indices;
	for (auto&& word : words)
	{
		indices.push_back(distinguish(indexer.get_list(word)));
	}
	if (indices.empty()) return {};
	auto intersected = indices[0];
	for (size_t i = 1u; i < indices.size(); ++i)
	{
		intersected = intersect_pred
		(
			intersected,
			indices[i]
		);
	}

	SearchResult res{};

	for (auto&& [doc, _] : intersected)
	{
		std::vector<std::set<coordinate_t>> coords;

		for (auto&& index : indices)
		{
			coords.push_back(index.at(doc));
		}
		
		auto all_sequences = find_all_sequences(coords, distances);
		
		if (!all_sequences.empty())
		{
			res.insert(doc);
		}
	}

	return res;
}

#endif // !COORDINATE_INVERTED_INDEX_
