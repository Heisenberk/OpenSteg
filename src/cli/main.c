#include <stdio.h>
#include <stdlib.h>

#include "maths/test.h"
#include "types.h"
#include "commandes/cli.h"

int main(int argc, char *argv[])
{
	stegx_info_t* com=init_stegx_info();
	fill_info(com,argc,argv);
	check_info(com);
	dest_stegx_info(com);
	return 1;
}
