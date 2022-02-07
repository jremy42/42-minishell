#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

int main ()
{
    char *prompt;

    prompt = NULL;
    while (1)
    {
        prompt = readline("prout~");
        add_history (prompt);
        printf("%s\n", prompt);
        if(prompt[0] == '*')
            break ;
    }
    getcwd(prompt, 100);
    printf("%s\n", prompt);
    chdir("/mnt/nfs/homes/jremy/Desktop/minishel/toto");
    getcwd(prompt, 100);
    printf("%s\n", prompt); 
}