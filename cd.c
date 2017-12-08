#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf(1, "Too many/few arguments, use: cd <directory>.\n");
		exit();
	}
	
	if(chdir(argv[1]) == 0)
	{
		printf(1, "Directory changed to %s.\n", argv[1]);
		exit();
	}
	else
	{
		printf(1, "Could not change directory.\n");
    		exit();
	}
}
