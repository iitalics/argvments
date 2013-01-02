#include "argvments.h"
#include <stdlib.h>
#include <string.h>

typedef struct argm argm;

struct argm
{
		/* small id  "-h" */
	char id;
		/* long id  "--help" */
	char* longid;
		/* how many times argument has been used */
	int used;
		/* whether argument takes input (e.g. in gcc, -c  VS  -o) */
	bool input;
		/* function pointer to handler to... handle it */
	argvm_handler handler;
		/* help text, for -h or --help options */
	char* helpText;
	
		/* next item in linked list */
	argm* next;
};

#define CLEANUP_NULLIFY_GLOBALS



// ####################################################



static int argc;
static char** argv;
static char* programName;


static char* usageText;
static char* versionText;

	/* Trigger help text automatically if no arguments are supplied */
static bool no_arg_help;

	/* argi iterates through arguments */
static int argi;

	/* argm variable to represent argument without flag */
static argm basic;

	/* linked list for all types of arguments */
static argm* arglist_first;
static argm* arglist_last;





// ####################################################



static void null_handler (char* _, int __){}
static argm* help_arg ()
{
	argm* m = malloc(sizeof(argm));
	m->id = 'h';
	m->longid = strdup("help");
	m->helpText = strdup("Show this help message");
	m->used = 0;
	m->input = false;
	m->handler = null_handler;
	m->next = NULL;
	return m;
}



	/* init happens here */
void argvm_begin (int _argc, char** _argv)
{
	
	if (argc == 0)
		// welp, what now?
		exit(-1);
	
	argc = _argc - 1;
	argv = _argv + 1;
	programName = _argv[0];
	
	no_arg_help = true;
	
	basic.used = 0;
	basic.handler = NULL;
	
	arglist_first = arglist_last = help_arg();
	
	usageText = versionText = NULL;
}

	/* Check if help text should show */
static inline bool do_help ()
{
	if (no_arg_help || argc == 0) return true;
	int i;
	for (i = 0; i < argc; i++)
		if (strcmp(argv[i]) == "--help" ||
				(argv[i][0] == '-' && argv[i][1] == 'h'))
			return true;
	return false;
}

	/* Assign stuff */
void argvm_usage_text (char* text)
{
	if (usage_text != NULL)
		free(usage_text);
	usage_text = strdup(text);
}
void argvm_version_text (char* text)
{
	if (version_text != NULL)
		free(version_text);
	version_text = strdup(text);
}
void argvm_no_arg_help (bool onoff)
{
	no_arg_help = onoff;
}

// ---------------------------| argm 'methods' |-------------------
static char* argm_helpstring (argm* arg)
{
	int len, i;
	char* buffer;
	
	len = 0;
	if (arg->id)
	{
		len += 2; // '-' + id character
	}
	if (arg->longid != NULL)
	{
		if (arg->id) len += 2; // two spaces precede short id
		len += 2 + strlen(arg->longid);
	}
	
	buffer = malloc(len + 1);
	
	i = 0;
	if (arg->id)
	{
		buffer[0] = '-';
		buffer[1] = arg->id;
		i = 4;
	}
	if (arg->longid)
	{
		if (i > 0)
			strcpy(buffer + 2, "  --");
		else
			buffer[0] = buffer[1] = '-'; /* alternatively: strcpy(buffer, "--"); */
			
					// two dashes
		strcpy(buffer + i + 2, arg->longid);
	}
	
	buffer[len] = 0;
	return buffer;
}
static inline void argm_do (argm* m, char* argument)
{
	if (m == NULL || m->handler == NULL) return;
	m->handler(argument, m->used++);
}




void argvm_basic (argvm_handler handler)
{
	basic_handler = handler;
}
void argvm_arg (char id, char* longid, bool input, argvm_handler handler, char* help)
{
	argm* m = malloc(sizeof(argm));
	m->handler = handler;
	m->id = id;
	if (longid != NULL) m->longid = strdup(longid);
	m->input = input;
	m->next = NULL;
	m->helpText = strdup(help);
	
	if (arglist_first == NULL)
	{
		arglist_first = arglist_last = m;
	}
	else
	{
		arglist_last->next = m;
		arglist_last = m;
	}
}

