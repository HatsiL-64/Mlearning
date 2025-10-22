import numpy as np
import pandas as pd

#El libro esta horrible, no explico nada de codigo. Lo que entendi fue que la siguiente funcion lo que hace es
#inicializar una neurona. El parametro learn_rate lo que hace es determina que tanto cambia los pesos al equivocarse en
#la prediccion de la clasificacion de un dato, el random state debe ser una semilla de numeros aleatorios el ultimo
#es la cantidad de epocas, limita la cantidad de veces que una neurona ajusta sus pesos
class Perceptron(object):
  def __init__(self, learn_rate = 0.01, n_inter=50, random_state=1):
    self.learn = learn_rate
    self.n_inter = n_inter
    self.random_state = random_state

  def fit(self, x, y):
    randomGen = np.random.RandomState(self.random_state)
    self.w_ = randomGen.normal(loc=0.0, scale=0.01, size=1 + x.shape[1])
    self.errors_ = []
    for _ in range(self.n_inter):
      errors = 0
      for xi, target in zip(x, y):
        update = self.learn * (target - self.predict(xi))
        self.w_[1:] += update * xi
        self.w_[0] += update
        errors += int(update != 0.0)
      self.errors_.append(errors)
    return self

  def net_input(self, x):
    return np.dot(x, self.w_[1:]) + self.w_[0]

  def predict(self, x):
    return np.where(self.net_input(x) >= 0.0, 1, -1)

irisData = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data', header = None)
print(irisData.tail())
#irisData.tail(5)