#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#define EXIT_LEAVE (2)

int main()
{
    int bufsize = 64;
    char s[64] = {0};
    pid_t pid = -1;
    int status = 0;
    int position = 0;
    char *token = NULL;
    char *string_token = NULL;
    char *combined_str = NULL;
    char **arr = malloc(sizeof(char *) * bufsize);
    assert(arr);
    while (pid != 0)
    {
        memset(s, 0, sizeof(s));
        printf("Enter command \n");
        fgets(s, sizeof(s), stdin);
        s[strlen(s) - 1] = '\0';
        pid = fork();
        if (pid != 0) //parent
        {
            assert(wait(&status) != -1);
            if(WEXITSTATUS(status) == EXIT_LEAVE)
            {
                exit(EXIT_SUCCESS);
            }
        }
        else // child
        {
            break;
        }
    }
    if ((strcmp(s, "leave") == 0))
    {
        exit(EXIT_LEAVE);
    }

    string_token = strtok(s, " ");
    while (string_token != NULL)
    {
        arr[position] = string_token;
        position++;
        if (position >= bufsize)
        {
            bufsize += bufsize;
            arr = realloc(arr, bufsize * sizeof(char *));
            assert(arr);
            if (!arr)
            {
                printf("Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        string_token = strtok(NULL, " ");
    }

    position = 0;
    // "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"
    char *path = getenv("PATH");
    token = strtok(path, ":");
    combined_str = malloc(sizeof(s) + strlen(token) + 1);
    assert(combined_str);
    while (token != NULL)
    {
        memset(combined_str, 0, sizeof(s) + strlen(token) + 1);
        strcpy(combined_str, token);
        strcat(combined_str, "/");
        strcat(combined_str, arr[0]);
        execv(combined_str, arr); //we dont care about the return value in here cus we get a msg later on,if we fail the loop.
        token = strtok(NULL, ":");
    }
    printf("No such command exists, please try again\n");
}