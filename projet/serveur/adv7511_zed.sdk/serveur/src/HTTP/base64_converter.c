#include <stdio.h>
#include <string.h>

void convert_chunk(const char *chunk, int length, char *base64);

/* Here is how base64 conversion is done
 * --------------------------------------
 * If we take the chunk "ABC" as an example:
 * From ascii, we have
 * A -> 65 -> 0100 0001
 * B -> 66 -> 0100 0010
 * C -> 67 -> 0100 0011
 * Our chunk being 01000001 01000010 01000011, we form groups
 * of 6 bits instead of groups of 8:
 * 010000 010100 001001 000011
 * Mapping each new "byte" to the index table, we get
 * 0001 0000 -> 16 -> Q
 * 0001 0100 -> 20 -> U
 * 0000 1001 -> 9  -> J
 * 0000 0011 -> 3  -> D
 */

static const char base64_index_table[64] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/******************************************************************************
 * converts a non null-terminated ascii string into a null-terminated 
 * base64 string (since a base64 string never contains null)
 *****************************************************************************/
void ascii_to_base64(const char *ascii, int length, char *base64)
{
	int i = 0;
	static char chunk[3];

	while (length-- > 0)
	{
		chunk[i++] = *ascii++;

		/* convert 3 bytes at a time */
		if (i > 2)
		{
			convert_chunk(chunk, 3, base64);
			base64 += 4;
			i = 0;
		}
	}

	/* padding with '=' */
	if (i != 0)
	{
		convert_chunk(chunk, i, base64);
		base64 += 4;
	}

	*base64 = '\0';
}

/******************************************************************************
 * converts a 3 bytes ascii string into a 4 bytes base64 string
 *****************************************************************************/
void convert_chunk(const char *chunk, int length, char *base64)
{
	int i, indexes[4];

	/* first byte: 6 left-most bits */
	indexes[0] = chunk[0] >> 2;

	/* special case: only one byte left in the chunk */
	if (length < 2)
	{
		indexes[1] = (chunk[0] & 3) << 4;
		indexes[2] = indexes[3] = -1;
	}
	else 
	{
		/* second byte */
		indexes[1] = ((chunk[0] & 3) << 4) | (chunk[1] >> 4);

		/* special case: only 2 bytes left in the chunk */
		if (length < 3) 
		{
			indexes[2] = (chunk[1] & 15) << 2;
			indexes[3] = -1;
		}
		else 
		{
			/* third and fourth bytes */
			indexes[2] = (chunk[2] >> 6) | ((chunk[1] & 15) << 2);
			indexes[3] = chunk[2] & 63;
		}
	}

	for (i = 0; i < 4; i++) 
		if (indexes[i] > 0)
			*base64++ = base64_index_table[indexes[i]];
		else
			*base64++ = '=';
}
