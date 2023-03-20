#ifndef DOCUMENT_INDEXER_
#define DOCUMENT_INDEXER_
#include "Dictionary.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "BitVectorAdapted.h"

template
<
	typename WordType,
	typename DocNameType,
	template<typename, typename> typename ContainerType,
	template<typename> typename ListType,
	typename PolicyType
>
class DocumentIndexer
{
public:
	using word_type = WordType;
	using filename_type = DocNameType;
	using filename_idmapper_type = std::vector<filename_type>;
	using mapped_type = typename filename_idmapper_type::size_type;
public:
	using policy_type = PolicyType;
	template<typename T, typename U>
	using container_type = ContainerType<T, U>;
	template<typename T>
	using dictlist_type = ListType<T>;
public:
	using dict_type = UnoptimizedDict
		<
		word_type,
		mapped_type,
		policy_type,
		container_type,
		dictlist_type
		>;
public:

	DocumentIndexer() noexcept {};

	DocumentIndexer(dict_type&& dict, filename_idmapper_type&& mapper) noexcept :
		_dict(std::move(dict)), _doc_mapper(std::move(mapper))
	{}

	DocumentIndexer(const DocumentIndexer&) = delete;

	DocumentIndexer(DocumentIndexer&&) = default;

	~DocumentIndexer() = default;

public:

	DocumentIndexer& operator=(const DocumentIndexer&) = delete;

	DocumentIndexer& operator=(DocumentIndexer&&) = default;

public:

	template<typename WW, typename FF>
	void add_word(WW&& word, FF&& filename)
	{
		if (!contains_docname(filename))
		{
			_dict.add_word(std::forward<WW>(word), _doc_mapper.size());
			_doc_mapper.push_back(std::forward<FF>(filename));
		}
		else
		{
			_dict.add_word(std::forward<WW>(word), get_docId(filename));
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

	void stabilize_lists()
	{
		if constexpr (std::is_same_v<typename dict_type::list_type, BitVectorAdapted<mapped_type>>)
		{
			for (auto&& [_, list] : _dict)
			{
				if (list.size() != _doc_mapper.size())
					list.resize(_doc_mapper.size());
			}
		}
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

#endif // !DOCUMENT_INDEXER_
