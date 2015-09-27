/*
 * genetic.h
 *
 *  Created on: Sep 26, 2015
 *      Author: scott
 */

#ifndef GENETIC_H_
#define GENETIC_H_

#include "timing.h"
#include "pins.h"

#define GA_POPULATION_FILE "/var/lib/rtcgmk2/ga_data"

/**
 * Initializes the genetic algorithm processing code
 * Specifically read data in from the storefile and start the worker thread
 */
void ga_init();

/**
 * Saves all data to the storefile
 */
void ga_save_data();

/**
 * Returns an array of the timing data used on the MCU to calculate whent to fire each coil
 * @param data An array of size NUM_COILS containing timing data. Note this assumes you've preallocated
 * 					and will cause a segfault if it's not
 * @return A unique id dictating which generation this data came from.
 */
unsigned int ga_get_next_timing(timing_data_t* data);

/**
 * Send the final velocity data to the genetic algorithm so it can calculate the next generation
 * @param id the unique id given by ga_get_next_timing()
 * @param data An array of size NUM_COILS containing the measured timing data from the fire
 * @param vf The final velocity (needed since velocity in timing data is before each coil)
 */
void ga_set_fire_data(unsigned int id, fire_data_t* data, uint64_t vf);


#endif /* GENETIC_H_ */
