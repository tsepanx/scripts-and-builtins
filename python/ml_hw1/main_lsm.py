import numpy as np

from typing import Sequence
from matplotlib import pyplot as plt

from matrices import get_curve
from lsm import lsm_matrix

# data = pd.read_csv('task1_data.csv')
#
# x_train = data.T.values[0]
# y_train = data.T.values[1]
# x_test = data.T.values[2]
# y_test = data.T.values[3]

Points = tuple[Sequence[float], Sequence[float]]


def correlated_data() -> Points:
    xx = np.array([-0.51, 51.2])
    yy = np.array([0.33, 51.6])
    means = [xx.mean(), yy.mean()]
    stds = [xx.std() / 3, yy.std() / 3]
    corr = 0.8  # correlation
    covs = [[stds[0] ** 2, stds[0] * stds[1] * corr],
            [stds[0] * stds[1] * corr, stds[1] ** 2]]

    m = np.random.multivariate_normal(means, covs, 1000).T
    xs, ys = m
    xs = xs.reshape(-1, 1)
    ys = ys.reshape(-1, 1)

    return xs, ys


def my_points() -> Points:
    points = np.array([
        [1, 10],
        [-3, 0],
        [-1, -2],
        [1.2, 1.2],
        [3, 7],
        [4, 3],
    ])

    xs, ys = np.split(points, 2, axis=1)
    return xs, ys


if __name__ == "__main__":
    border = 50
    plt.xlim(-border, border)
    plt.ylim(-border, border)

    n = 500
    np.random.seed(10)

    # xs, ys = correlated_data()
    xs, ys = my_points()

    plt.scatter(xs, ys)

    coeffs = lsm_matrix(xs, ys, max_degree=2)

    xs_curve, ys_curve, label_curve = get_curve(coeffs, [-border, border], step=0.5)

    plt.plot(
        xs_curve, ys_curve,
        label=label_curve,
        linewidth="3",
        # marker="o",
        # markersize=4,
        color="green"
    )

    plt.legend()
    plt.show()
