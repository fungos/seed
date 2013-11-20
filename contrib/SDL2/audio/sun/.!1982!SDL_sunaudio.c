/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL2/SDL_config.h"

#if SDL_AUDIO_DRIVER_SUNAUDIO

/* Allow access to a raw mixing buffer */

#include <fcntl.h>
#include <errno.h>
#ifdef __NETBSD__
#include <sys/ioctl.h>
#include <sys/audioio.h>
#endif
#ifdef __SVR4
#include <sys/audioio.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#endif
#include <unistd.h>

#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_sunaudio.h"

/* Open the audio device for playback, and don't block if busy */

#if defined(AUDIO_GETINFO) && !defined(AUDIO_GETBUFINFO)
#define AUDIO_GETBUFINFO AUDIO_GETINFO
#endif

/* Audio driver functions */
static Uint8 snd2au(int sample);

/* Audio driver bootstrap functions */
static void
SUNAUDIO_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    SDL_EnumUnixAudioDevices(iscapture, 1, (int (*)(int fd)) NULL, addfn);
}

#ifdef DEBUG_AUDIO
void
CheckUnderflow(_THIS)
{
#ifdef AUDIO_GETBUFINFO
    audio_info_t info;
    int left;

    ioctl(this->hidden->audio_fd, AUDIO_GETBUFINFO, &info);
    left = (this->hidden->written - info.play.samples);
    if (this->hidden->written && (left == 0)) {
        fprintf(stderr, "audio underflow!\n");
    }
#endif
}
#endif

static void
SUNAUDIO_WaitDevice(_THIS)
{
#ifdef AUDIO_GETBUFINFO
#define SLEEP_FUDGE 10      /* 10 ms scheduling fudge factor */
    audio_info_t info;
    Sint32 left;

    ioctl(this->hidden->audio_fd, AUDIO_GETBUFINFO, &info);
    left = (this->hidden->written - info.play.samples);
    if (left > this->hidden->fragsize) {
        Sint32 sleepy;

        sleepy = ((left - this->hidden->fragsize) / this->hidden->frequency);
        sleepy -= SLEEP_FUDGE;
        if (sleepy > 0) {
            SDL_Delay(sleepy);
        }
    }
#else
    fd_set fdset;

    FD_ZERO(&fdset);
    FD_SET(this->hidden->audio_fd, &fdset);
    select(this->hidden->audio_fd + 1, NULL, &fdset, NULL, NULL);
#endif
}

static void
SUNAUDIO_PlayDevice(_THIS)
{
    /* Write the audio data */
    if (this->hidden->ulaw_only) {
        /* Assuming that this->spec.freq >= 8000 Hz */
        int accum, incr, pos;
        Uint8 *aubuf;

        accum = 0;
        incr = this->spec.freq / 8;
        aubuf = this->hidden->ulaw_buf;
        switch (this->hidden->audio_fmt & 0xFF) {
        case 8:
            {
                Uint8 *sndbuf;

                sndbuf = this->hidden->mixbuf;
                for (pos = 0; pos < this->hidden->fragsize; ++pos) {
                    *aubuf = snd2au((0x80 - *sndbuf) * 64);
                    accum += incr;
                    while (accum > 0) {
                        accum -= 1000;
                        sndbuf += 1;
                    }
                    aubuf += 1;
                }
            }
            break;
        case 16:
            {
                Sint16 *sndbuf;

                sndbuf = (Sint16 *) this->hidden->mixbuf;
                for (pos = 0; pos < this->hidden->fragsize; ++pos) {
                    *aubuf = snd2au(*sndbuf / 4);
                    accum += incr;
                    while (accum > 0) {
                        accum -= 1000;
                        sndbuf += 1;
                    }
                    aubuf += 1;
                }
            }
            break;
        }
#ifdef DEBUG_AUDIO
        CheckUnderflow(this);
#endif
        if (write(this->hidden->audio_fd, this->hidden->ulaw_buf,
            this->hidden->fragsize) < 0) {
            /* Assume fatal error, for now */
            this->enabled = 0;
        }
        this->hidden->written += this->hidden->fragsize;
    } else {
#ifdef DEBUG_AUDIO
        CheckUnderflow(this);
#endif
        if (write(this->hidden->audio_fd, this->hidden->mixbuf,
            this->spec.size) < 0) {
            /* Assume fatal error, for now */
            this->enabled = 0;
        }
        this->hidden->written += this->hidden->fragsize;
    }
}

