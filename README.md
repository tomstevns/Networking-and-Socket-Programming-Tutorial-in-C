# Networking-and-Socket-Programming-Tutorial-in-C
# Generalize this example for Windows and Unix
# To properly disconnect TCP session from either side one should explicitly call shutdown(). Nice diagram also lacks this step.

# On the diagram we see a TCP session, but calls sendto() and recvfrom() are used to send/receive UDP datagrams without explicit connection initiation.

# It seems, that bind() may fail much more likely than listen() (e.g. server TCP port is in use), so add some diagnostics there.

# Cross-platform programming targetting both Windows and Unix using socket API is not that hard:
# 1. Use different set of header files - windows.h plus winsock.h
# 2. Remember to initialize socket library - WSAStartup()
# 3. Use send() and recv() rather than write() and read()
# 4. Call closesocket() instead of close()
# Of course, in real life examples things get not so simple.
