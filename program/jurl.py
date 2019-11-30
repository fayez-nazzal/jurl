import os, glob, sys
from datetime import date
from pynput.keyboard import Key, Listener
from colorama import init, Fore, Back, Style

mode = "none"
days = []
days_names = []
current_day = 0
clear = lambda: os.system('clear')  # on Linux System

def select_mode():
    command = input(Fore.GREEN + "enter command (r,w,cd) : ")
    if command == 'cd':
        path_select()
    if command == 'r':
        show_month()
    if command == "w":
        write_day()

def path_select():
    while (True):
        clear()
        for path in glob.glob("*.m"):
            print(Fore.RED + path[0:3])
        path = input(Fore.GREEN+"select path to go to:")
        if os.path.exists(path + ".m"):
            os.chdir(path + ".m")
            clear()
            break
            print(Fore.GREEN + "you are in path " + str(path) + "\033[1;32;40m press shift to select a command")
        else:
            print(Fore.RED + "DNE")
    with Listener(
            on_press=on_press) as listener:
        listener.join()

def write_day():
    print(Fore.GREEN + "enter your jurl, to finish just press ctrl+D")
    print(Fore.WHITE)
    document = sys.stdin.read()
    today = date.today()
    file = open(str(today.day)+".ju", "w")
    file.write(document)
    with Listener(
            on_press=on_press) as listener:
        listener.join()



def show_month():
    global days
    global days_names
    days.clear()
    for d in glob.glob("*.ju"):
        days.append(d)
        days_names.append(str(d))
    days.sort()
    days_names.sort()
    if len(days)>0:
        show_day(days[current_day])
    else:
        print(Fore.GREEN + "no days or no path")

def show_day(day):
    if os.path.exists(day):
        global mode
        mode = "read days"
        to_read = open(day, 'r')
        clear()
        print(Fore.GREEN + "========= " + days_names[current_day] + "========= ")
        print(Fore.GREEN + "-> next. <- prev")
        print(Fore.WHITE + to_read.read())
    with Listener(
            on_press=on_press) as lsa:
        lsa.join()

def on_press(key):
    global mode
    global days
    global current_day
    clear = lambda: os.system('clear')
    if key == Key.shift:
        clear()
        select_mode()
        mode = "select"
    elif key == Key.left:
        if current_day-1>=0:
            current_day -= 1
        clear()
        show_day(days[current_day])
    elif key == Key.right:
        if current_day+1<len(days):
            current_day += 1
        clear()
        show_day(days[current_day])


clear()
init()
print(Fore.WHITE + Back.RESET)
path_select()

