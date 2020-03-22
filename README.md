# Gliding_CV_class
This class implements 16bit pitch control voltages with adjustable glide/slew. Through the provided API, the user can attach whatever function needed to handle the output values. This enables the class to be used with a variety of DAC peripherals and configurations, as well as within a fully digital synthesizer system.


# Usage
- Create a CV object
- call CV.CV_init(tolerance, glide_const, output_function_pointer) to set the settings of the CV channel.
- Run CV.CV_update() at regular intervals. This function calculates the new value based on the configured glide_const and calls the output function set by the user.
- Whenever a new CV value has to be output (for example, a new key is pressed on a keyboard), call pitch.set_target_note(note). The output will glide towards the target value based on the glide_const set.

Typically, CV.CV_update() is called from a a timer interrupt. The range of the glide_const has to be adjusted in relation to the frequency of the CV.CV_update interrupt.

CV.set_target_note(uint8_t note) connects the 128 MIDI notes (0-127) to the correct target value. In case the user wants to bypass this conversion, CV.set_target_raw(uint16_t target) is used instead. This feature is useful when custom intonation and tunings are to be used.

CV.set_output_function(function) sets the callback for the value output. This function will be called by CV.CV_update everytime it runs (even if the pitch value has not changed since the last call). All the commands nessessary for outputting to the DAC(s) peripherals, along with every required check (for example, wether the DAC will output a value that is the same as the previous value). The values passed to the callback are 16bit and will have to be shifted right appropriately for the chosen DAC (ie, shift 4 bits for a 12bit DAC). If the callback feature is not used, call CV.unsetCV_output_function() or set the function pointer to 0 in the CV.CV_init() call.

The tolerance value is used to determine when the current value is close enough to the target value. If their distance becomes less that the tolerance, the current value is set to be equal to the target value. This improves the behaviour somewhat and makes sure all notes will eventually converge to the correct value and will be in tune. Reccomended values are 10-40. Set to 1 if not desired.

# Function Description
- uint8_t get_target_note() : return the target note currently set 
- uint16_t get_target_pitch(): return the target raw value currently set
- uint16_t get_current_pitch(): return the current raw CV value
- uint16_t get_glide_const(): returh the value of the glide constant
- uint16_t get_tolerance(): return the value of the tolerance constant
- uint16_t get_Semitone(): return the raw value that corresponds to one semitone interval
- uint16_t get_last_pitch(): return the CV value output in the previous CV_upade
- void CV_init(uint16_t tolerance, uint16_t glide_const,void (\*fptr)(uint16_t value)): initialise the class. set tolerance, glide constant and output handler
- void set_tolerance(uint16_t tolerance): set the tolerance
- void set_target_note(uint8_t note): set the target note value
- void set_target_raw(uint16_t target): set the raw target value
- void set_glide(uint16_t glidetime): set the glide constant
- void setCV_output_function(void (\*fptr)(uint16_t value)): attach the output callback
- void unsetCV_output_function(): detach the output callback
- uint16_t CV_update(): update the current CV value
-	void set_note_range(uint8_t range): set the maximum note of the interface. Default is 128, resulting in an octave range of about 10.6 octaves. Smaller values increase the pitch resolution, at the cost of output octave range.

# Setting the Octave Range
By default, the class will devide the full 16 bit range to 128 intervals, corresponding to a full range of 10.6 octaves. This results in a resolution of 32 values per semitone. 
If this is not deemed enough resolution, or a smaller output octave range is required, it can be configured by calling void set_note_range(uint8_t range). Range is given in number of notes: for example, 128 is 10.6 octaves (128/12) and 64 is 5.3 octaves (64/12). Values larger than the configured note_range will be set to the maximum note_range value instead.


# Using with an STM32Cube IDE project - examples
The CV_Class project demonstrates the use of the class with an STM32F072 microcontroller and the STM32Cube IDE.

In the main.cpp file:
- Create a CV object called pitch. Define it before the main() function, as a global variable.
- Call 	pitch.CV_init(tolerance, glide_const, output_CV), to initialise the CV object.
- void set_note(uint8_t note) interfaces the stm32f0xx_it.c file with the main.cpp file. Because the interrupt handler file is a .c it cannot interpret C++ commands using dot notation directly.
- void run_update(): same as set_note above
- output_CV(uint16_t valu) this function is called by the class every time it runs an update. The function checks if there is a different new value to output. If there is, the 16 bit CV value is shifted 4 times to downscale to 12bit then output by the DAC at channel 1.
-Remember to call 	HAL_DAC_Start(&hdac, DAC_CHANNEL_1) and	HAL_TIM_Base_Start_IT(&htim2); to start the DAC and TIM peripherals.

The CubeMX code generator places all interrupt functions in the stm32f0xx_it.c file. This particular example uses the TIM2 timer to run the CV_update() function, as well as vary the target notes in order to demonstrate functionality. All relevant code can be found under void TIM2_IRQHandler():
- run_update() updates the CV value and calls the DAC callback
- the rest of the commands vary the target notes for demonstrating purposes.


# Analogue interfacing
By default, this class is configured to be able to represent all 128 MIDI notes with the available 16 bit values. Most DACs have an output range of 0-3V or 0-5V, therefore some analogue circuitry will be required to increase the voltage to 1V/oct levels (10V peak to peak). For a full scale output (approximately 10 octaves), an amplification factor of ~3.5 will be needed, and a DC offset of about -1.5V (before amplification) or about -5V (after amplification). It is recommened that these are calibrated with multiturn trimmers.




