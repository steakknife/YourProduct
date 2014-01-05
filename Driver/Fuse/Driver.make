#
# Copyright (c) 2008 YourProduct Developers Association. All rights reserved.
#
# Governed by the YourProduct License 3.0 the full text of which is contained in
# the file License.txt included in YourProduct binary and source code distribution
# packages.
#

NAME := Driver

OBJS :=
OBJS += FuseService.o

CXXFLAGS += $(shell pkg-config fuse --cflags)

include $(BUILD_INC)/Makefile.inc
