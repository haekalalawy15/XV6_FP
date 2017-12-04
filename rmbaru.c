#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    int i;
    if(argc<2){
	printf(2,"need 2 arguments\n");
	exit();
    }

    for(i=1;i<argc;i++){
	unlink(argv[i]);
	exit();
    }
}
