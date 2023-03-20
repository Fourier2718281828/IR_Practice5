#ifndef INVERTED_INDEX_SERIALIZER_
#define INVERTED_INDEX_SERIALIZER_

#include "TextFileSerializer.h"
#include "DocumentIndexer.h"

template
<
	typename FOutStream, 
	typename FInStream,
	typename WordType,
	typename DocNameType,
	template<typename, typename> typename ContainerType,
	template<typename> typename ListType,
	typename PolicyType
>
class TextFileSerializer
	<
		FOutStream, 
		FInStream, 
		DocumentIndexer
		<
			WordType,
			DocNameType,
			ContainerType,
			ListType,
			PolicyType
		>
	> 
	: public ISerializer
	<
		FOutStream, 
		FInStream, 
		DocumentIndexer
		<
			WordType,
			DocNameType,
			ContainerType,
			ListType,
			PolicyType
		>
	>
{
	using Serializable = DocumentIndexer
		<
			WordType,
			DocNameType,
			ContainerType,
			ListType,
			PolicyType
		>;
public:
	void serialize(FOutStream& fout, const Serializable& dict) const override
	{
		if (fout.is_open())
		{
			using dict_type = typename Serializable::dict_type;
			using filename_idmapper_type = typename Serializable::filename_idmapper_type;
			using mapper_size_type = typename Serializable::mapped_type;

			const filename_idmapper_type& ids = dict.get_docids();

			fout << ids.size() << '\n';
			for (mapper_size_type i = 0u; i < ids.size(); ++i)
			{
				text_serialize_string(fout, ids[i]);
			}

			TextFileSerializer<FOutStream, FInStream, dict_type> dict_ser;
			dict_ser.serialize(fout, dict.get_dict());
		}
		else
		{
			throw std::runtime_error("Only open stream should be passed");
		}
	}

	Serializable deserialize(FInStream& fin) const override
	{
		if (fin.is_open())
		{
			using dict_type = typename Serializable::dict_type;
			using filename_idmapper_type = typename Serializable::filename_idmapper_type;
			using mapper_size_type = typename Serializable::mapped_type;

			mapper_size_type mapper_size;
			filename_idmapper_type mapper;

			fin >> mapper_size;
			mapper.reserve(mapper_size);
			text_deserialize_string(fin);

			for (mapper_size_type i = 0u; i < mapper_size; ++i)
			{
				mapper.push_back(text_deserialize_string(fin));
			}

			TextFileSerializer<FOutStream, FInStream, dict_type> dict_ser;
			dict_type deserialized = dict_ser.deserialize(fin);
			return { std::move(deserialized), std::move(mapper) };
		}
		else
		{
			throw std::runtime_error("Only open stream should be passed");
		}
	}
};


#endif // !INVERTED_INDEX_SERIALIZER_
