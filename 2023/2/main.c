 /*
 *	@file	main.c
 *	@brief	Advent of Code 2023 Day 2
 *	@date	December 7, 2023
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

struct game 
{
	int number;
	bool impossible;
};

enum cube_color {
	CUBE_COLOR_RED = 0,
	CUBE_COLOR_GREEN,
	CUBE_COLOR_BLUE,

	CUBE_COLOR_COUNT,
};

static const int CUBES_IN_BAG[CUBE_COLOR_COUNT] = {
	12,		/* CUBE_COLOR_RED */
	13,	 	/* CUBE_COLOR_GREEN */
	14,	 	/* CUBE_COLOR_BLUE */
};

static const char *CUBE_COLOR_STRINGS[] = {
	"red",		/* CUBE_COLOR_RED */
	"green",	/* CUBE_COLOR_GREEN */
	"blue",		/* CUBE_COLOR_BLUE */
};

struct set
{
	int cubes[CUBE_COLOR_COUNT];
};

enum cube_color cube_color_from_string(char *s, size_t len)
{
	enum cube_color color;
	for (color = CUBE_COLOR_RED; color != CUBE_COLOR_COUNT; color++) {
		if (0 == strncmp(CUBE_COLOR_STRINGS[color], s, len)) {
			break;
		}
	}
	return color;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return -1;
	};

	int games = 0;
	int total = 0;

	FILE *f = fopen(argv[1], "r");

	if (NULL == f) {
		return -1;
	}

	while (!feof(f))
	{
		struct game game = {0};
		int rc = fscanf(f, "Game %d:", &game.number);
		if (rc != 1) {
			printf("failed to parse game (rc:%d,errno:%d,game.number:%d)\r\n",
			       rc, errno, game.number);
			break;
		}

		while (1) {
			int count = 0;
			char color_str[6] = {0};
			char delimiter = '\0';
			rc = fscanf(f, "%d %[^,;\n]%c",
				    &count, color_str, &delimiter);
			if (rc != 3) {
				printf("failed to parse count (rc:%d,errno:%d,count:%d,color_str:%s,delimiter:%c)\r\n",
				       rc, errno, count, color_str, delimiter);
				break;
			}

			enum cube_color color;
			color = cube_color_from_string(color_str,
						       sizeof(color_str));
			if (CUBE_COLOR_COUNT == color) {
				/* Failed to match a color. */
				printf("failed to match cube color \"%s\"\r\n", color_str);
				break;
			}

			if (CUBES_IN_BAG[color] < count) {
				game.impossible = true;
			}

			if (delimiter == '\n') {
				/* End of this game. */
				games++;
				if (!game.impossible) {
					total += game.number;
				}
				break;
			} 
		}
	}

	(void) fclose(f);

	printf("games:\t%d\r\n", games);
	printf("total:\t%d\r\n", total);

	return 0;
}

