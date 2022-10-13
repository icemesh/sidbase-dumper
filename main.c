/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

inline uint32_t swapU32(uint32_t value);

uint32_t swapU32(uint32_t value)
{
	return ((value & 0x000000FF) << 24)
		 | ((value & 0x0000FF00) << 8)
		 | ((value & 0x00FF0000) >> 8)
		 | ((value & 0xFF000000) >> 24);
}

int main(int argc, const char* aArgv[])
{
	if(argc < 2 || argc > 2)
	{
		printf("Usage: ./main sidbase.bin\n");
		return 0;
	}
	else
	{
		FILE* fh = fopen(aArgv[1], "rb");
		if(!fh)
		{
			printf("Could not open %s! - Does it even exist?\n", aArgv[1]);
			return -1;
		}
		else
		{
			fseek(fh, 0x0, SEEK_END);
			size_t fsize = ftell(fh);
			fseek(fh, 0x0, SEEK_SET);
			uint8_t* pMem = (uint8_t*)malloc(fsize);
			if(!pMem)
			{
				printf("Failed to allocate 0x%08X bytes\n", fsize);
				fclose(fh);
				return -1;
			}
			else
			{
				fread(pMem, 0x1, fsize, fh);
				fclose(fh);
				FILE* dumpHandle = fopen("sidbase.txt", "w");
				if(dumpHandle)
				{
					int32_t numEntries = (int32_t)swapU32(*(uint32_t*)pMem);
					char* pStrings = pMem + (numEntries << 0x3) + 0x4;
					uint8_t* pEntry = pMem + 0x4;
					uint32_t stringId = 0x0;
					uint32_t offset = 0;
					for(int i = 0; i < numEntries; i++)
					{
						stringId = swapU32(*(uint32_t*)pEntry);
						offset = swapU32(*(uint32_t*)(pEntry+0x4));
						fprintf(dumpHandle, "0x%08X: %s\n", stringId, (pStrings + offset) );
						pEntry+=0x8;
					}
					fclose(dumpHandle);
					printf("Done....\n");
				}
				free(pMem);	
				return 0;
			}
		}
	}
}
