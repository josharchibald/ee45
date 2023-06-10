i = 0
while i < 200:
	if i < 8:
		for k in range(8):
			print((8*k)+i , ' ')
		print('\n')
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

	if i > 72 and i < 136:
		for k in range(8):
			print((8*k)+i,  ' ')
		print('\n')
		i = i + 1
		if i == 136:
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
