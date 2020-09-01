#include <stdint.h>
#include <string.h>

//#define MOTO_JORDAN     // DEFY
//#define MOTO_SHOLES     // Milestone
//#define MOTO_MILESTONE2 // Milestone 2
//#define MOTO_KOBE       // Bravo
//#define MOTO_SHADOW     // Droid X

#if defined(MOTO_JORDAN)
	#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x89312E76 + 1))(a1, a2, a3, a4, a5)
	#define hax_mbm_segment 0x0100
#elif defined(MOTO_SHOLES)
	#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x8F31C016 + 1))(a1, a2, a3, a4, a5)
	#define hax_mbm_segment 0x009b
#elif defined(MOTO_MILESTONE2)
	#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x89312E76 + 1))(a1, a2, a3, a4, a5)
	#define hax_mbm_segment 0x0108
#elif defined(MOTO_KOBE)
	#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x893139CE + 1))(a1, a2, a3, a4, a5)
	#define hax_mbm_segment 0x0100
#elif defined(MOTO_SHADOW)
	#define read_from_flash(a1, a2, a3, a4, a5) ((int (*)(int, int, char *, int, unsigned int)) (0x8931373A + 1))(a1, a2, a3, a4, a5)
	#define hax_mbm_segment 0x0127
#endif

int main()
{
	read_from_flash(hax_mbm_segment, hax_mbm_segment + 3, (char *) 0x89000000, 0, 0x40000);
	
	memcpy((char *) *(uint32_t *)(0x89000000 + 0x1000), (char *) 0x89000000, 0x40000);

	((void (*)()) (*(uint32_t *)(0x89000000 + 0x1000)))();
	
	return 0;
}