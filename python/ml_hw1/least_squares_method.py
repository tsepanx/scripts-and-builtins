import numpy as np
import pandas as pd

from matplotlib import pyplot as plt
from matplotlib.lines import Line2D

x_train = pd.read_csv('task1_data.csv').T.values[0]
y_train = pd.read_csv('task1_data.csv').T.values[1]
x_test = pd.read_csv('task1_data.csv').T.values[2]
y_test = pd.read_csv('task1_data.csv').T.values[3]


def get_curve_points(coeffs: list[float], x_lim, count=20) -> tuple[list[float], list[float], str]:
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

    return x_coords, y_coords, func_s(coeffs)


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


if __name__ == "__main__":
    xlim = [-10, 10]
    ylim = xlim

    plt.xlim(*xlim)
    plt.ylim(*ylim)

    points = np.array([
        [1, 10],
        [-3, 0],
        [-1, -2],
        [1.2, 1.2],
        [3, 7],
        [4, 3],
    ])

    # x_coords, y_coords = np.split(points, 2, axis=1)
    # plt.scatter(x_coords, y_coords)

    plt.scatter(x_train, y_train, label="train")
    plt.scatter(x_test, y_test, label="test")

    p_train = np.stack([x_train, y_train], axis=1)

    # coeffs = matrices_2d_method(points, degree=3)
    coeffs = matrices_2d_method(p_train, degree=1)

    x_coords_curve, y_coords_curve, label_curve = get_curve_points(coeffs, xlim, count=100)
    print(label_curve)

    plt.plot(
        x_coords_curve, y_coords_curve,
        label=label_curve,
        marker="o",
        markersize=2,
        color="green"
    )

    plt.legend()
    plt.show()
