#include <stdint.h>

int main()
{
	*(uint16_t *) 0x8932EAD4 = 0xA55A; // RESET

	/*
	*(uint8_t *) 0x8932F18F = 1;
	*(uint8_t *) 0x89314426 = 1;
	*(uint8_t *) 0x8931D628 = 1;
	*(uint8_t *) 0x8931D662 = 1;
	*/
	
	*(uint16_t *) 0x893200A2 = 0x2001;
	*(uint16_t *) 0x893200A4 = 0x4770;
	
	*(uint32_t *) 0x893100A0 = 0xEA006E19;
	((void (*)()) 0x89310000)();
	
	return 0;
}