#!/usr/bin/env python2

import RPi.GPIO as GPIO
import SimpleMFRC522


def readCard():
    reader = SimpleMFRC522.SimpleMFRC522()
    try:
        id, text = reader.read()
        print(id)
        print(text)
        return text
    finally:
        GPIO.cleanup()
readCard()
