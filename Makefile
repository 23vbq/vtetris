# Usage:
# make        # compile all binary

.PHONY = all

CC = g++

SRCS := main.cpp vector2.cpp
OUT := vtetris

all: build

build:
	@echo "Building vtetris..."
	${CC} -o ${OUT} ${SRCS}