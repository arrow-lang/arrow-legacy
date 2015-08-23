
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

// Types
// -----------------------------------------------------------------------------

/// Capable of holding all information needed to control a C I/O stream.
export struct FILE {
  // Opaque
}

// Constants
// -----------------------------------------------------------------------------

/// Seek from beginning of file.
export let SEEK_SET = 0;

/// Seek from the current file position.
export let SEEK_CUR = 1;

/// Seek from end of the file.
export let SEEK_END = 2;

// File access
// -----------------------------------------------------------------------------

// Opens a file indicated by filename and returns a file stream
// associated with that file.
export extern def fopen(filename: str, mode: str) -> *FILE;

// Open an existing stream with a different name.
export extern def freopen(filename: str, mode: str, stream: *FILE) -> *FILE;

// Closes the given file stream.
// TODO: export extern def fclose(stream: *FILE) -> c_int;
export extern def fclose(stream: *FILE) -> int64;

// Direct input/output
// -----------------------------------------------------------------------------

// Reads up to count objects into the array buffer from the given input
// stream.
// TODO: export extern def fread(
//   buffer: *mutable byte, size: size_t, count: size_t, stream: *FILE
// ) -> size_t;
export extern def fread(
  buffer: *mutable byte, size: uint64, count: uint64, stream: *FILE
) -> uint64;

// Writes up to count binary objects from the given array buffer to the
// output stream.
// TODO: export extern def fwrite(
//   buffer: *mutable byte, size: size_t, count: size_t, stream: *FILE
// ) -> size_t;
export extern def fwrite(
  buffer: *mutable byte, size: uint64, count: uint64, stream: *FILE
) -> uint64;

// Unformatted input/output
// -----------------------------------------------------------------------------

/// Writes a character string to stdout.
// TODO: export extern def puts(s: str) -> c_int;
export extern def puts(s: str) -> int64;

// File positioning
// -----------------------------------------------------------------------------

// Moves the file position indicator to a specific location in a file.
// TODO: export extern def fseek(stream: *FILE, offset: c_long, whence: c_int) -> c_int;
export extern def fseek(stream: *FILE, offset: int128, whence: int64) -> int64;
