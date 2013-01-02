#ifndef ARGVMENTS_H_INCLUDED
#define ARGVMENTS_H_INCLUDED

#include <stdbool.h>


typedef void (argvm_handler*)(char*, int);


extern void argvm_begin (int argc, char** argv);

extern void argvm_basic (argvm_handler handler);
extern void argvm_arg (char shortName, char* longNameOrNull, bool takesInput, argvm_handler handler, char* help);
extern void argvm_no_arg_help (bool onoff);

extern void argvm_usage_text (char* text);
extern void argvm_version_text (char* text);

extern void argvm_end ();





#endif//ARGVMENTS_H_INCLUDED