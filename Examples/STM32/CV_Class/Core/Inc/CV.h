/*
 * CV.h
 *
 *  Created on: Mar 22, 2020
 *      Author: Spiros Makris
 */

/** The CV class realises smooth value transition intented for pitch or slew limited CV.
 * The value is updated at the speed set by glide_time.
 * Calling update_target(note) will make the output glide towards that note.
 * The variable "note" refers to one of the 128 midi notes.
 */
#ifndef INC_CV_H_
#define INC_CV_H_

#include "stdint.h"
class CV {
public:
	CV();

	uint8_t get_target_note();
	uint16_t get_target_pitch();
	uint16_t get_current_pitch();
	uint16_t get_glide_const();
	uint16_t get_tolerance();
	uint16_t get_Semitone();
	uint16_t get_last_pitch();
	void CV_init(uint16_t tolerance, uint16_t glide_const,
			void (*fptr)(uint16_t value));
	void set_tolerance(uint16_t tolerance);
	void set_target_note(uint8_t note);
	void set_target_raw(uint16_t target);
	void set_glide(uint16_t glidetime);
	void setCV_output_function(void (*fptr)(uint16_t value));
	void unsetCV_output_function();
	uint16_t CV_update();
	void set_note_range(uint8_t range);
	virtual ~CV();
private:
	uint16_t note_range;
	uint16_t target_pitch;
	uint16_t current_pitch;
	uint16_t glide_const;
	uint16_t tolerance;
	uint16_t Semitone;
	uint16_t last_pitch;
	void (*CV_output_function)(uint16_t DAC_value);
};

#endif /* INC_CV_H_ */

