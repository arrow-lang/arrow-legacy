// General item "alias" (can be typename)
use c_int = int64;
use new_name = some_identifier;

// Constant expression "alias"
use c_int = if platform.os == "windows" { int64; } else { int32; };
use c_int = int64 if platform.os == "windows" else int32;
use c_int = match platform.os {
  case "windows" => int64,
  case _ => int32
};
