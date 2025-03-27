#define MA_NO_DECODING
#define MA_NO_ENCODING

#include <stdio.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

void data_callback
  (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_waveform_read_pcm_frames
      ((ma_waveform*)pDevice->pUserData, pOutput, frameCount, NULL);
}

int main(int argc, char** argv)
{
    ma_device device;
    ma_waveform sineWave;

    {
        ma_device_config cfg = ma_device_config_init(ma_device_type_playback);

        cfg.playback.format   = ma_format_f32;
        cfg.playback.channels = 2;

        cfg.sampleRate   = 48000;
        cfg.dataCallback = data_callback;
        cfg.pUserData    = &sineWave;

        if (ma_device_init(NULL, &cfg, &device) != MA_SUCCESS)
        {
            puts("Failed to open playback device.");
            return -4;
        }
    }

    printf("Device Name: %s\n", device.playback.name);

    {
        ma_waveform_config cfg = ma_waveform_config_init
          (device.playback.format,
           device.playback.channels,
           device.sampleRate,
           ma_waveform_type_sine, 0.2, 220);

        ma_waveform_init(&cfg, &sineWave);
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        puts("Failed to start playback device.");
        ma_device_uninit(&device);
        return -5;
    }
    
    puts("Press Enter to quit...");
    getchar();
    
    ma_device_uninit(&device);
    ma_waveform_uninit(&sineWave);
    
    return 0;
}
