#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (c) 2010-2013 Roger Light <roger@atchoo.org>
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Distribution License v1.0
# which accompanies this distribution.
#
# The Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
#
# Contributors:
#    Roger Light - initial implementation
# Copyright (c) 2010,2011 Roger Light <roger@atchoo.org>
# All rights reserved.

# This shows a simple example of an MQTT subscriber using connect_srv method.
from threading import Thread
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

def on_connect(mqttc, obj, flags, rc):
    print("Connected to %s:%s" % (mqttc._host, mqttc._port))

def on_message(mqttc, obj, msg):
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    publish.single(msg.topic, msg.payload, hostname="127.0.0.1")

def on_message2(mqttc, obj, msg):
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    publish.single(msg.topic, msg.payload, hostname="10.50.11.153")


def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)

class Thr1(Thread):
    def __init__ (self, num):
          Thread.__init__(self)
          self.num = num

    def run(self):
        # but note that the client id must be unique on the broker. Leaving the client
        # id parameter empty will generate a random id for you.
        mqttc = mqtt.Client()
        mqttc.on_message = on_message
        mqttc.on_connect = on_connect
        mqttc.on_publish = on_publish
        mqttc.on_subscribe = on_subscribe
        # Uncomment to enable debug messages
        #mqttc.on_log = on_log
        mqttc.connect("10.50.11.153", 1883, 60)
        mqttc.subscribe("/cagent/+/+/notify", 0)
        mqttc.subscribe("/cagent/+/+/deviceinfo", 0)
        rc = 0
        while rc == 0:
            rc = mqttc.loop()

class Thr2(Thread):
    def __init__ (self, num):
          Thread.__init__(self)
          self.num = num
    def run(self):
        mqttc2 = mqtt.Client()
        mqttc2.on_message = on_message2
        # Unc2omment to enable debug messages
        #mqtt2c.on_log = on_log2
        mqttc2.connect("127.0.0.1", 1883, 60)
        mqttc2.subscribe("/cagent/admin/0000000BABC80F4A/agentcallbackreq", 0)
        rc = 0
        while rc == 0:
            rc = mqttc2.loop()
            print("rc: "+str(rc))


a = Thr1(1);
a.start();
b = Thr2(2);
b.start();
