#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  /* getopt */

static char *out_filename = "output";
static char *in_filename;
static char *key1 = "QQ02C";
static char *key2 = "$GPGGA";

void extractor()
{
    if (in_filename == NULL) {
        printf("No input.\n");
        return;
    }   

    FILE *input_file, *output_file;
    char *buff;
    char *sub_buff;
    size_t len = 0;
    size_t nread;

    input_file = fopen(in_filename, "r");
    output_file = fopen(out_filename, "w");
    
    while ((nread = getline(&buff, &len, input_file)) != -1) {
        // check QQ02C keyword, if not exist, go to check next keyword
        if ((sub_buff = strstr(buff, key1)) != NULL) {
            sub_buff[strlen(sub_buff) - 2] = '\n';
            fwrite(sub_buff, 1, strlen(sub_buff) - 1, output_file);
        }
            
        // check $GPGGA keyword, if both keywords not exist, skip this loop
        if ((sub_buff = strstr(buff, key2)) != NULL) {
            sub_buff[strlen(sub_buff) - 2] = '\n';
            fwrite(sub_buff, 1, strlen(sub_buff) - 1, output_file);
        } else
            continue;
    }

    free(buff);
    free(sub_buff);
    fclose(input_file);
}

void flag_parser(int argc, char **argv)
{
    int cmd_opt = 0;

    while(1) {

        cmd_opt = getopt(argc, argv, "o:");

        /* End condition always first */
        if (cmd_opt == -1) {
            break;
        }

        switch (cmd_opt) {
            case 'o':
                out_filename = strdup(optarg);
                break;

            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                break;
            default:
                fprintf(stderr, "Not supported option\n");
                break;
        }
    }

    if (argc > optind)
        in_filename = strdup(argv[optind]);
}

int main(int argc, char **argv)
{
    flag_parser(argc, argv);
    extractor();

    return 0;
}
