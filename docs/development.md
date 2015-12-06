### Read in the source file
 - Interpet as UTF-8
 - Normalize with NFKC

### Tokenize (=> Token[])

### Parse (=> AST)

### Symbol Table [Pass: Expose]
Construct symbol table: variable (slot), function, module, etc.

### Type Inference [Pass: Analyze Type]
Determine type of each variable (slot),
function (parameter, result), module, etc.

### [Pass: Check Usage]
Determine if variables are being used correctly.
 - "'%s' is an invalid keyword argument"
 - "cannot capture mutable address of the immutable variable '%s'"
 - "module '%s' has no item '%s'"

### [Pass: Check Type]
Determine if types (of variables) are being used correctly.
 - mismatched types: expected 'bool', found '(int, bool)'

### [Pass: Analyze Module]
Determine dependencies of modules; and, if modules are being imported
in an unresolvable order.

### [Pass: Code Folding]

### [Pass: Declare]
Build function and (global) variable declarations.

### [Pass: Define]
Build function and (global) variable definitions (bodies).