#define cur_arg argv[argi]
#define arglist_loop(_it) _it = arglist_first; _it != NULL; _it = _it->next


static void handle_help ()
{
	argm* it;
	if (usageText == NULL) // generate usage text if none is given 
	{
		// TODO:
		// Generate text for no-flag argument handler
		int len, i;
		len = 0;
		for (arglist_loop(it))
			if (it->id != 0)
				len++;
		
		usageText = malloc(len + 4); // [-LEN] + null byte
		usageText[0] = '[';
		usageText[1] = '-';
		
		i = 2;
		for (arglist_loop(it))
			if (it->id != 0)
				usageText[i++] = id->id;
		usageText[i] = ']';
		usageText[i + 1] = '\0';
	}
	
	printf("Usage: %s %s\n", programName, usageText);
	
	int i, max_size, cur_size;
	int arglist_len = 0;
	for (arglist_loop(it)) arglist_len++;
	
	char** arglist_helps = malloc(arglist_len * sizeof(char*))
	
	i = 0;
	max_size = 12;
	for (arglist_loop(it))
	{
		cur_size = strlen(
					arglist_helps[i++] = argm_helpstring(it)
				   );
		
		if (cur_size > max_size)
			max_size = cur_size;
	}
	i = 0;
	for (arglist_loop(it))
	{
		printf("\t%s", arglist_helps[i]);
		
		cur_size = strlen(arglist_helps[i]);
		
		while (cur_size++ < max_size)
			putchar(' ');
		
		printf("%s\n", it->helpText);
	}
	
		/* free stuff up */
	for (i = 0; i < arglist_len; i++)
		free(arglist_helps[i]);
	free(arglist_helps);
	
	
	if (version_text != NULL)
		printf("\n%s\n", version_text);
}

static void argvm_cleanup ();


static char* next_arg ()
{
	if (argi == argc - 1) return "";
	return argv[++argi];
}

void argvm_end ()
{
	if (arglist_first == NULL) { }
	else if (do_help())
		handle_help();
	else
	{
		bool found;
		int j, len;
		argm* it;
		
		for (argi = 0; argi < argc; argi++)
		{
			found = false;
			
			if (cur_arg[0] != '-')
			{
				//found = false;
				goto not_found;
			}
			else if (cur_arg[1] == '-') // long id
			{
				for (arglist_loop(it))
				{
					if (it->longid != NULL && strcmp(it->longid, cur_arg + 2) == 0)
					{
						if (it->input)
							argm_do(it, next_arg());
						else
							argm_do(it, "");
						found = true;
						break;
					}
				}
			}
			else // short id
			{
				len = strlen(cur_arg);
				for (j = 1; j < len; j++)
				{
					for (arglist_loop(it))
						if (it->id != 0 && it->id == cur_arg[j])
						{
				/*  ^  */	if (it->input) // -x'blah' or -x 'blah'
							{
				/*  ^  */		if (cur_arg[j + 1] == '\0') // -x 'blah'
								{
				/*  ^  */			argm_do(it, next_arg());
								}
				/*  ^  */		else						// -x'blah'
								{
				/*  ^  */			argm_do(it, cur_arg + j + 1);
									j = len;
									// will break out of 'j < len' loop
				/*  ^  */		}
							}
				/*  ^  */	else  // -abc   args a, b and c are executed
							{
				/*  ^  */		argm_do(it, "");
								j++;
				/*  ^  */	}
							found = true;
				/*  ^-----*/break;
						}
				}
			}
			
			if (!found)
			{
		not_found:
				argm_do(&basic, cur_arg);
			}
		}
	}
	argvm_cleanup();
}



static void argvm_cleanup ()
{
	if (arglist_first != NULL)
	{
		argm* it;
		argm* next;
		for (it = arglist_first; it != NULL; it = next)
		{
			next = it->next;
			
			if (it->longid != NULL)
				free(it->longid);
			
			free(it->helpText);
			
			free(it);
		}
	}
	
	if (usageText != NULL) free(usageText);
	if (versionText != NULL) free(versionText);
	
	
#ifdef CLEANUP_NULLIFY_GLOBALS
	usageText = versionText = NULL;
	arglist_first = arglist_last = NULL;
	argv = NULL;
	argc = argi = -1;
	programName = NULL;
#endif
}