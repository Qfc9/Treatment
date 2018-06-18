#include <stdio.h>
#include <string.h>
#include <libscrypt.h>

int main(void)
{
	char buf[64] = {'\0'};
	char *input  = "67";
	char *salt  = "I Hate Liam Echlin";
	int ret = libscrypt_scrypt(input, strlen(input),
		salt, strlen(salt),
		2048, 4, 4,
		buf, sizeof(buf));
	
	fwrite(buf, sizeof(buf), 1, stdout);
}

