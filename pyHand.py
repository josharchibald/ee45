import serial

#previous data packet
oldData = "not collecting"
#indexing
i = 0
#size 256 array
handArray = [i for i in range(256)]


# Create a serial connection object
ser = serial.Serial('COM5', 9600)

while True:
	data = ser.readline()
	data = data.decode().strip() # Decode the received bytes and remove extra whitespace
	if (data == oldData):
		i = 0
	else:
		pass
	if i < 8:
		for k in range(8):
			handArray((8*k)+i) = input(k)
		i = i + 1
		if i == 8:
			i = 64
			print('\n')
			print('\n')

	if i > 8 and i < 72:
		for k in range(8):
			print((8*k)+i,  ' ')
		print('\n')
		i = i + 1
		if i == 72:
			i = 128
			print('\n')
			print('\n')

	if i > 72 and i < 135:
		for k in range(8):
			print((8*k)+i,  ' ')
		print('\n')
		i = i + 1
		if i == 135:
			i = 192
			print('\n')
			print('\n')

	if i > 135 and i < 200:
		for k in range(8):
			print((8*k)+i,  ' ')
		print('\n')
		i = i + 1
		if i == 200:
			i = 201
			print('\n')
			print('\n')




