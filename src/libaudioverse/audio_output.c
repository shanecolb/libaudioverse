/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/

#include <libaudioverse/private_all.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	LavObject *graph;
	LavCrossThreadRingBuffer *ring_buffer;
	PaStream *stream;
	unsigned int block_size, mix_ahead, channels;
	void* running_flag; //when cleared, the thread dies.
} ThreadParams;

int audioOutputCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
void audioOutputThread(void* vparam);

Lav_PUBLIC_FUNCTION LavError initializeAudioOutput() {
	PaError err = Pa_Initialize();
	if(err < 0) {
		return Lav_ERROR_CANNOT_INIT_AUDIO;
	}
	return Lav_ERROR_NONE;
}

Lav_PUBLIC_FUNCTION LavError createAudioOutputThread(LavObject *graph, unsigned int mixAhead, void **destination) {
	mixAhead+=1; //so we can actually mixahead 0 times.
	STANDARD_PREAMBLE;
	CHECK_NOT_NULL(graph);
	LOCK(graph->mutex);
	unsigned int blockSize = graph->block_size;
	ThreadParams *param = calloc(1, sizeof(ThreadParams));
	param->graph = graph;

	//Calculate the number of channels.
	LavObject *node;
	Lav_graphGetOutputNode(graph, &node);
	unsigned int channels = node->num_outputs;

	LavError err;
	err = createCrossThreadRingBuffer(blockSize*mixAhead, sizeof(float)*channels, &param->ring_buffer);
	ERROR_IF_TRUE(err != Lav_ERROR_NONE, err);
	param->block_size = blockSize;
	param->mix_ahead = mixAhead;

	PaStream *stream;
	Pa_OpenDefaultStream(&stream, 0, channels, paFloat32, ((LavGraph*)graph)->sr, blockSize, audioOutputCallback, param);
	param->stream = stream;
	param->channels = channels;

	//Make the flag:
	err = createAFlag(&param->running_flag);
	ERROR_IF_TRUE(err != Lav_ERROR_NONE, err);
	aFlagTestAndSet(param->running_flag);
	void* th;
	err = threadRun(audioOutputThread, param, &th);
	ERROR_IF_TRUE(err != Lav_ERROR_NONE, err);
	//let it go!
	*destination = param;
	SAFERETURN(Lav_ERROR_NONE);
	STANDARD_CLEANUP_BLOCK;
}

Lav_PUBLIC_FUNCTION void stopAudioOutputThread(void* thread) {
	aFlagClear(((ThreadParams*)thread)->running_flag);
}

void audioOutputThread(void* vparam) {
	ThreadParams *param = (ThreadParams*)vparam;
	LavObject *graph = param->graph;
	LavCrossThreadRingBuffer *rb = param->ring_buffer;
	void* localMemoryManager = createMmanager();
	Pa_StartStream(param->stream);
	//This is simple.
	//Process one block from the graph, write it to the ringbuffer, repeat.
	float* samples = mmanagerMalloc(localMemoryManager, param->block_size*sizeof(float)*param->channels);
	while(aFlagTestAndSet(param->running_flag)) {
		memset(samples, 0, param->block_size*sizeof(float)*param->channels);
		Lav_graphGetBlock(graph, samples);
		CTRBWriteItems(rb, param->block_size, samples);
		while(CTRBGetAvailableWrites(rb) <= param->block_size);// sleepFor(1); //sleep for 1 ms.
	}
	freeMmanager(localMemoryManager);
}

int audioOutputCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
	//read length items into the output buffer.  That is it.
	CTRBGetItems(((ThreadParams*)userData)->ring_buffer, frameCount, output);
	return paNoError;
}
