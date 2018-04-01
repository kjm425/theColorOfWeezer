import os
import librosa
import numpy as np
import wave
import pyaudio
import serial
import pickle
songname='1979'
filename = "C:\\Users\\Kyle\\PycharmProjects\\Audio\\"+songname+".wav"
wf = wave.open(filename, 'rb')
p = pyaudio.PyAudio()
import sys
import time
#y, sr = librosa.load(filename, sr=1000)
#pickleout = open(songname+'.pickle','wb')
#pickle._dump(y,pickleout)
#pickleout.close()

try:
    picklein = open(songname+'.pickle','rb')
except:
    print("Song initialization running...")
    y, sr = librosa.load(filename, sr=1000)
    pickleout = open(songname+'.pickle','wb')
    pickle._dump(y,pickleout)
    pickleout.close()
    picklein = open(songname + '.pickle', 'rb')
y = pickle.load(picklein)
picklein.close()
ser= serial.Serial("COM3",115200)
'''
CHUNK = 1024

stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True)
ysz = np.size(y)
print(wf.getnchannels())
data = wf.readframes(CHUNK)
starttime=stream.get_time()
print("Enjoy the show")
maxval = max(abs(y))
while data != '':
    stream.write(data)
    #print(stream.get_time()-starttime)
    songtime = stream.get_time()-starttime
    #print(y[math.floor(songtime*22050)]*100)
    if songtime*1000 <= ysz :
        #amps.append(abs(y[math.floor(songtime*11025)]*100))
        a = round((abs(y[round(songtime*1000)])/maxval)*255)
        #if a>9:
        #    a=9
        ser.write(b'#%d#' %a)
    else:
        break
    data = wf.readframes(CHUNK)

stream.stop_stream()
stream.close()
p.terminate()
'''
def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)

stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True,
                stream_callback=callback)
ysz = np.size(y)
maxval = max(abs(y))
stream.start_stream()
starttime=stream.get_time()
while stream.is_active():
    songtime = stream.get_time() - starttime
    if songtime * 1000 <= ysz:
        # amps.append(abs(y[math.floor(songtime*11025)]*100))
        a = round((abs(y[round(songtime * 1000)]) / maxval) * 300)
        if a>255:
            a=255
        ser.write(b'#%d#' % a)
    else:
        break
    time.sleep(0.02)

stream.stop_stream()
stream.close()
wf.close()

p.terminate()
