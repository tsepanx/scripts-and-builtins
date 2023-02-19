import numpy as np

from matplotlib import pyplot as plt

from matrices import get_curve_points
from lsm import lsm_matrix

# data = pd.read_csv('task1_data.csv')
#
# x_train = data.T.values[0]
# y_train = data.T.values[1]
# x_test = data.T.values[2]
# y_test = data.T.values[3]

if __name__ == "__main__":
    border = 10
    # plt.xlim(-border, border)
    # plt.ylim(-border, border)

    # points = np.array([
    #     [1, 10],
    #     [-3, 0],
    #     [-1, -2],
    #     [1.2, 1.2],
    #     [3, 7],
    #     [4, 3],
    # ])
    # x_coords, y_coords = np.split(points, 2, axis=1)

    n = 500
    np.random.seed(10)

    # coords_list = lambda x: np.random.multivariate_normal(0, x, n).reshape(-1, 1)
    # # coords_list = lambda: np.random.exponential(10, n).reshape(-1, 1)
    # # coords_list = lambda: np.random.beta(1, 2, n).reshape(-1, 1)
    #
    # xs = coords_list(1)
    # ys = coords_list(0.5)

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

    plt.scatter(xs, ys)

    coeffs = lsm_matrix(xs, ys, max_degree=2)

    xs_curve, ys_curve, label_curve = get_curve_points(coeffs, [-10, 100])
    print(label_curve)

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
