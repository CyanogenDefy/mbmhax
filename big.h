#ifndef __BIG_H__
#define __BIG_H__

#include <stdint.h>
#include <stdlib.h>

#pragma pack(push, 1)
class big_uint16_t
{
	uint16_t value;

public:
	big_uint16_t() {}

	big_uint16_t(uint16_t value)
	{
		this->value = _byteswap_ushort(value);
	}

	uint16_t operator=(uint16_t value)
	{
		this->value = _byteswap_ushort(value);
		return value;
	}

	operator uint16_t()
	{
		return _byteswap_ushort(this->value);
	}
};

class big_uint32_t
{
	uint32_t value;

public:
	big_uint32_t() {}

	big_uint32_t(uint32_t value)
	{
		this->value = _byteswap_ulong(value);
	}

	uint32_t operator=(uint32_t value)
	{
		this->value = _byteswap_ulong(value);
		return value;
	}

	operator uint32_t()
	{
		return _byteswap_ulong(this->value);
	}
};
#pragma pack(pop)

#endif