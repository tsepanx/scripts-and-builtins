import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from sklearn import linear_model, datasets



data = pd.read_csv('task1_data.csv')

X_train, y_train, X_test, y_test = data.T.values

X_train = X_train.reshape(-1, 1)
y_train = y_train.reshape(-1, 1)
X_test = X_test.reshape(-1, 1)
y_test = y_test.reshape(-1, 1)

regr = linear_model.LinearRegression()
regr.fit(X_train, y_train)


y_prediction = regr.predict(X_test).reshape(-1, 1)

plt.xlabel("x")
plt.ylabel("y")

plt.scatter(X_train, y_train)
plt.scatter(X_test, y_test)
plt.plot(X_test, y_prediction)

plt.show()


def fff(x_coords, y_coords):
    from numpy.linalg import lstsq
    # points = [(1,5),(3,4)]
    # x_coords, y_coords = zip(*points)
    A = np.stack([x_coords, np.ones(len(x_coords)).reshape(-1, 1)]).T[0]
    m, c = lstsq(A, y_coords)[0]
    print("Line Solution is y = {m}x + {c}".format(m=m,c=c))


fff(X_test, y_prediction)