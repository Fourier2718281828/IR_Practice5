#ifndef SINGLE_DOCMAPPER_DOCINDEXER_SERIALIZER_
#define SINGLE_DOCMAPPER_DOCINDEXER_SERIALIZER_
#include "TextFileSerializer.h"
#include "DocumentIndexer.h"

template
<
	typename FOutStream,
	typename FInStream,
	typename Serializable
>
class SingleIDMapperDocIndexerSerializer : public ISerializer<FOutStream, FInStream, Serializable>
{
public:
	using idmapper_type = typename Serializable::filename_idmapper_type;
	using idmapper_ptr = typename Serializable::idmapper_ptr;
public:

	explicit constexpr SingleIDMapperDocIndexerSerializer
	(
		idmapper_ptr idmapper,
		const filename_t& idmapper_filename
	) :
		idmapper_{ idmapper },
		idmapper_filename_(idmapper_filename)
	{
		if (idmapper == nullptr) return;

		FOutStream fout(idmapper_filename);
		if (fout.is_open())
		{
			using mapper_size_type = typename Serializable::mapped_type;

			const idmapper_type& ids = *idmapper;

			fout << ids.size() << '\n';
			for (mapper_size_type i = 0u; i < ids.size(); ++i)
			{
				text_serialize_string(fout, ids[i]);
			}
		}
		else
		{
			fout.close();
			throw std::runtime_error("Impossible to open or create file: " + idmapper_filename);
		}
		fout.close();
	}

public:
	void serialize(FOutStream& fout, const Serializable& dict) const override
	{
		if (idmapper_ == nullptr)
			throw std::runtime_error("Impossible to serialize: nullptr common idmapper");
		if (fout.is_open())
		{
			using dict_type = typename Serializable::dict_type;
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
		using dict_type = typename Serializable::dict_type;
		using filename_idmapper_type = typename Serializable::filename_idmapper_type;
		using mapper_size_type = typename Serializable::mapped_type;

		FInStream idfin(idmapper_filename_);
		if (idmapper_ == nullptr && idfin.is_open())
		{
			mapper_size_type mapper_size;
			idmapper_ = std::make_shared<typename idmapper_ptr::element_type>();

			idfin >> mapper_size;
			idmapper_->reserve(mapper_size);
			text_deserialize_string(idfin);

			for (mapper_size_type i = 0u; i < mapper_size; ++i)
			{
				idmapper_->push_back(text_deserialize_string(idfin));
			}
		}
		else if(idmapper_ != nullptr)
		{
			idfin.close();
			throw std::runtime_error("Impossible to open file: " + idmapper_filename_);
		}
		idfin.close();

		if (fin.is_open())
		{
			TextFileSerializer<FOutStream, FInStream, dict_type> dict_ser;
			dict_type deserialized = dict_ser.deserialize(fin);
			return { std::move(deserialized), idmapper_ };
		}
		else
		{
			throw std::runtime_error("Only open stream should be passed");
		}
	}

private:
	mutable idmapper_ptr idmapper_;
	const filename_t idmapper_filename_;
};

#endif // !SINGLE_DOCMAPPER_DOCINDEXER_SERIALIZER_
