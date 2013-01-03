#include "argvments.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


static bool all_caps = false;

static void option_caps   (char* _, int times) { all_caps = true; }
static void option_nocaps (char* _, int times) { all_caps = false; }
static void option_arg (char* text, int times)
{
	int i;
	for (i = 0; text[i] != '\0'; i++)
		putchar(
			all_caps ? toupper(text[i])
					 : tolower(text[i])
				);
	putchar(' ');
}

int main (int argc, char** argv)
{
	argvm_begin(argc, argv);
	
	argvm_no_arg_help(false);
	argvm_usage_text("[TEXT..]");
	argvm_version_text("Version 1.0");
	
	argvm_basic(option_arg);
	argvm_option('l', "lower", false, option_nocaps, "Make text lowercase");
	argvm_option('u', "upper", false, option_caps,   "Make text uppercase");
	
	if (argvm_end() != ARGVM_HELP_TEXT)
		putchar('\n');
	
	return EXIT_SUCCESS;
}