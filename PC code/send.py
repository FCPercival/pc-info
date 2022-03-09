import GPUtil
import serial
import time

print("OK")

arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data
while True:
    gpu=GPUtil.getGPUs()[0]
    value = write_read(str(gpu.temperature))
    print(value) # printing the value
    print(str(f"{gpu.temperature}"))
    time.sleep(5)
