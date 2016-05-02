#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <math.h>
#include "errors.h"
#include "stack.h"

#include "symbol_table.h"
#include "strings.h"
#include "lib.h"
#include "ial.h"


// --- API ---
ERROR interpret(SYMBOL_TABLE*);
ERROR recursive_interpret(FUNCTION_PTR, STACK_PTR);


#endif