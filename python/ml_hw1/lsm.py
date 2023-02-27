import numpy as np

from matrices import matmul


def lsm_matrix(xs: np.ndarray, ys: np.ndarray, max_degree=2) -> list[float]:
    # y = ax^2 + bx + c

    # x = [ a b c ]^T
    # Ax = b -> No solutions
    # Ax_new = p -> Have solutions

    # x_coorT
    # Ax = b -> No solutions
    # Ax_new = p -> Have solutionsds, y_coords = np.split(points, 2, axis=1)
    A = np.array(list()).reshape(-1, max_degree + 1)
    b = ys

    for xp in xs.reshape(1, -1)[0]:
        # A_row = [xp ** 2, xp, 1]
        A_row = [xp ** i for i in range(max_degree, -1, -1)]
        A = np.vstack([A, A_row])

    # Alternative: np.matmul()
    A_p = matmul(A.transpose(), A)
    p = matmul(A.transpose(), b)

    # TODO
    x_sol = np.linalg.solve(A_p, p)
    coefficients = list(x_sol.transpose()[0])

    print('Coeffs:', *coefficients)
    return coefficients


def lsm_analytical_line(xs: np.ndarray, ys: np.ndarray):

    xm = xs.mean()
    ym = ys.mean()

    xy_mean = (xs * ys).mean()

    x_squared_mean = (xs * xs).mean()
    x_mean_squared = xs.mean() ** 2

    k = (xy_mean - xm * ym) / (x_squared_mean - x_mean_squared)
    b = ym - k * xm

    return k, b
