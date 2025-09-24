# ----------------------------
# Makefile Options
# ----------------------------

NAME = NIOCE
ICON = icon.png
DESCRIPTION = "Nspire-IO CE Demo"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)