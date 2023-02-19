import numpy as np

from matplotlib import pyplot as plt
from matplotlib.lines import Line2D


def update_curve(line: Line2D, coeffs: list[float], x_lim, count=20):
    x_left, x_right = x_lim
    x_coords = np.arange(x_left, x_right, (x_right - x_left) / count).reshape(-1, 1)
    x_coords = list(x_coords.transpose()[0])

    # f = lambda x: a * x ** 2 + b * x + c

    def func_s(coeffs: list[float]) -> str:
        s = ''
        pow = 0
        for c in reversed(coeffs):
            cc = round(c, 1)
            if cc != 0:
                s_coef = f'+ {cc}' if cc > 0 else f'{cc}'
                s_x = '' if pow == 0 else 'x' if pow == 1 else f'x^{pow}'
                s = f'{s_coef}{s_x} ' + s

            pow += 1
        return s

    def func(coeffs: list[float], x) -> float:
        res = 0
        pow = 0
        for c in reversed(coeffs):
            res += c * x ** pow
            pow += 1

        return res

    y_coords = [func(coeffs, x) for x in x_coords]

    # plt.plot(
    #     x_coords, y_coords,
    #     label=func_s(coeffs)
    # )

    line.set_xdata(x_coords)
    line.set_ydata(y_coords)
    line.set_label(func_s(coeffs))


def matrices_2d_method(points: np.ndarray, degree=2) -> list[float]:
    # y = ax^2 + bx + c

    # x = [ a b c ]^T
    # Ax = b -> No solutions
    # Ax_new = p -> Have solutions

    x_coords, y_coords = np.split(points, 2, axis=1)
    A = np.array(list()).reshape(-1, degree + 1)
    b = y_coords

    for xp in x_coords.transpose()[0]:
        # A_row = [xp ** 2, xp, 1]
        A_row = [xp ** i for i in range(degree, -1, -1)]
        A = np.vstack([A, A_row])

    # TODO
    A_p = np.matmul(A.transpose(), A)
    p = np.matmul(A.transpose(), b)

    # TODO
    x_sol = np.linalg.solve(A_p, p)
    coefficients = list(x_sol.transpose()[0])

    print('Coeffs:', *coefficients)
    return coefficients


def update_points(scatter, points: np.ndarray):
    x_coords, y_coords = np.split(points, 2, axis=1)
    # x_coords: np.ndarray
    # y_coords: np.ndarray

    # plt.scatter(x_coords, y_coords)
    scatter.set_xdata(x_coords)
    scatter.set_ydata(y_coords)


xlim = [-20, 20]
ylim = xlim

plt.xlim(*xlim)
plt.ylim(*ylim)
plt.legend()

points_scatter = plt.scatter([1], [1])
line = plt.plot(
    [], [],
    marker="o",
    markersize=4,
    color="orange",
)

for i in range(10):
    points = np.array([
        [i * 3, 10],
        [-3, 0],
        [-1, -2],
        [1.2, 1.2],
        [3, 7],
        [4, 3],
    ])
    points2 = np.array([
        [-1 + i * 3, 2],
        [0 + i, 0],
        [1 + i * 2, -3],
        [2 - i, -5]
    ])

    points = points2

    update_points(points_scatter, points)

    coeffs: list[float] = matrices_2d_method(points, degree=1)

    update_curve(line, coeffs, x_lim=xlim)

    plt.draw()
