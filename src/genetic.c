/*
 * genetic.c
 *
 *  Created on: Sep 27, 2015
 *      Author: scott
 */

#define TAG "GA"

#include "genetic.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#include "libs/utils.h"
#include "libs/pins.h"

#define BUFFER_NEXT_FIRE_DATA() \
	for (int _i = 0; _i < NUM_STAGES; _i++) \
		nextFire.data[_i] = population[popIndex].data[_i];

#define STOREFILE_SIZE (sizeof(stage_timing_data_t) * NUM_STAGES * GA_POPULATION_SIZE)

typedef shot_timing_data_t individual_t;

unsigned int lastID;
shot_timing_data_t nextFire;
int dataFile;
stage_timing_data_t* storefileRaw;
individual_t* population;
unsigned char popIndex;

void loadPopulation() {
	population = (individual_t*) malloc(
			sizeof(individual_t) * GA_POPULATION_SIZE);
	int i;
	for (i = 0; i < GA_POPULATION_SIZE; i++) {
		population[i].data = &(storefileRaw[i * NUM_STAGES]);
	}
}

int ga_open_storefile() {
	if (access(GA_POPULATION_FILE, F_OK) != -1) {
		// file exists
		dataFile = open(GA_POPULATION_FILE, O_RDWR);
		if (dataFile == -1) {
			SYSLOG_TAG(LOG_ERR, "Could not open storefile");
			return -1;
		}
		return 0;
	} else {
		// file doesn't exist, make it and grow it to proper size
		dataFile = open(GA_POPULATION_FILE, O_RDWR | O_CREAT | O_TRUNC);
		if (dataFile == -1) {
			SYSLOG_TAG(LOG_ERR, "Could not open storefile");
			return -1;
		}
		if (ftruncate(dataFile, STOREFILE_SIZE) == -1) {
			SYSLOG_TAG(LOG_ERR, "Failed to grow storefile to needed size.");
			return -1;
		}

		// successfully created the file, opened it, and grew it to appropriate size
		return 0;
	}
}

int ga_init() {
	// ensure storefile is created and make it if it isn't
	int res = ga_open_storefile();
	if (res)
		return res;

	// memory map the file. has the advantage of being faster as well as the
	// file is treated as being in memory meaning we can access it as if it were an array
	storefileRaw = mmap(NULL, STOREFILE_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, dataFile, 0);

	popIndex = 0;
	nextFire.data = (stage_timing_data_t*) malloc(sizeof(stage_timing_data_t) * NUM_STAGES);
	if (nextFire.data == NULL) {
		SYSLOG_TAG(LOG_CRIT, "Failed to allocate memory for nextFire");
		return -1;
	}
	BUFFER_NEXT_FIRE_DATA();


	return 0;
}
