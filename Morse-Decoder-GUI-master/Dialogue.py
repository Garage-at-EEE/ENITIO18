#!/usr/bin/env python3

from tkinter import *

class Dialogue:

    def create_first_dialog(self):
        self.win = Toplevel()
        self.win.wm_title("Congratulations!")
        background = Label(self.win, bg='black')
        background.pack(fill=BOTH, expand=True)
        Label(background, text="Successful decryption. Good job agents."
              , font='None 24 bold', bg='black', fg='white').pack()
        subFrame = Frame(background)
        subFrame.pack(anchor=CENTER)
        Button(self.win, text='Destroy', command=self.win.destroy).pack(in_=subFrame, side=LEFT)
        Button(self.win, text='Destroy some more!', command=self.win.destroy).pack(in_=subFrame, side=LEFT)


'''
test = Test_Windows()
root = Tk()
Button(root, text='activate', command=test.create_first_dialog).pack()
root.mainloop()'''
