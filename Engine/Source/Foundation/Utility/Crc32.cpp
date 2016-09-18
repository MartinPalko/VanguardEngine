#include "Crc32.h"

#include "Types/VanguardString.h"
#include "FileSystem.h"

#define HARDCODE_TABLE

namespace Vanguard
{
		uint32 Crc32::CalcCrc(const byte* aData, size_t aDataLength)
		{
			uint32 out = 0xffffffff; //Initilaize the CRC.
			while (aDataLength--)
			{
				out = (out >> 8) ^ crcTable[(out & 0xFF) ^ *aData++];
			}
			return(out ^ 0xffffffff); //Finalize the CRC and return.
		}
}