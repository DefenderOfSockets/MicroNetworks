import socket
from time import sleep

# Адрес и порт сервера
SERVER_ADDRESS = '192.168.4.1'
SERVER_PORT = 80

# Создание нового сокета и подключение к серверу
c = socket.socket()
c.connect((SERVER_ADDRESS, SERVER_PORT))
print("Connected to " + str((SERVER_ADDRESS, SERVER_PORT)))

# После подключения бесконечный цикл 
# позволит беспрерывно получать данные
while True:

  # Отправление сообщения на сервер
  # для начала передачи данных
  c.send("Start_Transmission_Data")

  # Принятие данных с сервера
  # и запись их в буфер (2048 байт)
  received_data = c.recv(2048)
  if not received_data:
    print("Server abended. Exiting")
  break

  # Делим буфер-строку на подстроки разделённые символом "А",
  # для определения границ пакета данных
  packet_data = received_data.split("A")
  
  # Делим подстроку на подстроки разделённые символом "F",
  # для определения каждого элемента ax,ay,az,gx,gy,gz в пакете данных
  for i in range(len(packet_data)):
    IMU_data = packet_data[i].split("F")
    print(IMU_data)
    i=+1
    
  # Перерыв на 0.5 секунд, для того чтобы данные успевали обрабатываться
  sleep(0.5)
# Закрываем подключение
c.close()
