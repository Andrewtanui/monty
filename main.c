#include "monty.h"

int main(int argc, char *argv[])
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    unsigned int line_number = 0;
    stack_t *stack = NULL;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1)
    {
        line_number++;
        /* Tokenize the line to get the opcode */
        char *opcode = strtok(line, " \t\n\r");

        if (opcode != NULL)
        {
            int i;
            int valid_opcode = 0;
            instruction_t opcodes[] = {
                {"push", push},
                {"pall", pall},
                {"pint", pint},
                /* Add more opcodes here */
                {NULL, NULL}
            };

            /* Compare the opcode with the valid opcodes */
            for (i = 0; opcodes[i].opcode != NULL; i++)
            {
                if (strcmp(opcode, opcodes[i].opcode) == 0)
                {
                    valid_opcode = 1;
                    opcodes[i].f(&stack, line_number);
                    break;
                }
            }

            if (!valid_opcode)
            {
                fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
                exit(EXIT_FAILURE);
            }
        }
    }

    free(line);
    fclose(file);
    /* Free the stack */
    while (stack != NULL)
    {
        stack_t *temp = stack;
        stack = stack->next;
        free(temp);
    }

    exit(EXIT_SUCCESS);
}
