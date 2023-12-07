/*
 *	@file	main.c
 *	@brief	Advent of Code 2023 Day 1 
 *	@date	December 5, 2023
 *	@author	Peter Maxwell Warasila
 *
 *	Copyright (c) 2023 Peter Maxwell Warasila. All rights reserved.
 *
 *	SPDX-License-Identifer: UNLICENSED
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct line {
	bool first_found;
	char first;
	char last;
	uint64_t window;	
};

void line_reset(struct line *line)
{
	memset(line, 0, sizeof(*line));
}

int line_total(struct line *line)
{
	return (line->first * 10) + line->last;
}

void line_finish(struct line *line, int *lines, int *total)
{
	(*lines)++;
	int subtotal = line_total(line);
	*total += line_total(line);
	printf("\tfirst: %d, last: %d, line total: %d, total: %d\r\n",
	       line->first, line->last, subtotal, *total);	
	line_reset(line);
}

int line_check_window(struct line *line)
{
	uint64_t match = line->window & 0xffffffffff;
	switch (match) {
		case (((uint64_t) 't' << (4 * 8)) |
		      ((uint64_t) 'h' << (3 * 8)) |
		      ((uint64_t) 'r' << (2 * 8)) |
		      ((uint64_t) 'e' << (1 * 8)) |
		       (uint64_t) 'e'):
			return 3;
		case (((uint64_t) 's' << (4 * 8)) |
		      ((uint64_t) 'e' << (3 * 8)) |
		      ((uint64_t) 'v' << (2 * 8)) |
		      ((uint64_t) 'e' << (1 * 8)) |
		       (uint64_t) 'n'):
			return 7;
		case (((uint64_t) 'e' << (4 * 8)) |
		      ((uint64_t) 'i' << (3 * 8)) |
		      ((uint64_t) 'g' << (2 * 8)) |
		      ((uint64_t) 'h' << (1 * 8)) |
		       (uint64_t) 't'):
			return 8;
	}
	match &= 0xffffffff;
	switch (match) {
		case (('f' << (3 * 8)) |
		      ('o' << (2 * 8)) |
		      ('u' << (1 * 8)) |
		       'r'):
			return 4;
		case (('f' << (3 * 8)) |
		      ('i' << (2 * 8)) |
		      ('v' << (1 * 8)) |
		       'e'):
			return 5;
		case (('n' << (3 * 8)) |
		      ('i' << (2 * 8)) |
		      ('n' << (1 * 8)) |
		       'e'):
			return 9;
	}
	match &= 0xffffff;
	switch (match) {
		case (('o' << (2 * 8)) |
		      ('n' << (1 * 8)) |
		       'e'):
			return 1;
		case (('t' << (2 * 8)) |
		      ('w' << (1 * 8)) |
		       'o'):
			return 2;
		case (('s' << (2 * 8)) |
		      ('i' << (1 * 8)) |
		       'x'):
			return 6;
	}
	return -1;
}


void line_consume_character(struct line *line, char c)
{
	line->window = (line->window << 8) | c;

	char val;

	if ('0' <= c && c <= '9') {
		val = c - '0';
	} else {
		val = line_check_window(line);
	}

	if (0 <= val) {
		if (!line->first_found) {
			line->first_found = true;
			line->first = val;
		}
		line->last = val;
	}
}

int main(int argc, char **argv)
{
	FILE *f = fopen(argv[1], "r");

	if (NULL == f) {
		return -1;
	}

	int lines = 0;
	int total = 0;
	struct line line;
	line_reset(&line);

	while (1) {
		int c = fgetc(f);
		if (EOF == c) {
			if (line.first_found) {
				line_finish(&line, &lines, &total);
			}
			break;
		}
		if (c == '\n') {
			line_finish(&line, &lines, &total);
		} else if (c != '\r') {
			putchar(c);
		}

		line_consume_character(&line, c);
	}

	(void) fclose(f);

	printf("lines:\t%d\r\n", lines);
	printf("total:\t%d\r\n", total);

	return 0;
}

