#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(void)
{
    int option_index = 0,opt = 0;
    int argc = 4;

    char *argv[]=
    {
        "-123",
        "--jin0",
        "--nan0",
        "--cccc0",
    };

    struct option long_options[]=
    {
        {"chen",optional_argument,NULL,'1'},
        {"jin",optional_argument,NULL,'2'},
        {"nan",optional_argument,NULL,'3'},
    };

    while ((opt=getopt_long(argc,argv,"123",long_options,&option_index))!=-1) {
        printf("opt = %c,value = %s\n",opt,optarg);
    }

    return 0;
}

