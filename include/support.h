#ifndef LUMA_SUPPORT_H
#define LUMA_SUPPORT_H

#define _log(name, format, args...) fprintf(stdout, "["#name":log] "format"\n", ## args)
#define _info(name, format, args...) fprintf(stdout, "\x1b[30;1m["#name":info] "format"\x1b[0m\n", ## args)
#define _warn(name, format, args...) fprintf(stderr, "\x1b[33;1m["#name":warn] "format"\x1b[0m\n", ## args)
#define _error(name, format, args...) fprintf(stderr, "\x1b[31;1m["#name":error] "format"\x1b[0m\n", ## args)

#define bool unsigned int
#define false 0
#define true 1

#endif

