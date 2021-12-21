from flask import Flask,render_template,request
import pygame
import pygame.camera
import time
app = Flask(__name__)
dEbug = False
if (dEbug):
    pathToConf = "waterer/cont_debug.txt"
else:
    pathToConf = "waterer/conf/conf.txt"
# function used to take a list of toggle values, pre defined in config conf.txt
# should use a bool for space effeciency but more readable this way.
def updateList(path):
    ListOfVals = [0,0,1,1,1,1,0,0,0]
    file1 = open(path,"r")
    for i in range(len(ListOfVals)):
        k = int(file1.readline())
        ListOfVals[i]=k
        #print(k)
    file1.close()
    return ListOfVals
# this is used to change config file back after a method has been run
def updateFile(IList):
    file1 = open(pathToConf,"w")
    for i in range(len(IList)):
        file1.write(str(IList[i])+"\n")
    file1.close()
# this is used to get right image for the website display to show that corrisponds to the actuator states
def giveCorrectPath(IList):
    tup = ["nul","nul","nul"]
    if (IList[1]):
        tup[0]= "tapIcon1.gif"
    else:
        tup[0]= "tapIcon0.gif"
    if (IList[4]):
        tup[1]= "fanIcon1.gif"
    else:
        tup[1]= "fanIcon0.gif"
    if (IList[7]):
        tup[2]= "light1.gif"
    else:
        tup[2]= "light0.gif"
    return tup
# used to generate an image for display on website
def TakePicture():
    pygame.camera.init()
    pygame.camera.list_cameras() #Camera detected or not
    cam = pygame.camera.Camera("/dev/video0",(300,300))
    cam.start()
    img = cam.get_image()
    ts = str(time.time())
    path = "photos/pic-"+ts.replace(".", "-")+".jpg"
    pygame.image.save(img,"static/"+path)
    cam.stop()
    return path
#Here is the web server stuff
@app.route("/")
def buttonTest():
    ListOfVals = updateList(pathToConf)
    state = giveCorrectPath(ListOfVals)
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name="main.png",WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])
#After toggle button on fan hit
@app.route("/toggle_fan",methods=["POST"])
def fanToggle():

    ListOfVals = updateList(pathToConf)
    ListOfVals[4] =int(not(ListOfVals[4]))

    updateFile(ListOfVals)
    state = giveCorrectPath(ListOfVals)
    path = TakePicture()
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name=path,WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])
#After toggle button on light hit
@app.route("/toggle_light",methods=["POST"])
def lightToggle():
    ListOfVals = updateList(pathToConf)
    ListOfVals[7] =int(not(ListOfVals[7]))
    updateFile(ListOfVals)
    state = giveCorrectPath(ListOfVals)
    path = TakePicture()
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name=path,WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])
#After toggle button on water hit
@app.route("/toggle_water",methods=["POST"])
def waterToggle():
    ListOfVals = updateList(pathToConf)
    ListOfVals[1] =int(not(ListOfVals[1]))
    updateFile(ListOfVals)
    state = giveCorrectPath(ListOfVals)
    path = TakePicture()
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name=path,WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])
# After reset hit
@app.route("/sendDefault",methods=["POST"])
def defaultReset():
    ListOfVals = updateList("waterer/conf/default.txt")
    updateFile(ListOfVals)
    state = giveCorrectPath(ListOfVals)
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0], name="main.png",WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])
# used when updated configs are sent
@app.route("/send",methods=["POST"])
def FormSubmission():
    ListOfVals = updateList(pathToConf)
    n = int(request.form.get("numberIn"))
    type = str(request.form.get("Relay"))
    if (type == "FOT"):
        ListOfVals [3]= n
    elif(type == "FOFT"):
        ListOfVals [5]= n
    elif(type == "LOT"):
         ListOfVals [6]= n
    elif(type == "LOFT"):
        ListOfVals [8]= n
    elif(type == "WOT"):
        ListOfVals [0]= n;
    elif(type == "WOFT"):
        ListOfVals [2]= n
    updateFile(ListOfVals)
    state = giveCorrectPath(ListOfVals)
    path = TakePicture()
    return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name=path,WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])

# used to get new picture of plant
@app.route("/wantPic",methods=["POST"])
def updatePic():
   ListOfVals = updateList(pathToConf)
   path = TakePicture()
   state = giveCorrectPath(ListOfVals)
   return render_template("index.html",fanButton=state[1],lightButton=state[2],waterButton=state[0],name=path,WaterOnT=ListOfVals[0],WaterOffT=ListOfVals[2],FanOnT=ListOfVals[3],FanOffT=ListOfVals[5],LightOnT=ListOfVals[6],LightOffT=ListOfVals[8])

if __name__ == '__main__':
   app.debug=True
   app.run(host="0.0.0.0") # accessable at [ip]:5000 on any computer in the network
