import pandas as pd 
import numpy as np
import matplotlib.pyplot as plt

class Adaline(object):

  def __init__(self, learn_rate = 0.01, n_epocas = 50, random_state = 1):
    self.learn=learn_rate
    self.epocas=n_epocas
    self.rand=random_state
    
  def fit(self, x, y):
    rgen = np.random.RandomState(self.rand)
    self.w_ = rgen.normal(loc=0.0, scale=0.01, size=1 + x.shape[1])

    self.cost = []

    for i in range(self.epocas):
      net_input = self.net_input(x)
      output = self.activation(net_input)
      errors = (y - output)
      self.w_[1:] += self.learn * x.T.dot(errors)
      self.w_[0] += self.learn * errors.sum()
      cost = (errors**2).sum() / 2.0
      self.cost.append(cost)
    return self
  
  def net_input(self, x):
    return np.dot(x, self.w_[1:]) + self.w_[0]

  def activation(self, x):
    return x

  def predict(self, x):
    return np.where(self.activation(self.net_input(x)) >= 0.0, 1, -1)
  
irisData = pd.read_csv('./datos.csv')

y = irisData.iloc[0:100, 4].values
y = np.where(y == 'Iris-setosa', 1, -1)

x = irisData.iloc[0:100, [0, 2]].values

fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(10, 4))

neurona1 = Adaline(learn_rate=0.01, n_epocas=10).fit(x, y)
ax[0].plot(range(1, len(neurona1.cost) + 1), np.log10(neurona1.cost), marker='o')
ax[0].set_xlabel('Epocas')
ax[0].set_ylabel('log(Suma ^2 errores)')
ax[0].set_title('adaline eta-0.01')

neurona2 = Adaline(learn_rate=0.0001, n_epocas=10).fit(x, y)
ax[1].plot(range(1, len(neurona2.cost) + 1), neurona2.cost, marker='o')
ax[1].set_xlabel('Epocas')
ax[1].set_ylabel('log(Suma ^2 errores)')
ax[1].set_title('adaline eta-0.0001')

plt.show()