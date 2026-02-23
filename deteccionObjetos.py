import cv2
import numpy as np
import requests
from ultralytics import YOLO

# Cargar el modelo de deteccion de objetos
model = YOLO('yolov8n.pt') 

'''
#pide la imagen
response = requests.get('http://192.168.4.1/capture', timeout=10)
img_array = np.frombuffer(response.content, np.uint8)
img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
'''


results = model(img)

obstaculos_detectados = []
for result in results:
    for box in result.boxes:
        # Obtener la clase del objeto y la confianza
        clase_id = int(box.cls[0])
        confianza = float(box.conf[0])
        nombre_objeto = model.names[clase_id]
        
        # Filtrar por confianza y por objetos que consideramos obstaculos
        if confianza > 0.5:  # Umbral de confianza
            # Coordenadas del obstaculo (para dibujar el recuadro)
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            
            # Guardar la informacion
            obstaculos_detectados.append({
                'objeto': nombre_objeto,
                'confianza': round(confianza, 2),
                'posicion': (x1, y1, x2, y2)
            })
            
            # Dibujar el recuadro en la imagen (opcional, para depuracion)
            cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(img, f"{nombre_objeto} ({confianza:.2f})", 
                       (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

# --- 5. Decidir que hacer con los obstaculos ---
if obstaculos_detectados:
    print("¡Cuidado! Obstaculos detectados:")
    for obs in obstaculos_detectados:
        print(f"  - {obs['objeto']} (confianza: {obs['confianza']})")
    # Aquí podrías generar una alerta de voz en el celular
else:
    print("Camino despejado.")

# --- 6. Mostrar la imagen con los objetos detectados (opcional) ---
cv2.imshow("Deteccion de Obstaculos", img)
cv2.waitKey(0)
cv2.destroyAllWindows()