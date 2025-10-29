import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

#El libro esta horrible, no explico nada de codigo. Lo que entendi fue que la siguiente funcion lo que hace es
#inicializar una neurona. El parametro learn_rate lo que hace es determina que tanto cambia los pesos al equivocarse en
#la prediccion de la clasificacion de un dato, el random state debe ser una semilla de numeros aleatorios el ultimo
#es la cantidad de epocas, limita la cantidad de veces que una neurona ajusta sus pesos
class Perceptron(object):
  def __init__(self, learn_rate = 0.01, n_epocas=50, random_state=1):
    self.learn = learn_rate
    self.n_epocas = n_epocas
    self.random_state = random_state

#esta funcion inicializa los pesos y los ajusta, self_w[] guarda los pesos y en su primer elemento tiene el sesgo
#como parametro pide dos arrays, x contiene las caracteristicas de las muestras, estas se usan al iniciar los pesos.
  def fit(self, x, y):
    randomGen = np.random.RandomState(self.random_state)
    self.w_ = randomGen.normal(loc=0.0, scale=0.01, size=1 + x.shape[1])
    self.errors_ = []
    for _ in range(self.n_epocas):
      errors = 0
      for xi, target in zip(x, y):
        update = self.learn * (target - self.predict(xi))
        self.w_[1:] += update * xi
        self.w_[0] += update
        errors += int(update != 0.0)
      self.errors_.append(errors)
    return self

#esta funcion calcula el producto punto entre valores de entrada x y los pesos correspondientes a estos valores, al 
#final le suma el sesgo
  def net_input(self, x):
    return np.dot(x, self.w_[1:]) + self.w_[0]

#predice la etiqueta de clase de una muestra x, si es mayor que 0, devuelve 1 y si no -1
  def predict(self, x):
    return np.where(self.net_input(x) >= 0.0, 1, -1)


#aqui escanea una tabla con muestras de flores y muestra los ultimos 5 elementos
irisData = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data', header = None)
print(irisData.tail())

#Para asignar las etiquetas de clase seleccionamos los primeros 100 elementos de la 4ta columna de la tabla, esta
#columna contiene los nombres de las muestras, si el nombre es Iris-setosa, le asigna un 1 y si no, un -1
y = irisData.iloc[0:100, 4].values
y = np.where(y == 'Iris-setosa', 1, -1 )
#print(f"{y}")

#Las medidas del sepalo y del petalo se encuentran en las columnas 0 y 2, las extraemos y las ponemos en un arreglo X
x = irisData.iloc[0:100, [0, 2]].values
#print(f"{x}")

#Para representar los datos usamos la libreria matplotlib
plt.scatter(x[:50, 0], x[:50, 1], color='red', marker='o', label='setosa')
plt.scatter(x[50:100, 0], x[50:100, 1], color='blue', marker='X', label='Versicolor')
plt.xlabel('sepalo [cm]')
plt.ylabel('petalo [cm]')
plt.legend(loc='upper left')
plt.show()

#Para entrenar a nuestro perceptron solo tenemos que inicializarlo y usar la funcion fit
neurona = Perceptron(learn_rate=0.1, n_epocas=10)
neurona.fit(x, y)

#Vamos a graficar la cacntidad de errores por epocas en el entrenamiento

plt.plot(range(1, len(neurona.errors_) + 1), neurona.errors_, marker='o')
plt.xlabel('Epocas')
plt.ylabel('Actualizaciones')
plt.show()

#ahora podemos predecir el tipo de flor segun sus medidas, si la etiqueta asignada es 1, sera una setosa, si es -1 sera una
#versicolor

indiceMuestras = []
medidasMuestras = []
for i in range(10):
  indiceMuestras.append(random.randint(0, 100))
  medidasMuestras.append(irisData.iloc[indiceMuestras[i], [0, 2]].values)
  print(f"Numero: {indiceMuestras[i]}")
  print(f"Medidas = {medidasMuestras[i]}")
  if(neurona.predict(medidasMuestras[i]) == 1):
    print("Clase predicha: Setosa")
  else:
    print("Clase predicha: versicolor")