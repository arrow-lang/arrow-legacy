
// <stdlib.h>
// =============================================================================

// Process control
// -----------------------------------------------------------------------------

/// Causes abnormal program termination (without cleaning up).
// TODO: [[noreturn]]
export extern def abort();

/// Causes normal program termination with cleaning up.
// TODO: [[noreturn]]
// TODO: export extern def exit(status: c_int);
export extern def exit(status: int64);

/// Causes quick program termination without completely cleaning up.
// TODO: [[noreturn]]
// TODO: export extern def quick_exit(status: c_int);
export extern def quick_exit(status: int64);

/// Causes normal program termination without cleaning up.
// TODO: [[noreturn]]
// TODO: export extern def _Exit(status: c_int);
export extern def _Exit(status: int64);

/// Registers a function to be called on `exit` invocation.
// TODO: export extern def atexit(func: () -> None) -> c_int;
export extern def atexit(func: () -> None) -> int64;

/// Registers a function to be called on `quick_exit` invocation.
// TODO: export extern def at_quick_exit(func: () -> None) -> c_int;
export extern def at_quick_exit(func: () -> None) -> int64;

/// Calls the host environment's command processor.
// TODO: export extern def system(string: str) -> c_int;
// TODO: export extern def system(string: Option<str>) -> c_int;
export extern def system(string: str) -> int64;

/// Access to the list of environment variables.
export extern def getenv(name: str) -> str;

// Memory management
// -----------------------------------------------------------------------------

/// Allocates memory.
// TODO: export extern def malloc(size: size_t) -> *byte;
export extern def malloc(size: int64) -> *mutable byte;

/// Allocates memory for an array of num objects of size size and
// initializes it to all bits zero.
// TODO: export extern def calloc(num: size_t, size: size_t) -> *byte;
export extern def calloc(num: int64, size: int64) -> *mutable byte;

/// Reallocates the given area of memory.
// TODO: export extern def realloc(new_size: size_t) -> *byte;
export extern def realloc(ptr: *byte, new_size: int64) -> *mutable byte;

/// Deallocates previously allocated memory.
export extern def free(ptr: *byte);

// <stdio.h>
// =============================================================================

// Unformatted input/output.
// -----------------------------------------------------------------------------

/// Writes a character string to stdout.
// TODO: export extern def puts(s: str) -> c_int;
export extern def puts(s: str) -> int64;
