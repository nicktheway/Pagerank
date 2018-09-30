import numpy as np 
import os
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

#import helper functions
import helpers as hp

dataset = "web-BerkStan"

pagerankFilePath = "./results/pageranks/" + dataset + ".data"
serialPagerankFilePath = "./results/serial/pageranks/" + dataset + ".data"
logFilePath = "./results/logs/" + dataset + ".txt"
serialLogFilePath = "./results/serial/logs/" + dataset + ".txt"

plt.style.use('seaborn-deep')

dataPath, loadToCrsTime, makeStochasticTime, colorTime, colorGroups, iterationTimes, errorProgression = hp.loadParallelLogData(logFilePath)
SdataPath, SloadToCrsTime, SmakeStochasticTime, SiterationTimes, SerrorProgression = hp.loadSerialLogData(serialLogFilePath)
x = np.arange(0,iterationTimes.size,1)

# Time plot
fig1 = plt.figure(1)
ax1 = fig1.subplots()
ax1.tick_params(axis='y')
plt.bar(x, np.cumsum(iterationTimes), alpha=0.6, label='Parallel (ms)')
ax1.bar(x, np.cumsum(SiterationTimes), alpha=0.4, label='Serial (ms)')
ax1.set_ylabel('Cumulative Time (ms)')
ax1.set_xlabel('Iteration #')
ax1.grid(True, linestyle='--', linewidth=0.2)
legend1 = ax1.legend(loc=0, shadow=True, title="Iteration cumulative Time")
plt.xlim([-1, iterationTimes.size])
plt.title('Pagerank calculation\'s times.')
plt.suptitle(dataPath, fontweight='bold')

plt.show()

# Speed up-convergence delta plot
fig2 = plt.figure(2)
ax2_1 = fig2.subplots()

## data
speed_ups = SiterationTimes / iterationTimes
meanSpeedUp = np.cumsum(speed_ups) / np.arange(1, speed_ups.size+1)

ax2_1.plot(meanSpeedUp, label='mean speed up', linestyle='--')
ax2_1.set_xlabel('Iteration #')
ax2_1.set_ylabel('Mean speed up')
legend2_1 = ax2_1.legend(loc=0, shadow=True)

ax2_2 = ax2_1.twinx()
ax2_2.semilogy()
ax2_2.tick_params(axis='y', colors='C2')
ax2_2.plot(errorProgression, color='C2', label='Parallel')
ax2_2.plot(SerrorProgression, color='C2', ls='', marker='*', label='Serial')
ax2_2.set_ylabel('convergence delta')
ax2_2.grid(True, color='C2', linestyle='--', linewidth=0.1)
legend2_2 = ax2_2.legend(loc=0, shadow=True, bbox_to_anchor=(1,0.9), title="Convergence delta")

plt.xlim([0, iterationTimes.size])
plt.suptitle(dataPath, fontweight='bold')
plt.title('How convergence_delta, speed_up and iteration_times relate.')
plt.show()

'''
'''
# Difference plot
## data
pr = np.fromfile(pagerankFilePath, dtype=float)
pr_cor = np.fromfile(serialPagerankFilePath, dtype=float)

fig3 = plt.figure(3)
ax3 = fig3.subplots()
ax3.plot(pr-pr_cor)

plt.xlim([0, pr.size])
plt.ylim(bottom=-1e-12, top=1e-12)
plt.title("Pagerank's vector difference between\nthe two implementations.")
plt.suptitle(dataPath, fontweight='bold')
plt.xlabel('index')
plt.ylabel('difference')

plt.show()

fig4 = plt.figure(4)
ax4 = fig4.subplots()
ax4.plot(pr)
plt.suptitle(dataPath, fontweight='bold')
plt.title("Pagerank vector.")
plt.xlabel('index')
plt.ylabel('pagerank')
plt.show()
'''
'''

# Console messages
prCalcTime = sum(iterationTimes)
print("Data preperation time:\n\tLoad to memory = %0.2fms\n\tColor data in %d groups = %0.2fms\n\tMake matrix stochastic = %0.2fms" % (loadToCrsTime, colorGroups, colorTime, makeStochasticTime))
print("Pagerank calculation time: %0.2fms" % prCalcTime)
print("Total Time = %0.2fms" % (prCalcTime+loadToCrsTime+makeStochasticTime+colorTime))
print("Pagerank vector sum = %0.2f" % (sum(pr)))