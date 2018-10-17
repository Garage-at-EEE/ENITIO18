#!/usr/bin/env python3

import threading
import time
import pygame
import subprocess

class Sound:
    ONE_UNIT = 0.2
    TWO_UNITS = 2 * ONE_UNIT
    FIVE_UNITS = 5 * ONE_UNIT
    pygame.mixer.init()
    pygame.mixer.set_num_channels(100)
    dot = pygame.mixer.Sound("res/dot.ogg")
    dash = pygame.mixer.Sound("res/dash.ogg")

    def loadSoundEndless(self, filename):
        pygame.mixer.music.load(filename)
        pygame.mixer.music.play(-1)

    def loadSoundOnce(self, filename):
        pygame.mixer.Sound(filename).play()

    def playEndlessLoop(self, filename):
        threading.Thread(target=self.loadSoundEndless, args=(filename, )).start()

    def playSound(self, filename):
        #   threading.Thread(target=self.loadSoundOnce, args=(filename, )).start()
        subprocess.Popen(['vlc', '/home/pi/Morse-Decoder-GUI/' + str(filename)])

    def stopSound(self, filename):
        pygame.mixer.pause()
        
    def playCode(self, code):
        for letter in code:
            if letter == '-':
                self.dash.play()
                time.sleep(self.TWO_UNITS)
            elif letter == '.':
                self.dot.play()
                time.sleep(self.TWO_UNITS)
            elif letter == ' ':
                time.sleep(self.FIVE_UNITS)


    def playMorse(self, code):
        threading.Thread(target=self.playCode, args=(code, )).start()
