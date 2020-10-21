/**
 * bshell - A simple Cadex BASIC/BASIC86 shell
 *
 *
 * Copyright (C) 2019-2020 The CadexOS Project
 * This software is distributed under the GNU General Public License.
 * See the file LICENSE for details.
 */

#include <auth.h>
#include <library/math.h>
#include <library/scanf.h>
#include <library/stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_INPUT_CHARS 1024
#define pass            continue

int a, b, c, d, e, f, g, h, sd, dd, dx, ax, ac, edx, ecd, ecx, rbp;
char *rsp;

int check_semicolon(char *string[]) { return strEndsWith(string, ";"); }
int check_colon(char *string[]) { return strEndsWith(string, ":"); }
int is_annotation(char *string[]) { return strStartsWith("@", string); }
int is_comment(char *string[]) { return strStartsWith("-", string); }

int main(int argc, char *argv[]) {
    char *cargv[100];
    int cargc;
    int scargv;
    int assembly = 0;
    int asyncmethod = 0;
    int x;
    int y;
    char *current_stored_cmd;
    char *input[MAX_INPUT_CHARS];
    char *pre_prompt_str = "";

    printf("BASIC Commander v1.0\n");
    printf(
        "Type HELP for a list of commands or EXIT for exiting the commander\n");
    while (1) {
        // printf("%s> ", pre_prompt_str);
        scanf(input, sizeof(input));
        // Number of arguments
        cargc = 0;
        // LEXER!!!
        // This is similar to cargv.split(" ") in JavaScript
        cargv[cargc] = strtok(input, " ");
        while (cargv[cargc]) {
            cargc++;
            cargv[cargc] = strtok(0, " ");
        }
        if (cargc > 0) {
            if (!strcmp(cargv[0], "help")) {
                printf("List of available commands:\n");
                printf(" * PRINT: Prints text to the screen\n");
                printf(" * INPUT: Gets input from the user and print the "
                       "prompt if "
                       "specified\n");
                printf(" * EXIT: Exits the commander\n");
                printf(" * END: Ends the current command input stream\n");
                printf(" * CLS: Clears the screen\n");
                printf(" * EXECUTE: Executes the specified file\n");
                printf(" * CMDREGISTER: Adds a new command to the command "
                       "list. [NOTE: "
                       "This commander can only store one custom command]\n");
                printf("\nList of available Annotations (Attributes):\n");
                printf(" * @CMD: Enables command definition mode\n");
                printf(" * @END_CMD: Disables command definition mode\n");
            } else if (is_comment(cargv[0])) {
                continue;
            }

            else if (!strcmp(cargv[0], "exit")) {
                _process_exit(0);
                return 0;
            } else if (!strcmp(cargv[0], "print:")) {
                scargv = cargc;
                for (size_t i = 1; i < scargv; i++) {
                    if (!strcmp(cargv[i], ":end")) {
                        break;
                    } else if (!strcmp(cargv[i], "\\n")) {
                        printf("\n");
                    } else if (!strcmp(cargv[i], "\\t")) {
                        printf("   ");
                    } else if (!strcmp(cargv[i], "$RAND")) {
                        printf("%d", rand(1, 10000));
                    }

                    else {
                        printf("%s ", cargv[i]);
                    }
                }
                printf("\n");
            } else if (!strcmp(cargv[1], "=")) {
                if (cargc > 2) {
                    rsp = cargv[0];
                }
            } else if (!strcmp(cargv[0], rsp)) {

                printf("%s", rsp);
            }

            else if (!strcmp(cargv[0], "cls")) {
                int x1 = 12;
                int y1 = 12;
                int dims[2];
                char stop = -1;
                int i = 0;
                syscall_object_size(WN_STDWINDOW, dims, 2);

                int width = dims[0];
                int height = dims[1];

                render_window(WN_STDWINDOW);
                clearScreen(0, 0, width, height);
                flushScreen();
                flush();
            } else if (!strcmp(cargv[0], "cmdregister")) {
                /* code */
            }

            else if (!strcmp(cargv[0], "execute:")) {
                int pid = fork();

                if (pid == 0) {
                    printf("Process %d started.\n", syscall_process_self());
                    const char *args[] = {cargv[1]};
                    system(cargv[1], 1, args);
                } else {
                    // printf("hello world, I am the parent %d.\n",
                    // syscall_process_self());
                    struct process_info info;
                    syscall_process_wait(&info, -1);
                    syscall_process_reap(info.pid);
                }
            } else if (!strcmp(cargv[0], "drawrect")) {
                int dims[2];
                syscall_object_size(WN_STDWINDOW, dims, 2);

                int width = dims[0];
                int height = dims[1];

                render_window(WN_STDWINDOW);
                // clearScreen(0, 0, width, height);
                flush();
                drawRect(x, y, atoi(cargv[1]), atoi(cargv[2]));
                flush();
                flushScreen();
            } else if (!strcmp(cargv[0], "setx")) {
                if (cargc > 1) {
                    x = atoi(cargv[1]);
                } else {
                    printf("Syntax: setx <x>\n");
                }
            } else if (!strcmp(cargv[0], "sety")) {
                if (cargc > 1) {
                    y = atoi(cargv[1]);
                } else {
                    printf("Syntax: sety <y>\n");
                }
            } else if (!strcmp(cargv[0], "setxy")) {
                if (cargc > 2) {
                    x = atoi(cargv[1]);
                    y = atoi(cargv[2]);
                } else {
                    printf("Syntax: setxy <x> <y>\n");
                }
            } else if (!strcmp(cargv[0], "setcolor")) {
                if (cargc > 1) {
                    if (!strcmp(cargv[1], "red")) {
                        setTextColor(CLEAR_RED, 0);
                        render_window(WN_STDWINDOW);
                        flushScreen();
                        flush();
                    } else if (!strcmp(cargv[1], "green")) {
                        setTextColor(GREEN, 0);
                        render_window(WN_STDWINDOW);
                        flushScreen();
                        flush();
                    } else if (!strcmp(cargv[1], "blue")) {
                        setTextColor(BLUE, 0);
                        render_window(WN_STDWINDOW);
                        flushScreen();
                        flush();
                    }

                    else if (!strcmp(cargv[1], "white")) {
                        setTextColor(WHITE, 0);
                        render_window(WN_STDWINDOW);
                        flushScreen();
                        flush();
                    } else {
                        printf("%s IS NOT A VALID COLOR CODE\n", cargv[1]);
                    }
                }
            } else if (!strcmp(cargv[0], "input:")) {
                char *line[1024];
                scargv = cargc;
                for (size_t i = 1; i < scargv; i++) {
                    if (!strcmp(cargv[i], ":end")) {
                        break;
                    } else if (!strcmp(cargv[i], "\\n")) {
                        printf("\n");
                    } else {
                        printf("%s ", cargv[i]);
                    }
                }
                scanf(line, sizeof(line));
            } else if (!strcmp(cargv[0], "loop")) {
                if (cargc > 2) {
                    for (int io = 0; io > atoi(cargv[1]); io++) {
                        printf(cargv[2]);
                    }
                }
            } else {
                printf("SYNTAX ERROR?\n");
            }
        }
    }
    // return 0;
}
