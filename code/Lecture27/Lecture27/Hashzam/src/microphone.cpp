#include "microphone.h"
#include "portaudio.h"
#include "vector.h"
#include "fftw3.h"
#include "grid.h"
#include "error.h"
#include <iostream>
#include <math.h>
using namespace std;

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS    (1)
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#define PI 3.14159265

double windowHanning(int i, int windowLength) {
    return 0.54 - (0.46 * cos(2 * PI * (i / ((windowLength - 1) * 1.0))));
}

void normalize(Grid<double> & spectrum) {
    double min = 0;
    double max = 0;
    bool minSet = false;
    bool maxSet = false;
    for(int r = 0; r < spectrum.numRows(); r++) {
        for(int c = 0; c < spectrum.numCols(); c++) {
            double value = spectrum[r][c];
            if(!minSet || value < min) {
                min = value;
                minSet = true;
            }
            if(!maxSet || value > max) {
                max = value;
                maxSet = true;
            }
        }
    }
    for(int r = 0; r < spectrum.numRows(); r++) {
        for(int c = 0; c < spectrum.numCols(); c++) {
            double value = (spectrum[r][c] / 96);
            value = (value > 1) ? 1 : value;
            value = (value < 0) ? 0 : value;
            spectrum[r][c] = value;
        }
    }
}

Grid<double> getSpectrogram(Vector<double> input) {
    int transform_size = 1024;
    int half = transform_size/2;
    int step_size = transform_size/2;
    double in[transform_size];
    double processed[half];
    fftw_complex *out;
    fftw_plan p;

    int num_steps = input.size()/step_size;

    Grid<double> spectrogram(half, num_steps);

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * transform_size);

    for (int x=0; x < num_steps; x++) {
        // Fill the transformation array with a sample frame and apply the window function.
        // Normalization is performed later
        for(int i = 0; i < transform_size; i++) {
            int j = x * step_size + i;
            double value = (j < input.size()) ? input.get(j) : 0.0;
            in[i] = value * windowHanning(i, transform_size) * 5e5;

        }
        p = fftw_plan_dft_r2c_1d(transform_size, in, out, FFTW_ESTIMATE);

        fftw_execute(p);

        for (int i=0; i < half; i++) {
            out[i][0] *= (2./transform_size);                         // real values
            out[i][1] *= (2./transform_size);                         // complex values
            processed[i] = out[i][0]*out[i][0] + out[i][1]*out[i][1]; // power spectrum

            processed[i] = 10./log(10.) * log(processed[i]);   // dB
            spectrogram.set((half - 1) - i, x, processed[i]);
        }
    }

    // fftw_destroy_plan(p);
    fftw_free(out);
    normalize(spectrogram);
    return spectrogram;
}

typedef struct {
    int frameIndex; /* Index into sample array. */
    int maxFrameIndex;
    float * recordedSamples;
}
Recording;

static int recordCallback(const void * inputBuffer, void * outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo * timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void * userData) {
    Recording * data = (Recording * ) userData;
    const SAMPLE * rptr = (const SAMPLE * ) inputBuffer;
    SAMPLE * wptr = & data -> recordedSamples[data -> frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data -> maxFrameIndex - data -> frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (framesLeft < framesPerBuffer) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if (inputBuffer == NULL) {
        for (i = 0; i < framesToCalc; i++) { * wptr++ = SAMPLE_SILENCE; /* left */
            if (NUM_CHANNELS == 2) * wptr++ = SAMPLE_SILENCE; /* right */
        }
    } else {
        for (i = 0; i < framesToCalc; i++) { * wptr++ = * rptr++; /* left */
            if (NUM_CHANNELS == 2) * wptr++ = * rptr++; /* right */
        }
    }
    data -> frameIndex += framesToCalc;
    return finished;
}

static int playCallback(const void * inputBuffer, void * outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo * timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void * userData) {
    Recording * data = (Recording * ) userData;
    SAMPLE * rptr = & data -> recordedSamples[data -> frameIndex * NUM_CHANNELS];
    SAMPLE * wptr = (SAMPLE * ) outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data -> maxFrameIndex - data -> frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (framesLeft < framesPerBuffer) {
        /* final buffer... */
        for (i = 0; i < framesLeft; i++) { * wptr++ = * rptr++; /* left */
            if (NUM_CHANNELS == 2) * wptr++ = * rptr++; /* right */
        }
        for (; i < framesPerBuffer; i++) { * wptr++ = 0; /* left */
            if (NUM_CHANNELS == 2) * wptr++ = 0; /* right */
        }
        data -> frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for (i = 0; i < framesPerBuffer; i++) { * wptr++ = * rptr++; /* left */
            if (NUM_CHANNELS == 2) * wptr++ = * rptr++; /* right */
        }
        data -> frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}



Vector <double> getMicrophoneSample(int numSeconds) {
    PaStreamParameters inputParameters;
    PaStream * stream;
    PaError err = paNoError;
    Recording data;
    int i;
    int totalFrames;
    int numSamples;
    int numBytes;

    data.maxFrameIndex = totalFrames = numSeconds * SAMPLE_RATE; /* Record for a few seconds. */
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data.recordedSamples = (SAMPLE * ) malloc(numBytes); /* From now on, recordedSamples is initialised. */
    if (data.recordedSamples == NULL) {
        //Error("Could not allocate record array.\n");

    }
    for (i = 0; i < numSamples; i++) data.recordedSamples[i] = 0;

    Pa_Initialize();
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device) -> defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    Pa_OpenStream( & stream, & inputParameters,
                   NULL, /* &outputParameters, */
                   SAMPLE_RATE,
                   FRAMES_PER_BUFFER,
                   paClipOff, /* we won't output out of range samples so don't bother clipping them */
                   recordCallback, & data);

    Pa_StartStream(stream);

    cout << "\n=== Now recording ===\n";

    int seconds = 0;
    while (true) {
        err = Pa_IsStreamActive(stream);
        if (err == 0) break;
        Pa_Sleep(1000);
        seconds++;
        if(seconds <= numSeconds) {
            cout << "time = " << seconds << "s" << endl;
        }
    }
    Pa_CloseStream(stream);

    Vector < double > result;
    bool start = false;
    for (int i = 0; i < data.maxFrameIndex; i++) {
        if(data.recordedSamples[i] != 0) {
            start = true;
        }
        if(start) {
            result.add(data.recordedSamples[i]);
        }
    }
    // playSample(result);
    return result;
}



void playSample(Vector<double> input) {
    float * dataCopy = new float[input.size()];
    for (int i = 0; i < input.size(); i++) {
        dataCopy[i] = input.get(i);
    }

    Recording data;
    data.frameIndex = 0;
    data.maxFrameIndex = input.size();
    data.recordedSamples = dataCopy;

    PaStreamParameters outputParameters;
    PaError err = paNoError;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device) -> defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    cout << "now playing back" << endl;

    PaStream * stream;
    Pa_OpenStream( & stream,
                   NULL, & outputParameters,
                   SAMPLE_RATE,
                   FRAMES_PER_BUFFER,
                   paClipOff,
                   playCallback, & data);

    Pa_StartStream(stream);

    while ((err = Pa_IsStreamActive(stream)) == 1) {
        Pa_Sleep(100);
    }
    Pa_CloseStream(stream);

    delete[] dataCopy;
}
