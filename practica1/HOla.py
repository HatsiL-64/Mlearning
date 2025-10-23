import tensorflow as tf
import numpy as np
celsius = np.array([-40, -10, 0, 8, 15, 22, 38], dtype=float)
fahrenheit = np.array([-40, 14, 32, 46.4, 59, 71.6, 100.4], dtype=float)

modelo = tf.keras.Sequential([
    tf.keras.Input(shape=(1,)),
    tf.keras.layers.Dense(units=1)
])

modelo.compile(
    optimizer=tf.keras.optimizers.Adam(0.8),
    loss='mean_squared_error'
)

print("Entrenando...")
modelo.fit(celsius, fahrenheit, epochs=1000, verbose=False)
print("Finalizado")

resultado = modelo.predict(np.array([10870]))
print(f"10870 grados Celsius son {resultado[0][0]} Fahrenheit")
peso = modelo.get_weights()
print(f"Pesos aprendidos: {peso}")



