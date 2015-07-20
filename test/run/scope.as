extern def exit(status: int32);

let value: int32 = 42;
{
  let value: int32 = 34;
  {
    let value: int32 = 60;
  }
}

exit(value);
