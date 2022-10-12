import serial, csv

lectura_serial = serial.Serial("/tmp/ttyS1", 9600) #Comunicacion con la placa mediante el puerto ttyS1

filename= open("Datos.csv",'w') 
escritura_archivo = csv.writer(filename)

while (True):
	datos_escribir = lectura_serial.readline().decode().split(' ')
	escritura_archivo.writerow(datos_escribir)

filename.close()