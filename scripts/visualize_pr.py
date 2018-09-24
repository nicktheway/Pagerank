import numpy as np 
import os
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

pagerankFilePath = "./results/serial/pageranks/web-Google.data"
validationFilePath = "./results/serial/result.data"
logFilePath = "./results/serial/logs/web-Google.txt"

iterationTimes = []
errorProgression = []

with open(logFilePath, "r") as f:
    logLines = f.readlines()
    
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

# Get the database name from the log file's path.
dataPath = os.path.basename(dataPath)
dataPath = os.path.splitext(dataPath)[0]

fig, ax = plt.subplots()
ax.tick_params(axis='y', colors='blue')
cax = ax.plot(iterationTimes, color='blue')
ax.set_ylabel('Time (ms)')

ax2 = ax.twinx()
ax2.semilogy()
ax2.tick_params(axis='y', colors='red')
cax2 = ax2.plot(errorProgression, color='red')
ax2.set_ylabel('convergence delta')

plt.xlim([0, iterationTimes.size])
plt.title('Pagerank calculation\'s iteration times.')

plt.xlabel('Iteration #')
plt.suptitle(dataPath, fontweight='bold')
plt.show()

prCalcTime = sum(iterationTimes)
print("Data preperation time:\n\tLoad from memory = %0.2fms\n\tMake matrix stochastic = %0.2fms" % (loadToCrsTime, makeStochasticTime))
print("Pagerank calculation time: %0.2fms" % prCalcTime)
print("Total Time = %0.2fms" % (prCalcTime+loadToCrsTime+makeStochasticTime))

pr = np.fromfile(pagerankFilePath, dtype=float)
pr_cor = np.fromfile(validationFilePath, dtype=float)

#pr_cor = np.fromfile(validationFilePath, dtype=float)

fig2, ax3 = plt.subplots()
cax3 = ax3.plot(pr)

plt.xlim([0, pr.size])
plt.ylim(bottom=0)
plt.title(dataPath, fontweight='bold')
plt.xlabel('index')
plt.ylabel('pagerank')

plt.show()


