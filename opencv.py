import cv2
import numpy as np 
#FUncion para ordenar puntos/vertices del contorno
def ordenar(vertices):
    #De la varibale aprox, se pasa como lista para tener solo un corchete 
    puntos.np.concatenate([vertices[0],vertices[1],vertices[2],vertices[3]]).tolist()
    #ordenar en y según el valor de puntos gracias a lamda 
    y=sorted(puntos,key=Lambda puntos: puntos[1])#Los que están más cercanos a cero estarán en primer lugar
    x=y[:2]
    x=sorted(x,key=Lambda x:x[0])
    x2=y[2:4]
    x2=sorted(x2,key=Lambda x2:x2[0])
    return [x[0],x[1],x2[0],x2[1]]
    
#Para convertir a formato para OpenCv
imagenCv = np.frombuffer(imagenRe,np.unit8)
imag=cv2.imdecode(imagenCv,cv2.IMREAD_COLOR)
#Escala de grises
gris=cv2.cvtColor(imag,cv2.COLOR_BGR2GRAY)
#Suavizar
gris=cv2.medianBlur(gris,3)
#Detección de bordes, umbral alto y bajo 
bordes=cv2.Canny(gris,10,150)
#dilata imagenes
#Como se van a expandir los pixeles blancos, dirección 
bordes =cv2.dilate(bordes,None,iterations=1)
#Encontrar contornos externos
#Imagen, como devolverá los contornos, como se almacenarñan los puntos del contorno 
contExt=cv2.findContours(bordes,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[0]
#Ordenar los contornos de mayor a menor 
#Objeto para ordenar, Criterio de ordenamiento, en qué orde 
contExt=sorted(contText, key=cv2.contourArea,reverse=True)[:1]
#Ver los vertices del contorno de la imagen 
#arcLength calcula el perimetro de contorno, por un porcentaje para la precisión de aproximación de contornos
#porcApro es la distancia máxima entre el contorno original y la aproximación 
#Detectar el rectangulo 
for a in contExt:
    porcApro=0.01*cv2.arcLength(a,True)#True si el contorno es cerrado
    aprox=cv2.approxPolyDP(a,porcApro,True)#Mantener vértices más prominentes 
    ordenar(aprox)
    #POner puntos de imagen de entrada y de destino 
   # ptse = np.float32([aprox[1][0],aprox[0][0],aprox[2][0],aprox[3][0]])
    ptse=np.float32(puntos)
    ptsd= np.float32([[0,0],[270,0],[0,310],[270,310]])
    nuev=cv2.getPerspectiveTransform(ptse,ptsd)
    imagFinal=cv2.warpPerspective(gris,nuev,(270,310))


