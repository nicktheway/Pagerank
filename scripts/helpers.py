import numpy as np 
import os

def loadParallelLogData(log_file):
    with open(log_file, "r") as f:
        logLines = f.readlines()
    
    iterationTimes = []
    errorProgression = []
    i = 0
    for line in logLines:
        a = line.split()
        if i == 0:
            dataPath = a[1]
        elif i == 1:
            loadToCrsTime = a[4]
        elif i == 2:
            colorGroups = a[2]
        elif i == 3:
            colorTime = a[6]
        elif i == 5:
            makeStochasticTime = a[4]
        if a[0] == "Iteration:":
            iterationTimes.append(a[1])
            errorProgression.append(a[5])
        i += 1

    loadToCrsTime = float(loadToCrsTime)
    makeStochasticTime = float(makeStochasticTime)
    colorTime = float(colorTime)
    colorGroups = int(colorGroups)
    iterationTimes = np.asarray(list(map(float, iterationTimes)))
    errorProgression = np.asarray(list(map(float, errorProgression)))

    dataPath = os.path.basename(dataPath)
    dataPath = os.path.splitext(dataPath)[0]
    
    return dataPath, loadToCrsTime, makeStochasticTime, colorTime, colorGroups, iterationTimes, errorProgression


def loadSerialLogData(log_file):
    with open(log_file, "r") as f:
        logLines = f.readlines()
    
    iterationTimes = []
    errorProgression = []
    i = 0
    for line in logLines:
        a = line.split()
        if i == 0:
            dataPath = a[1]
        elif i == 1:
            loadToCrsTime = a[4]
        elif i == 3:
            makeStochasticTime = a[4]
        if a[0] == "Iteration:":
            iterationTimes.append(a[1])
            errorProgression.append(a[5])
        i += 1

    loadToCrsTime = float(loadToCrsTime)
    makeStochasticTime = float(makeStochasticTime)
    iterationTimes = np.asarray(list(map(float, iterationTimes)))
    errorProgression = np.asarray(list(map(float, errorProgression)))

    dataPath = os.path.basename(dataPath)
    dataPath = os.path.splitext(dataPath)[0]

    return dataPath, loadToCrsTime, makeStochasticTime, iterationTimes, errorProgression