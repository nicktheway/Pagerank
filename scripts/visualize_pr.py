import numpy as np 
import os
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

#import helper functions
import helpers as hp

dataset = "web-Stanford"

pagerankFilePath = "./results/pageranks/" + dataset + ".data"
serialPagerankFilePath = "./results/serial/pageranks/" + dataset + ".data"
logFilePath = "./results/logs/" + dataset + ".txt"
serialLogFilePath = "./results/serial/logs/" + dataset + ".txt"

plt.style.use('seaborn-deep')

dataPath, loadToCrsTime, makeStochasticTime, colorTime, colorGroups, iterationTimes, errorProgression = hp.loadParallelLogData(logFilePath)
SdataPath, SloadToCrsTime, SmakeStochasticTime, SiterationTimes, SerrorProgression = hp.loadSerialLogData(serialLogFilePath)

fig = plt.figure(1)
ax = fig.subplots()
ax.tick_params(axis='y', color='C2')
cax1 = ax.plot(iterationTimes, color='C2', label='Parallel (ms)')
cax11 = ax.plot(SiterationTimes, color='C2', ls='--', label='Serial (ms)')
ax.set_ylabel('Time (ms)')
legend = ax.legend(loc=0, shadow=True, bbox_to_anchor=(1,0.8), title="Iteration Time")

ax2 = ax.twinx()
ax2.semilogy()
ax2.tick_params(axis='y', colors='C1')
cax2 = ax2.plot(errorProgression, color='C1', label='Parallel')
cax21 = ax2.plot(SerrorProgression, color='C1', ls='', marker='*', label='Serial')
ax2.set_ylabel('convergence delta')
legend2 = ax2.legend(loc=0, shadow=True, title="Convergence delta")

plt.xlim([0, iterationTimes.size])
plt.title('Pagerank calculation\'s iteration times.')

ax.grid(True, color='C2', linestyle='--', linewidth=0.2)
ax2.grid(True, color='C1', linestyle='--', linewidth=0.1)

ax.set_xlabel('Iteration #')
plt.suptitle(dataPath, fontweight='bold')
plt.show()

fig2 = plt.figure(2)
ax3 = fig2.subplots()
cax3 = ax3.plot(SiterationTimes / iterationTimes, color='C4', label='speedup')
ax3.set_xlabel('Iteration #')
plt.xlim([0, iterationTimes.size])
plt.suptitle(dataPath, fontweight='bold')
plt.title('Pagerank parallel calculation\'s speedup.')
plt.show()

prCalcTime = sum(iterationTimes)
print("Data preperation time:\n\tLoad to memory = %0.2fms\n\tColor data in %d groups = %0.2fms\n\tMake matrix stochastic = %0.2fms" % (loadToCrsTime, colorGroups, colorTime, makeStochasticTime))
print("Pagerank calculation time: %0.2fms" % prCalcTime)
print("Total Time = %0.2fms" % (prCalcTime+loadToCrsTime+makeStochasticTime+colorTime))


'''
'''
pr = np.fromfile(pagerankFilePath, dtype=float)
pr_cor = np.fromfile(serialPagerankFilePath, dtype=float)

fig4 = plt.figure(3)
ax4 = fig4.subplots()
cax4 = ax4.plot(pr-pr_cor)

plt.xlim([0, pr.size])
plt.ylim(bottom=-1e-30, top=1e-30)
plt.title("Pagerank's vector difference between\nthe two implementations.")
plt.suptitle(dataPath, fontweight='bold')
plt.xlabel('index')
plt.ylabel('pagerank')

plt.show()
'''
'''