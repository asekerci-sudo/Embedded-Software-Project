import paho.mqtt.client as mqtt
import serial as serial
import threading
import time

nameTempdict = {}
referenceTemperature = 24
averageTemp = 0
taskCounter = 0
tempID = 0
lampActive = False
enabledThermostat = False
listentoOwnCommandsOnly = False

def tempAverage():
    global averageTemp
    global nameTempdict
    tempSum = 0
    for name, temp in nameTempdict.items():
        tempSum += temp
    averageTemp = tempSum / len(nameTempdict)
    return averageTemp

def tempAverageCheck():
    global averageTemp
    global referenceTemperature
    return averageTemp >= referenceTemperature

def on_connect(client, userdata, flags, rc):
    print("Connected. Resultcode:", rc)

def on_temp(client,userdata,msg):
    global nameTempdict
    global lampActive
    print(90)
    nameTempdict[msg.topic.split("/")[0]] = int(msg.payload)
    averageTemp = tempAverage()
    if(tempAverageCheck() == True and lampActive == False):
        print(100)
        serialcon.write(b'led 1\n')
        lampActive = True
    if(tempAverageCheck() == False and lampActive == True):
        print(101)
        serialcon.write(b'led 0\n')
        lampActive = False
    client.publish("averageTemps",averageTemp)
    print(91)
    return

def on_command(client,userdata,msg):
    global listentoOwnCommandsOnly
    if listentoOwnCommandsOnly and msg.topic != "abdu/commands":
        pass
    else:
        serialcon.write(msg.payload)
        serialcon.write(b'\n')

def on_Thermostat(client,userdata,msg):
    global taskCounter
    global enabledThermostat
    global tempIDCounter
    if enabledThermostat == False and int(msg.payload) == 1:
        serialcon.write(b'temp 0 0 2000\n')
        tempIDCounter = taskCounter
        taskCounter += 1
        enabledThermostat = True
        print("Thermostat enabled")
    if enabledThermostat == True and int(msg.payload) == 0:
        #string = delete
        deleteString = "delete " + str(tempIDCounter) + "\n"
        serialcon.write(deleteString.encode('ASCII'))
        enabledThermostat = False
        print("Thermostat disabled")
    

def ListenAllChannels(listenChannelFlag):
    print("Listens")
    global listentoOwnCommandsOnly
    if(listenChannelFlag.strip() and int(listenChannelFlag) == 1):
        listentoOwnCommandsOnly == True
        print("Own channel is being listened on.")
    if(listenChannelFlag.strip() and int(listenChannelFlag) == 0):
        listentoOwnCommandsOnly == True
        print("All channels are being listened on.")


def activateThermostat(thermostatFlag):
    print(int(thermostatFlag))
    if(thermostatFlag.strip() and (int(thermostatFlag) == 0 or int(thermostatFlag) == 1)):
        client.publish("enableThermostat",int(thermostatFlag))

def on_message(client, obj, msg):
    print(msg.topic + ":" ,msg.payload.decode('utf-8'))

def on_publish(client, obj, mid):
    print("Published. MessageID:", mid)

def on_subscribe(client, obj, mid, granted_qos):
    print("Subscribed:",mid,granted_qos)

def publishCMD(msg,prefix,suffix):
    return client.publish(prefix + "/" + suffix,msg)

#def on_disconnect(client, userdata, flags, rc):
#    return

brokerIP = "134.91.79.29"
serialcon = serial.Serial('/dev/ttyACM0',timeout=5)
time.sleep(2)
client = mqtt.Client("abdu")
client.connect(brokerIP,1883)
client.on_connect = on_connect
client.on_message = on_message
client.on_subscribe = on_subscribe
client.on_publish = on_publish
client.subscribe("+/commands")
client.subscribe("+/temp")
client.subscribe("enableThermostat")
client.subscribe("abdu/output´")
client.subscribe("averageTemps")
client.message_callback_add("enableThermostat",on_Thermostat)
client.message_callback_add("+/commands",on_command)
client.message_callback_add("+/temp",on_temp)
client.loop_start()


def send():
    while True:
        inputText = input()
        if inputText[0:16] == "enableThermostat":
            print(50)
            activateThermostat(inputText[17:])
        elif("listenAll" in inputText):
            print(60)
            ListenAllChannels(inputText[10:])
        else:
            (rc,mid) = publishCMD(inputText,"abdu","commands")

def receive():
    global enabledThermostat
    while True:
        arduinoOutput = serialcon.readline().decode("utf-8")
        if enabledThermostat == True and arduinoOutput[0:11] == "Temperature":
            print(42)
            if(arduinoOutput[14:16].isnumeric() == True):
                (rc,mid) = publishCMD(arduinoOutput[14:16],"abdu","temp")
        else:
            (rc,mid) = publishCMD(arduinoOutput,"abdu","output")


sendThread = threading.Thread(target=send)
receiveThread = threading.Thread(target=receive)
sendThread.start()
receiveThread.start()