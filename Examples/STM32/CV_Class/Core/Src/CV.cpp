/*
 * CV.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: Spiros
 */

/*The CV class realises smooth value transition for pitch CV.
 *  The value is updated at the speed set by glide_time.
 *Calling update_target(note) will make the output glide towards that note.
 *Calling The variable "note" refers to one of the 128
 *midi notes.
 */
#include "CV.h"
CV::CV() {
	note_range = 128;
	Semitone = 0xffff / (note_range - 1);
	CV_output_function = 0;
	target_pitch = 0;
	current_pitch = 0;
	last_pitch = 0;

}
void CV::CV_init(uint16_t tolerance, uint16_t glide_const,
		void (*fptr)(uint16_t value)) {
	set_tolerance(tolerance);
	set_glide(glide_const);
	setCV_output_function(fptr);
}

void CV::set_note_range(uint8_t range) {
	if (range > 1) {
		note_range=range;
		Semitone = 0xffff / (note_range - 1);

	}
}

CV::~CV() {
	// TODO Auto-generated destructor stub
}

void CV::set_target_note(uint8_t note) {
	if (note > (note_range-1)) {
		note = note_range-1;
	}

	target_pitch = note * Semitone;

}
uint8_t CV::get_target_note() {
	return (target_pitch / Semitone);
}
uint16_t CV::get_target_pitch() {
	return target_pitch;
}
uint16_t CV::get_current_pitch() {
	return current_pitch;
}
uint16_t CV::get_glide_const() {
	return glide_const;
}
uint16_t CV::get_tolerance() {
	return tolerance;
}
void CV::set_tolerance(uint16_t tolerance) {
	this->tolerance = tolerance;
}
uint16_t CV::get_last_pitch() {
	return last_pitch;
}
uint16_t CV::get_Semitone() {
	return Semitone;
}

void CV::set_target_raw(uint16_t target) {
	target_pitch = target;
}

void CV::set_glide(uint16_t glide_const) {
	this->glide_const = glide_const;
	if (this->glide_const == 0) {
		this->glide_const = 1;
	}
}

//to be called at regular intervals.
uint16_t CV::CV_update() {
	uint16_t specific_glide = glide_const;
	uint16_t pitch_distance;
	last_pitch = current_pitch;
	if (target_pitch > current_pitch) {
		pitch_distance = target_pitch - current_pitch;
		if (pitch_distance <= tolerance) {
			current_pitch = target_pitch;
		} else {
			if (glide_const > (pitch_distance)) {
				specific_glide = pitch_distance;
			}
			current_pitch = current_pitch + pitch_distance / specific_glide;
		}
	}

	else if (current_pitch > target_pitch) {
		pitch_distance = current_pitch - target_pitch;
		if (pitch_distance <= tolerance) {
			current_pitch = target_pitch;
		} else {
			if (glide_const > pitch_distance) {
				specific_glide = pitch_distance;
			}
			current_pitch = current_pitch - pitch_distance / specific_glide;
		}
	}

	if (CV_output_function != 0) {
		(*CV_output_function)(current_pitch);
	}
	return current_pitch;
}

void CV::setCV_output_function(void (*fptr)(uint16_t value)) {
	CV_output_function = fptr;
}
void CV::unsetCV_output_function() {
	CV_output_function = 0;
}
