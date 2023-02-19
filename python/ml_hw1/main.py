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
