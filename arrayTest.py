i = 0
l =[]
while i < 200:
	if i < 8:
		for k in range(8):
			print("%4s" % ((8*k)+i), end='')
			l.append(i)
		print()
		i = i + 1
		if i == 8:
			i = 64
			print('\n')

	if i > 8 and i < 72:
		for k in range(8):
			print("%4s" % ((8*k)+i), end='')
			l.append(i)
		print()
		i = i + 1
		if i == 72:
			i = 128
			print('\n')

	if i > 72 and i < 136:
		for k in range(8):
			print("%4s" % ((8*k)+i), end='')
			l.append(i)
		print()
		i = i + 1
		if i == 136:
			i = 192
			print('\n')

	if i > 135 and i < 200:
		for k in range(8):
			print("%4s" % ((8*k)+i), end='')
			l.append(i)
		print()
		i = i + 1
		if i == 200:
			print('done')
			# i = 201
			print('\n')

print(l)

sarr = []
for i in range(4):
	s = '1111000'
	s = s[:i] + '0' + s[i + 1:]
	for j in range(8):
		s2 = str(bin(j))[2:]
		s = s[:-len(s2)] + s2
		sarr.append(s)
print(sarr)

