#include "ngshell.h"

#include <ch.h>

#include <ctype.h>
#include <string.h>

typedef struct {
	ngshell_t *self;
} ngshell_tls_t;

#define SELF ((ngshell_t *)(chThdGetSelfX()->tlsPtr))

static inline char *skipblanks(char *ptr) {
	while (*ptr && isspace((int)*ptr))
		ptr++;
	return ptr;
}

static inline char *skiptoken(char *ptr) {
	while (*ptr && !isspace((int)*ptr))
		ptr++;
	return ptr;
}

static int ngshell_parse(char *buffer, char **argv) {
	int argc = 0;
	char *ptr = buffer;
	while (*ptr) {
		char *token = skipblanks(ptr);
		if (*token != '\0') {
			argv[argc++] = token;
			ptr = skiptoken(token);
			if (*ptr)
				*ptr++ = '\0';
		} else
			break;
	}
	return argc;
}

static void self_puts(const char *str) {
	while(*str)
		SELF->cfg->io->put(*str++);
}

static int builtin_help(int argc, char **argv);

static const ngshell_cmd_t builtins[] = {
		{ "help", builtin_help },
		{ NULL, NULL }
};

static void print_cmd(const ngshell_cmd_t *c) {
	self_puts("> ");
	self_puts(c->command);
	self_puts("\n");
}

static int builtin_help(int argc, char **argv) {
	int i;
	const ngshell_cmd_t *cmd = SELF->commands;

	(void) argc;
	(void) argv;

	self_puts("Commands:\n");
	for (i = 0; builtins[i].function; i++)
		print_cmd(&builtins[i]);
	for (i = 0; cmd[i].function; i++)
		print_cmd(&cmd[i]);

	return 0;
}

static int ngshell_exec(const ngshell_cmd_t *cmds, int argc, char **argv) {
	int i;
	for (i = 0; builtins[i].function; i++)
		if (strcmp(builtins[i].command, argv[0]) == 0)
			return builtins[i].function(argc, argv);
	for (i = 0; cmds[i].function; i++)
		if (strcmp(cmds[i].command, argv[0]) == 0)
			return cmds[i].function(argc, argv);
	self_puts("Command \"");
	self_puts(argv[0]);
	self_puts("\" not found\n");
	return -1;
}

int ngshell_process(ngshell_t *sh) {
	rdline_ctx_t rdctx;
	chThdGetSelfX()->tlsPtr = (void*) sh;
	rdline_init(&rdctx, sh->cfg);
	while (1) {
		int argc;
		char **argv = (char**) sh->argvbuf;
		self_puts(sh->promt);
		rdline_read(&rdctx);
		argc = ngshell_parse(sh->cfg->buf, argv);
		if (argc)
			ngshell_exec(sh->commands, argc, argv);
	}
	return 0;
}

static THD_FUNCTION(ngshell_entry, p) {
  ngshell_process((ngshell_t*) p);
}

void *ngshell_init(ngshell_t *sh, void *wsp, size_t size, int pr) {
	return chThdCreateStatic(wsp, size, pr, ngshell_entry, (void*) sh);
}