static Uint8 *
SUNAUDIO_GetDeviceBuf(_THIS)
{
    return (this->hidden->mixbuf);
}

static void
SUNAUDIO_CloseDevice(_THIS)
{
    if (this->hidden != NULL) {
        SDL_FreeAudioMem(this->hidden->mixbuf);
        this->hidden->mixbuf = NULL;
        SDL_free(this->hidden->ulaw_buf);
        this->hidden->ulaw_buf = NULL;
        if (this->hidden->audio_fd >= 0) {
            close(this->hidden->audio_fd);
            this->hidden->audio_fd = -1;
        }
        SDL_free(this->hidden);
        this->hidden = NULL;
    }
}

static int
SUNAUDIO_OpenDevice(_THIS, const char *devname, int iscapture)
{
    const int flags = ((iscapture) ? OPEN_FLAGS_INPUT : OPEN_FLAGS_OUTPUT);
    SDL_AudioFormat format = 0;
    audio_info_t info;

    /* We don't care what the devname is...we'll try to open anything. */
    /*  ...but default to first name in the list... */
    if (devname == NULL) {
        devname = SDL_GetAudioDeviceName(0, iscapture);
        if (devname == NULL) {
            return SDL_SetError("No such audio device");
        }
    }

    /* Initialize all variables that we clean on shutdown */
    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *this->hidden));
    if (this->hidden == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(this->hidden, 0, (sizeof *this->hidden));

    /* Open the audio device */
    this->hidden->audio_fd = open(devname, flags, 0);
    if (this->hidden->audio_fd < 0) {
        return SDL_SetError("Couldn't open %s: %s", devname, strerror(errno));
    }

#ifdef AUDIO_SETINFO
    int enc;
#endif
    int desired_freq = this->spec.freq;

    /* Determine the audio parameters from the AudioSpec */
    switch (SDL_AUDIO_BITSIZE(this->spec.format)) {

    case 8:
        {                       /* Unsigned 8 bit audio data */
            this->spec.format = AUDIO_U8;
#ifdef AUDIO_SETINFO
            enc = AUDIO_ENCODING_LINEAR8;
#endif
        }
        break;

    case 16:
        {                       /* Signed 16 bit audio data */
            this->spec.format = AUDIO_S16SYS;
#ifdef AUDIO_SETINFO
            enc = AUDIO_ENCODING_LINEAR;
#endif
        }
        break;

    default:
        {
            /* !!! FIXME: fallback to conversion on unsupported types! */
            return SDL_SetError("Unsupported audio format");
        }
    }
    this->hidden->audio_fmt = this->spec.format;

    this->hidden->ulaw_only = 0;    /* modern Suns do support linear audio */
#ifdef AUDIO_SETINFO
    for (;;) {
        audio_info_t info;
        AUDIO_INITINFO(&info);  /* init all fields to "no change" */

        /* Try to set the requested settings */
        info.play.sample_rate = this->spec.freq;
        info.play.channels = this->spec.channels;
        info.play.precision = (enc == AUDIO_ENCODING_ULAW)
            ? 8 : this->spec.format & 0xff;
        info.play.encoding = enc;
        if (ioctl(this->hidden->audio_fd, AUDIO_SETINFO, &info) == 0) {

            /* Check to be sure we got what we wanted */
            if (ioctl(this->hidden->audio_fd, AUDIO_GETINFO, &info) < 0) {
                return SDL_SetError("Error getting audio parameters: %s",
                                    strerror(errno));
            }
            if (info.play.encoding == enc
                && info.play.precision == (this->spec.format & 0xff)
                && info.play.channels == this->spec.channels) {
                /* Yow! All seems to be well! */
                this->spec.freq = info.play.sample_rate;
                break;
            }
        }

        switch (enc) {
        case AUDIO_ENCODING_LINEAR8:
            /* unsigned 8bit apparently not supported here */
            enc = AUDIO_ENCODING_LINEAR;
            this->spec.format = AUDIO_S16SYS;
            break;              /* try again */

        case AUDIO_ENCODING_LINEAR:
