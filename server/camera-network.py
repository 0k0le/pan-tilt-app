import Adafruit_BBIO.ADC as ADC
import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.GPIO as GPIO
import socket
import threading


# Control PINS
IRIS_POT_PIN = "P9_39"
ZOOM_POT_PIN = "P9_40"
FOCUS_POT_PIN = "P9_37"

IRIS_PWM_PIN = "P9_14"
IRIS_DIR_PIN = "P8_28"

FOCUS_PWM_PIN = "P8_45"
FOCUS_DIR_PIN = "P8_30"

ZOOM_PWM_PIN = "P9_16"
ZOOM_DIR_PIN = "P8_29"

FOCUS_MIN = 0.08
FOCUS_MAX = 1.65

IRIS_MIN = 0.2
IRIS_MAX = 1.5

ZOOM_MIN = 0.2
ZOOM_MAX = 1.5

HOST = "10.25.2.4"
PORT = 65432

IRIS_CONTROL = b"IRIS\x00"
ZOOM_CONTROL = b"ZOOM\x00"
FOCUS_CONTROL = b'FOCU\x00'

flock = threading.Condition()
zlock = threading.Condition()
ilock = threading.Condition()

ftarget = int(1)
ztarget = int(1)
itarget = int(1)

ADC.setup()
GPIO.setup(IRIS_DIR_PIN, GPIO.OUT)
GPIO.setup(FOCUS_DIR_PIN, GPIO.OUT)
GPIO.setup(ZOOM_DIR_PIN, GPIO.OUT)

def focus_thread():
    print("Focus thread starting")

    PWM.start(FOCUS_PWM_PIN)
    PWM.set_duty_cycle(FOCUS_PWM_PIN, 0)

    while True:
        flock.acquire()
        if ftarget == 500:
            break
        flock.release()

        thresh = FOCUS_MAX-FOCUS_MIN
        perc = float(ftarget)/100
        target_voltage = (thresh*perc)+FOCUS_MIN

        current_voltage = ADC.read(FOCUS_POT_PIN) * 1.8
        #print(f"{current_voltage}")

        if current_voltage > target_voltage + 0.01:
            GPIO.output(FOCUS_DIR_PIN, GPIO.HIGH)
            PWM.start(FOCUS_PWM_PIN, 50)
        elif current_voltage < target_voltage - 0.01:
            GPIO.output(FOCUS_DIR_PIN, GPIO.LOW)
            PWM.start(FOCUS_PWM_PIN, 50)
        else:
            PWM.set_duty_cycle(FOCUS_PWM_PIN, 0)
        

    PWM.set_duty_cycle(FOCUS_PWM_PIN, 0)
    PWM.stop(FOCUS_PWM_PIN)

    flock.release()
    print("Focus thread shutting down")

def zoom_thread():
    print("Zoom thread starting")

    PWM.start(ZOOM_PWM_PIN)
    PWM.set_duty_cycle(ZOOM_PWM_PIN, 0)

    while True:
        zlock.acquire()

        if ztarget == 500:
            break

        zlock.release()

        thresh = ZOOM_MAX-ZOOM_MIN
        perc = float(ztarget)/100
        target_voltage = (thresh*perc)+ZOOM_MIN

        current_voltage = ADC.read(ZOOM_POT_PIN)*1.8

        if current_voltage > target_voltage + 0.01:
            GPIO.output(ZOOM_DIR_PIN, GPIO.LOW)
            PWM.start(ZOOM_PWM_PIN, 50)
        elif current_voltage < target_voltage - 0.01:
            GPIO.output(ZOOM_DIR_PIN, GPIO.HIGH)
            PWM.start(ZOOM_PWM_PIN, 50)
        else:
            PWM.set_duty_cycle(ZOOM_PWM_PIN, 0)

    PWM.set_duty_cycle(ZOOM_PWM_PIN, 0)
    PWM.stop(ZOOM_PWM_PIN)

    zlock.release()
    print("Zoom thread shutting down")

def iris_thread():
    print("Iris thread starting")

    PWM.start(IRIS_PWM_PIN)
    PWM.set_duty_cycle(IRIS_PWM_PIN, 0)

    while True:
        ilock.acquire()

        if itarget == 500:
            break

        ilock.release()

        thresh = IRIS_MAX-IRIS_MIN
        perc = float(itarget)/100
        target_voltage = (thresh*perc)+IRIS_MIN

        current_voltage = ADC.read(IRIS_POT_PIN)*1.8

        if current_voltage > target_voltage + 0.01:
            GPIO.output(IRIS_DIR_PIN, GPIO.HIGH)
            PWM.start(IRIS_PWM_PIN, 50)
        elif current_voltage < target_voltage - 0.01:
            GPIO.output(IRIS_DIR_PIN, GPIO.LOW)
            PWM.start(IRIS_PWM_PIN, 50)
        else:
            PWM.set_duty_cycle(IRIS_PWM_PIN, 0)

    PWM.set_duty_cycle(IRIS_PWM_PIN)
    PWM.stop(IRIS_PWM_PIN)

    ilock.release()
    print("Iris thread shutting down")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Client accepted {addr}")

        # Start threads
        fthread = threading.Thread(target=focus_thread)
        zthread = threading.Thread(target=zoom_thread)
        ithread = threading.Thread(target=iris_thread)

        fthread.start()
        zthread.start()
        ithread.start()

        while True:
            data = conn.recv(5)
            if not data:
                break;
            print(f"Message Recieved: {data}")
           
            if data != b'test\x00':
                num = int.from_bytes(conn.recv(4), "little")

            # Figure out what kind of data it is looking for
            if data == FOCUS_CONTROL:
                print(f"Changing focus to {num}%")

                flock.acquire()
                ftarget = num
                flock.release()
            elif data == ZOOM_CONTROL:
                print(f"Changing zoom to {num}%")
                zlock.acquire()
                ztarget = num
                zlock.release()
            elif data == IRIS_CONTROL:
                print(f"Changing iris to {num}%")
                ilock.acquire()
                itarget = num
                ilock.release()

            conn.sendall(b"conf")


        flock.acquire()
        ftarget = 500
        flock.release()
        
        zlock.acquire()
        ztarget = 500
        zlock.release()

        ilock.acquire()
        itarget = 500
        ilock.release()

        fthread.join()
        zthread.join()
        ithread.join()
