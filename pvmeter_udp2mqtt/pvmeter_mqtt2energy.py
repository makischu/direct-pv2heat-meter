#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#Copyright (C) 2023 makischu

#pvmeter_mqtt2energy.py translates U and I into P and E
# (simulates an energy counter based on U and I measurement)

# requires paho-mqtt. for anaconda:
# conda install -c conda-forge paho-mqtt   

import sys
import paho.mqtt.client as mqtt
import json 
import signal
from time import sleep, time
import datetime as dt

run = True

topicIn  = 'dev/pvmeter/telemetry'
topicOut = 'dev/pvmeter/energy'

clientLocal = mqtt.Client()
clientLocal.connect("192.168.2.43", 1883, 60)


def handler_stop_signals(signum, frame):
    global run
    run = False

signal.signal(signal.SIGINT, handler_stop_signals)
signal.signal(signal.SIGTERM, handler_stop_signals)


tE_s  = time()
E_Ws  = 0.0
E_day = dt.date.today()

def on_message(client, userdata, message):
    global tE_s
    global E_Ws
    global E_day
    if message.topic == topicIn:
        try:  
            #avoid: json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1
            jmessage = message.payload[:-1].decode().replace(' ','').replace(',','","').replace(':','":""').replace('{','{"').replace('}','"}').replace('"""','"').replace('""','"')   
            data = json.loads(jmessage)
            
            U = data['UoV']
            I = data['IoA']
            on =data['Sta'][-2]
            ry =data['Sta'][-1]
            TiC = data['TiC']
            TeC = data['TeC']
            
            P = 0.0
            if on == '1':
                P = float(U)*float(I)
                t_now = time()
                deltat = t_now - tE_s
                deltat = min([5.0,deltat])
                E_Ws = E_Ws + P*deltat 
                tE_s = t_now
            
            E_kWh = E_Ws / 3600.0 / 1000.0
            
            data_out = {'P_DC[W]':"{:.1f}".format(P), 
                        'E_day[kWh]':"{:.3f}".format(E_kWh),
                        'T_in[C]':TiC,
                        'T_pv[C]':TeC,
                        'highpowermode':ry,
                        'outputon':on,
                        'U_out[V]':U,
                        'I_out[A]':I,
                        }
            
            if E_day != dt.date.today():
                E_day = dt.date.today()
                E_Ws = 0.0
            
            clientLocal.publish(topicOut, json.dumps(data_out))
        except: 
            print("ignoring some exception")
            pass
        

clientLocal.on_message = on_message;
clientLocal.subscribe(topicIn)
clientLocal.loop_start()

while run:
    try:
        sleep(0.1)
    except:
        print("Unexpected error:", sys.exc_info()[0])
        raise
    sleep(0.1)    

clientLocal.loop_stop()