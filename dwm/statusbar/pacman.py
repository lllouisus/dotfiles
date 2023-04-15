#!/usr/bin/env python

import os
import sys
import subprocess
import re
import time
import _thread
import common


text_color="^c#4169e1^^b#7fffd40x99^"
icon_color="^c#4169e1^^b#7fffd40x99^"
DELAY_TIME=18000

filename= os.path.basename(__file__)
name=re.sub("\..*",'',filename)

def get_update_packages_nums():
  num=0
  os.system("notify-send 'Get update info... ... ...' -r 1011")
  os.system("sudo pacman -Sy")
  os.system("notify-send 'Get Update info completed !' -r 1011")
  cmd="echo $(pacman -Qu | grep -Fcv '[ignored]' )"
  result = subprocess.run(cmd, shell=True, timeout=3, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
  num=int(result.stdout.decode('utf-8').replace('\n',''))
  os.system("notify-send 'Totally "+str(num)+" packages can update' -r 1011")
  return str(num)

def update_packages():
  num=get_update_packages_nums()
  os.system("notify-send 'Preparing update ... ... ...' -r 1012")
  os.system("sudo pacman -Su")
  os.system("notify-send 'Update completed' -r 1012")



def update(loop=False,exec=True):
  while True :
    icon=" "
    text=str(get_update_packages_nums())+" "
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
  pass

def click(string='') :
  match string:
    case 'L':
      notify()
    case 'M':
      pass
    case 'R':
      update_packages()
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
      update(exec=False)
      click(sys.argv[1])
      update(exec=False)
  else :
    update()
 
