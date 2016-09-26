# d2d
Command line interface for d2d convergence
==========================================


Build
-----

```
g++ -Wall -g -o d2d `pkg-config --cflags glib-2.0` d2d.cpp d2d_conv_manager_fake.cpp techno_service.cpp `pkg-config --libs glib-2.0` -lpthread -lreadline
```

or

```
g++ -Wall -g -o d2d `pkg-config --cflags glib-2.0` d2d_tizen.cpp d2d_conv_manager_fake.cpp techno_service.cpp  -lpthread
```

Run
---

```
./d2d
```

###use flags (not in simplified version)

 **-h**   to print help

 **-sdi** to scip dramatic intro
