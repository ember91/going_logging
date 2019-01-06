## Description
**goinglogging** is an open source, lightweight, multiplatform, single header
C++11 library that logs information to command line or file. This is useful when
debugging realtime systems where compiling with debug symbols and/or a low
optimization level will slow down the program too much. This library is designed
to make it as effortless as possible to output the name and value of a variable. 

Note that goinglogging is a logging library for debugging when a debugger is a
worse alternative. It is not for heavy logging. Use a proper library for that.

## Usage

```
#include "goinglogging/goinglogging.h"
```

### A variable
```
int i = 1;
l(i);
```
Which outputs:
```
i = 1
```

### Multiple variables
```
int i = 1;
const char* s = "s";
l(i, s);
```
Which outputs:
```
i = 1, s = s
```

### Array
```
int a[] = {0, 1, 2};
l_arr(a, 3);
```

### Matrix
```
int m[][] = {{0, 11}, {22, 33}};
l_mat(m, 2, 2);
```

### Custom objects
Can output any object with an overloaded << operator.

### Prefixes
Output file, line, and other information by using:
```
gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
int i = 1;
l(i);
```
Which outputs:
```
my_file.cpp:68: i = 1
```

## License

Copyright 2018 Emil Berg

Licensed under the [MIT license](LICENSE)
