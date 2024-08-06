#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
#define ARENA_MALLOC_ERROR  -1
#define ARENA_CLEANUP_ERROR -2
#define ARENA_NULL_GIVEN    -3

^ I forgot this wasn't zig and I
can't just hand errors to the user
*/

#define SCARY_MAGIC_NUMBER 2
typedef struct Aren {
	void** Blocks;		/* Array of any-pointers */
	int AmountOfBlocks;
} Arena_t;

Arena_t* Arena_Boot(void) {
	Arena_t* NewArena = malloc(sizeof(*NewArena));
	if (!NewArena)
		return NULL;
	NewArena->AmountOfBlocks = 0;
	NewArena->Blocks = malloc(sizeof(void*));
	if (!NewArena->Blocks) {
		free(NewArena);
		return NULL;
	}
		
	return NewArena;
}

void* Arena_Alloc(Arena_t* Arena, void* What, size_t SizeOfWhat) {
	if (!What)
		return NULL;

	Arena->Blocks = realloc( Arena->Blocks, ( sizeof(*(Arena->Blocks) ) * (Arena->AmountOfBlocks + SCARY_MAGIC_NUMBER) ) );
	if (!Arena->Blocks) {
		free(Arena);
		return NULL;
	}

	Arena->Blocks[Arena->AmountOfBlocks] = malloc(SizeOfWhat);
	if (!(Arena->Blocks[Arena->AmountOfBlocks]))
		return NULL;
	memcpy_s(Arena->Blocks[Arena->AmountOfBlocks], SizeOfWhat, What, SizeOfWhat);
	
	return Arena->Blocks[Arena->AmountOfBlocks++];
}

int Arena_Cleanup(Arena_t* Arena) {
	if (!Arena) /* like cmon wyd */
		return NULL;
	int NullPointersFound = 0;
	for (int idx = 0; idx < Arena->AmountOfBlocks; idx++) {
		void* ToFree = Arena->Blocks[idx];
		if (!ToFree)
			NullPointersFound++;
		else
			free(ToFree);
	}

	free(Arena->Blocks);
	free(Arena);

	return NullPointersFound;
	/* I hope this ^ never goes over 0 */
}