import serial

# Open the serial port
ser = serial.Serial(
    port='/dev/ttyUSB0',     # or 'COM3' on Windows
    baudrate=921600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1                # seconds
)

print("Listening on", ser.port)

# Read loop
while True:
    data = ser.read(40)     # read up to 256 bytes
    print ("\n")
    if data:
        for x in data:
            print("Received:", x)
