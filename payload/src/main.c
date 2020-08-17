#include <stdint.h>
#include <string.h>

#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x89312E76 + 1))(a1, a2, a3, a4, a5)

int main()
{
	read_from_flash(4, 7, (char *) 0x90000000, 0, 0x40000);
	
	memcpy((char *) 0x89310000, (char *) 0x90000000, 0x40000);
	
	// insecure patch
	*(uint16_t *) 0x893200A2 = 0x2001;
	*(uint16_t *) 0x893200A4 = 0x4770;

	// jump
	((void (*)()) 0x89310000)();
	
	return 0;
}