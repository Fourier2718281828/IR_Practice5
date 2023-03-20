#ifndef I_SERIALIZER_
#define I_SERIALIZER_

template<typename FOutStream, typename FInStream, typename Serializable>
class ISerializer
{
public:
	virtual ~ISerializer() = default;
public:
	virtual void serialize(FOutStream&, const Serializable&)	const = 0;
	virtual Serializable deserialize(FInStream&)				const = 0;
};

#endif // !I_SERIALIZER_
