# @brief: For converting svg's star format to opengl's.
raw = [1557, 2160, -78, 198, -203, -62, 106, 184, -176, 120, 211, 32, -16, 212,
       156, -144, 157, 144, -16, -212, 210, -32, -175, -120, 106, -184, -203, 62]
ls = []
for i in range(len(raw)//2):
    ls.append([raw[2*i], raw[2*i+1]])
print(ls)
for i in range(1, len(ls)):
    ls[i][0] += ls[i-1][0]
    ls[i][1] += ls[i-1][1]

for i in range(len(ls)):
    ls[i][1] = 5040-ls[i][1]
print(ls)
