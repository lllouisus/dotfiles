#!/usr/bin/env python

import os
import sys
import subprocess
import re
import time
import common
import _thread
import psutil


text_color="^c#000000^^b#ffffff0x99^"
icon_color="^c#000000^^b#ffffff0x99^"
DELAY_TIME=1

filename= os.path.basename(__file__)
name=re.sub("\..*",'',filename)

def update(loop=False,exec=True):
  while True :
    icon=""
    cpu_usage=int(psutil.cpu_percent())
    if(cpu_usage>50) : icon=" "
    else : icon=" "
    cpu_usage="{:<3}".format(str(cpu_usage)+"%")
    cmd = "cat /sys/class/thermal/thermal_zone0/temp"
    result = subprocess.run(cmd, shell=True, timeout=3, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
    temperature=int(float(result.stdout.decode('utf-8').replace('\n',''))/1000)
    text=cpu_usage+""+str(temperature)+" "
    txt="^s"+str(name)+"^"+str(icon_color)+str(icon)+str(text_color)+str(text)
    common.write_to_file(txt+"\n",str(name))
    if loop == False : 
      if exec==True :
        os.system("xsetroot -name '"+str(txt)+"'")
      break
    time.sleep(DELAY_TIME)

def update_thread():
  _thread.start_new_thread(update,(False,False))

def notify(string='') :
  cmd='notify-send "  CPU tops"  "$(ps axch -o cmd:15,%cpu --sort=-%cpu | head  | '+"sed 's/$/&%/g')"+'"'+" -r 1014"
  os.system(cmd)

def click(string='') :
  match string:
    case 'L':
      notify()
    case 'M':
      pass
    case 'R':
      os.system("alacritty -t statusutil --class floatingTerminal -e btop")
      pass
    case 'U':
      pass
    case 'D':
      pass
    case  _: pass


if __name__ == "__main__":
  if len(sys.argv) > 1:
    if(sys.argv[1]=="update") :
      pass
    else :
      click(sys.argv[1])
      update(exec=False)
  else :
    update()
   
