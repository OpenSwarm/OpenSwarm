The sound lib is an adaptation of the Microchip sample program.
Now it allows you to play a single WAV.
You can choose to play the wole file, or only a part of it.

the wav is stored in const_peur_sound.s
it is a list of sample value. A simple matlab script can be used to create such a file from a wav input.

functions :
- void init_sound(void);
  prepare the sound module. 


- void play_sound(int sound_offset, int sound_length);
  play the sound in const_peur_sound.s from the (file_start + offset) sample to the (file_start + offset + sound_length) sample.
  WARNING :	sound_length integer must be a multiple of 4
		4 samples are sent at each interrupt, so if length is not a multiple of 4, the stop condition is never true. the program may crash and reboot the DsPic.

Two global variables are exported :
StopFlag
and
DCIUnavailable

StopFlag is automatically set to 0 when a sound is played. But if you want to stop the sound during a play, you can set this variable to 1. Next time the interrupt tries to send new sound data, it will stop.
DCIUnavailable is a flag that indicate that the sound module is playing something. When sound is over, the flag return to 0.