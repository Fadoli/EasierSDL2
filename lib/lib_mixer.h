/**
 * @file lib_mixer.h
 * @author Franck Mourre (franck.mourre@gmail.com)
 * @brief Wrapper around sounds / sounds volume and angles
 * @version 0.1
 * @date 2019-07-13
 * 
 * @copyright Copyright (c) 2019
 */

#ifndef mixer_H
#define mixer_H

#include <SDL2/SDL_mixer.h>

/// viens de la lib sous sdl 1.2
typedef Mix_Chunk SDL_Sound;

namespace sdl2_lib
{
class soundManager
{
public:
    void AllocateChannels(int channels);

private:
    soundManager();
    ~soundManager();
    int targetChannels;
};

class sound
{
public:
    sound(const char *name);
    ~sound();
    SDL_Sound *_sound;
};

class soundPlay
{
public:
    soundPlay(sound *sound, int soundLevel = 64, int loopCount = 0);
    ~soundPlay();

    /**
     * @brief Set the Volume of the sound
     * 
     * @param volume 0-128 (128 is 100%)
     * @return const soundPlay& 
     */
    const soundPlay &setVolume(int volume);
    /**
     * @brief Set the Angle of the sound
     * 
     * @param angle 0-360 (automaticly wraps other value around), precision is by 5 unit
     * @return const soundPlay& 
     */
    const soundPlay &setAngle(int angle);
    /**
     * @brief Set the Distance of the sound
     * 
     * @param distance 0-255 (0 is closest, 255 is furthest), precision is by 5 unit
     * @return const soundPlay& 
     */
    const soundPlay &setDistance(int distance);

    const soundPlay &Pause();
    const soundPlay &Resume();

private:
    sound *_sound;
    int channel;
    int angle, distance;
};
} // namespace sdl2_lib

#endif // mixer_H
