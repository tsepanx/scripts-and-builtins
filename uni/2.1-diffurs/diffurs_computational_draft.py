import math
import pprint
import sys
import matplotlib
from matplotlib import pyplot as plt

n = 5
a = 0
b = 0.5
h = (b - a) / n
print(n)
fx = lambda x: -18 * (math.e ** (6 * x)) / (13 * (math.e ** (6 * x)) * (6 * x - 1) + 160/13)
# fx = lambda x: -234 * (math.e ** (6 * x)) / (169 * (math.e ** (6 * x)) * (6 * x - 1) + 160)
fxy = lambda x, y: 26 * x * y ** 2 + 6 * y
yk_analyt_arr = [fx(i * h) for i in range(int(n))]

yk = 26
xk = 0
yk_num_arr = []
# (lambda x, y: y ** 2 + x ** 2 * y ** 2)(xk, yk)
for i in range(int(n)):
    yk = min(int(yk + h * fxy(xk, yk)), sys.maxsize)
    xk += h
    xk = round(xk, 3)
    print(f"xk: {xk}, yk: {yk}")  # , fxy: {fxy(xk, yk)}")
    yk_num_arr.append(yk)

pprint.pprint([(
    round(i * h, 1),
    # yk_num_arr[i],
    # yk_analyt_arr[i],
    yk_num_arr[i] - yk_analyt_arr[i]
) for i in range(int(n))])


x_axis = [i * h for i in range(int(n))]

# plt.rcParams["figure.figsize"] = [7.00, 3.50]
plt.rcParams["figure.autolayout"] = True
plt.xlim(a, b)
plt.ylim(-1000, 1000)
plt.grid()
# error_arr = []
# for i in range(int(n)):
#     e = abs(yk_analyt_arr[i] - yk_num_arr[i])
#     print(e)
plt.plot(x_axis, [abs(yk_analyt_arr[i] - yk_num_arr[i]) for i in range(int(n))], marker="o", color="orange")
plt.plot(x_axis, yk_num_arr, marker="o", color="black")
# plt.plot(x_axis, list(map(lambda x: x / 10000, yk_num_arr)), marker="o")
plt.plot(x_axis, yk_analyt_arr, marker="o")

plt.show()
# matplotlib.pyplot.show()
