
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifdef HAVE_DVB
#include "audiodvb.h"
#include "condition.h"
#include "devicedvbinput.h"
#include "recordconfig.h"



AudioDVB::AudioDVB(AudioDevice *device)
 : AudioMPEG(device)
{
}

AudioDVB::~AudioDVB()
{
}

DeviceMPEGInput *AudioDVB::get_mpeg_input()
{
        return DeviceDVBInput::get_mpeg_input(device);
}

int AudioDVB::open_input()
{
	if( !audio_open ) {
		if( !mpeg_input ) {
			mpeg_input = get_mpeg_input();
			if( !mpeg_input ) return 1;
		}
		device->in_bits = device->in_config->dvb_in_bits;
		mpeg_input->audio_reset();
		audio_open = 1;
	}
	return 0;
}

#endif
