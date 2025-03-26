#
# Makefile
#
# liblookup - a platform-independent runtime and static lookup library
#
# Copyright (c) 2025 Impact Tiling Group Pty Ltd.
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
 
CC = gcc
CFLAGS = -Wall -g -fPIC
AR = ar
ARFLAGS = rcs
LIB_NAME_STATIC = liblookup.a
LIB_NAME_DYNAMIC = liblookup.so

SRC_DIR = src
OBJ_DIR = BUILD/obj
INCLUDE_DIR = include
BUILD_DIR = BUILD
INSTALL_LIB_DIR = /usr/local/lib
INSTALL_INCLUDE_DIR = /usr/local/include

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	DYNAMIC_LIB_CMD = $(CC) -dynamiclib -o
	LIB_NAME_DYNAMIC = liblookup.dylib
else
	DYNAMIC_LIB_CMD = $(CC) -shared -o
endif

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR)/usr/local/lib $(BUILD_DIR)/usr/local/include:
	mkdir -p $@

$(LIB_NAME_STATIC): $(OBJ) | $(OBJ_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_NAME_DYNAMIC): $(OBJ) | $(OBJ_DIR)
	$(DYNAMIC_LIB_CMD) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

all: $(LIB_NAME_STATIC) $(LIB_NAME_DYNAMIC)

install: all | $(INSTALL_LIB_DIR) $(INSTALL_INCLUDE_DIR) $(BUILD_DIR)/usr/local/lib $(BUILD_DIR)/usr/local/include
	cp $(LIB_NAME_STATIC) $(INSTALL_LIB_DIR)/
	cp $(LIB_NAME_DYNAMIC) $(INSTALL_LIB_DIR)/
	cp -r $(INCLUDE_DIR)/* $(INSTALL_INCLUDE_DIR)/
	cp $(LIB_NAME_STATIC) $(BUILD_DIR)/usr/local/lib/
	cp $(LIB_NAME_DYNAMIC) $(BUILD_DIR)/usr/local/lib/
	cp -r $(INCLUDE_DIR)/* $(BUILD_DIR)/usr/local/include/

$(INSTALL_LIB_DIR) $(INSTALL_INCLUDE_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_NAME_STATIC) $(LIB_NAME_DYNAMIC)
	rm -rf $(BUILD_DIR)
