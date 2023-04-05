import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

# Configure the serial port
ser = serial.Serial(
    port='COM3',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS
)
ser.isOpen()

def read_serial_data():
    while True:
        line = ser.readline()
        try:
            x, y = map(int, line.split())
            yield x, y
        except ValueError:
            pass

def update_plot(xy_values):
    x, y = xy_values
    dot.set_data(x, y)
    return dot,

# Set up the plot
fig, ax = plt.subplots()
ax.set_xlim(-100, 100)  # Adjust x-axis range
ax.set_ylim(-100, 100)  # Adjust y-axis range
ax.set_facecolor('white')
dot, = ax.plot([], [], 'ro')

# Run the animation
ani = animation.FuncAnimation(fig, update_plot, read_serial_data, blit=True, interval=100, repeat=False, cache_frame_data=False)
plt.show()