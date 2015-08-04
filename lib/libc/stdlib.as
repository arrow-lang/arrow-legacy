// General utilities

// Communication with the environment

// The abort function causes abnormal program termination to occur, unless the
// signal SIGABRT is being caught and the signal handler does
// not return.
// TODO: [[noreturn]]
export extern def abort();

// The atexit function registers the function pointed to by func,
// to be called without arguments at normal program
// termination.
// TODO: export extern def atexit(func: () -> None) -> c_int;

// The at_quick_exit function registers the function pointed to by func, to
// be called without arguments should quick_exit be called.
// TODO: export extern def at_quick_exit(func: () -> None) -> c_int;

// The exit function causes normal program termination to occur.
// TODO: [[noreturn]]
// TODO: export extern def exit(status: c_int);
export extern def exit(status: int);

// The _Exit function causes normal program termination to occur
// and control to be returned to the host environment.
// No functions registered by the atexit function, the at_quick_exit
// function, or signal handlers registered by the signal
// function are called.
// TODO: [[noreturn]]
// TODO: export extern def _Exit(status: c_int);
export extern def _Exit(status: int);

// The getenv function searches an environment list,
// provided by the host environment, for a string that matches the string
// pointed to by name.
export extern def getenv(name: str) -> str;

// The quick_exit function causes normal program termination to occur.
// No functions registered by the atexit function or signal handlers
// registered by the signal function are called.
// TODO: [[noreturn]]
// TODO: export extern def quick_exit(status: c_int);
export extern def quick_exit(status: int);

// If string is a null pointer, the system function determines whether the host
// environment has a command processor. If string is not a null pointer,
// the system function passes the string pointed to by string to that command
// processor to be executed in a manner which the implementation shall
// document; this might then cause the program calling system to behave in a
// non-conforming manner or to terminate.
// TODO: export extern def system(string: str) -> c_int;
// TODO: export extern def system(string: Option<str>) -> c_int;
export extern def system(string: str) -> int;
