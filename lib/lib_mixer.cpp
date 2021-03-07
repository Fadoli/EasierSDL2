#include <stdio.h>
#include "lib_mixer.h"
#include "lib_core.h"

namespace sdl2_lib
{
soundManager::soundManager()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s\n", Mix_GetError());
    }

    this->targetChannels = 32;
    AllocateChannels(this->targetChannels);
}
soundManager::~soundManager()
{
    Mix_CloseAudio();
}
void soundManager::AllocateChannels(int channels)
{
    Mix_AllocateChannels(channels);
}

sound::sound(const char *filename)
{
    this->_sound = Mix_LoadWAV(filename);
}
sound::~sound()
{
    Mix_FreeChunk(this->_sound);
}

soundPlay::soundPlay(sound *sound, int soundLevel, int loopCount)
{
    this->angle = this->distance = 0;
    this->_sound = sound;
    this->channel = Mix_PlayChannel(-1, sound->_sound, loopCount);
    Mix_Volume(this->channel, soundLevel);
}
soundPlay::~soundPlay()
{
    this->_sound = NULL;
}
const soundPlay &soundPlay::setVolume(int volume)
{
    Mix_Volume(this->channel, volume);
    return *this;
}
const soundPlay &soundPlay::setAngle(int angle)
{
    this->angle = angle;
    Mix_SetPosition(this->channel, this->angle, this->distance);
    return *this;
}
const soundPlay &soundPlay::setDistance(int distance)
{
    this->distance = distance;
    Mix_SetPosition(this->channel, this->angle, this->distance);
    return *this;
}
const soundPlay &soundPlay::Pause()
{
    Mix_HaltChannel(this->channel);
    return *this;
}
const soundPlay &soundPlay::Resume()
{
    Mix_Resume(this->channel);
    return *this;
}
} // namespace sdl2_lib
