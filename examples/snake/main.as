// TODO: [ ] Pointers
// TODO: [ ] Modules (Source)
// TODO: [ ] Constant folding
// TODO: [ ] Tuples
// TODO: [ ] Structures
// TODO: [ ] Global variables
// TODO: [ ] Type aliasing
// TODO: [ ] Included Modules:
//            - std
//            - platform

extern def puts(s: str);
extern def printf(s: str, a: int32);
extern def exit(status: int32);

def assert(condition: bool) {
  if condition {
    // Do nothing
    // TODO: Negation / Unless
  } else {
    exit(1);
  }
}

// import platform;
// use c_int = match (platform.os, platform.arch) {
//   ("darwin", _) => int32,
//   ("windows", "x86") => int32,
//   ("windows", "x86_64") => int64,
//   ("linux", _) => int32,
// }

// def main() {
//   // assert(false);
//   let x = true;
//   if x {
//     puts("Sghs");
//   }

// return 23;

extern def SDL_Init(flags: uint32);
extern def SDL_CreateWindow(
  title: str,
  x: int32,
  y: int32,
  w: int32,
  h: int32,
  flags: uint32
) -> uint64;
extern def SDL_FillRect(dst: uint64, rect: uint64, color: uint32);
extern def SDL_GetWindowSurface(window: uint64) -> uint64;
extern def SDL_UpdateWindowSurface(window: uint64);
extern def SDL_Delay(ms: int32);
extern def SDL_DestroyWindow(window: uint64);
extern def SDL_Quit();

// let SDL_INIT_TIMER          = 0x00000001;
// let SDL_INIT_AUDIO          = 0x00000010;
// let SDL_INIT_VIDEO          = 0x00000020;
// let SDL_INIT_JOYSTICK       = 0x00000200;
// let SDL_INIT_HAPTIC         = 0x00001000;
// let SDL_INIT_GAMECONTROLLER = 0x00002000;
// let SDL_INIT_EVENTS         = 0x00004000;
// let SDL_INIT_NOPARACHUTE    = 0x00100000;
// let SDL_INIT_EVERYTHING     = (
//   SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
//   SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER
// );

// #define SDL_WINDOWPOS_UNDEFINED_MASK    0x1FFF0000
// #define SDL_WINDOWPOS_UNDEFINED_DISPLAY(X)  (SDL_WINDOWPOS_UNDEFINED_MASK|(X))
// #define SDL_WINDOWPOS_UNDEFINED         SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)
// #define SDL_WINDOWPOS_ISUNDEFINED(X)    \
//             (((X)&0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

// typedef enum
// {
//     SDL_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
//     SDL_WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
//     SDL_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
//     SDL_WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
//     SDL_WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
//     SDL_WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
//     SDL_WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
//     SDL_WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
//     SDL_WINDOW_INPUT_GRABBED = 0x00000100,      /**< window has grabbed input focus */
//     SDL_WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
//     SDL_WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
//     SDL_WINDOW_FULLSCREEN_DESKTOP = ( SDL_WINDOW_FULLSCREEN | 0x00001000 ),
//     SDL_WINDOW_FOREIGN = 0x00000800,            /**< window not created by SDL */
//     SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000,      /**< window should be created in high-DPI mode if supported */
//     SDL_WINDOW_MOUSE_CAPTURE = 0x00004000       /**< window has mouse captured (unrelated to INPUT_GRABBED) */
// } SDL_WindowFlags;

def main(argc: int32, argv: uint64) -> int32 {
  SDL_Init(0x00000020 | 0x00000001 | 0x00000010 | 0x00004000);
  let window = SDL_CreateWindow("Snake", (0x1FFF0000|0), (0x1FFF0000|0), 640, 480, 0x00000004);
  let surface = SDL_GetWindowSurface(window);

  SDL_FillRect(surface, 0, 0xffffffff);
  SDL_UpdateWindowSurface(window);

  SDL_Delay(10000);

  SDL_Quit();

  return 0;
}
