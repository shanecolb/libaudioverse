#Synthesize binaural beats, play for 5 seconds.
#But also record this using a graph listener.
#Note: this is not production quality.
#If this example is made too long, it will run out of ram; the solution is to write the wave file in a thread.
import libaudioverse
import wave
import time
import Queue
import struct

libaudioverse.initialize()
sim = libaudioverse.Simulation()
w1 = libaudioverse.SineObject(sim)
w2 = libaudioverse.SineObject(sim)
listener = libaudioverse.GraphListenerObject(sim, 2) #This passes through, and can--in this case--take the place of mixers.
listener.inputs[0] = w1, 0
listener.inputs[1] = w2, 0

#settings for binaural beats: 300 and 305.
w1.frequency = 300
w2.frequency = 305

#the queue and callback for recording.
audio_queue = Queue.Queue()
def callback(obj, frames, channels, buffer):
	#Copying is not optional. The memory belongs to libaudioverse.
	audio_queue.put([buffer[i] for i in xrange(channels*frames)])

listener.set_listening_callback(callback)

print "beginning synthesis and playing for 5 seconds..."
sim.output_object = listener
time.sleep(5.0)
sim.output_object = None
libaudioverse.shutdown()

print "Writing audio data to out.wav in the current directory:"
f = wave.open("out.wav", "w")
f.setnchannels(2)
f.setframerate(44100)
f.setsampwidth(2)
try:
	while True:
		frame = audio_queue.get(block = False)
		frame = [int(i*32767) for i in frame] #python wants integer samples, Libaudioverse gives float.
		frame_string = struct.pack(str(len(frame))+"h", *frame)
		f.writeframes(frame_string)
except Queue.Empty:
		pass
f.close()
