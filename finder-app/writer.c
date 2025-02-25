#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    openlog(NULL,0,LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Error: You must provide two arguments: <file-path> <text-string>\n");
        printf("Error: You must provide two arguments: <file-path> <text-string>\n");
        closelog();
        exit(1);
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    FILE *file = fopen(writefile, "w");
    if (file == NULL)
    {
        syslog(LOG_ERR, "Error opening file %s: %m", writefile);
        perror("Error opening file");
        closelog();
        exit(1);
    }

    fprintf(file, "%s", writestr);
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", writestr, writefile);

    fclose(file);
    closelog();
    return 0;
}
