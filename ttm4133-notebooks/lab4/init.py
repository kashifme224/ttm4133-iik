import os
import sem
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import pprint
import matplotlib.ticker as ticker
import sys
sns.set_style("whitegrid")


args = sys.argv
distance = []
algo = []
    
a2a4 = False
a3 = False
a2a4SCT = 30
a2a4NO = 1

if(len(args)>1):
    if(len(args)==2):
        if(len(args[1])>0):
            distance = float(args[1])
    
    elif(len(args)==3):
        if(len(args[1])>0 and len(args[2])>0):
            distance = float(args[1])
            algo = args[2]
       
    elif(len(args)==4):
        if(len(args[1])>0 and len(args[2])>0 and len(args[3])>0):
            distance = float(args[1])
            algo = args[2]
            a2a4SCT = int(args[3])
       
    elif(len(args)==5):
        if(len(args[1])>0 and len(args[2])>0 and len(args[3])>0 and len(args[4])>0):
            distance = float(args[1])
            algo = args[2]
            a2a4SCT = int(args[3])
            a2a4NO = int(args[4])
else:
    print("Argument Issue")
    sys.exit()

    
if algo == 'a2a4':
    a2a4 = True
elif algo == 'a3':
    a3 = True

print("The simulation parameters are: distance = ", distance, "meters, Handover Algorithm = ", algo, ", Serving Cell Threshold = ", a2a4SCT, "dB, Neighbor Cell Offset = ", a2a4NO, "dB.")

#for rsrp values
########################################################################################
ns_path = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ns-3')
script = 'task3-1a' # ns-3 script in ../ns-3/scratch/
res_path = 'results-rsrp' + '-' + str(distance) + '-' + str(algo) + '-' + str(a2a4SCT) + '-' + str(a2a4NO);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab4', 'results', res_path)
campaign = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'd1': distance,
    'enablersrp': True,
    'a2a4Ho': a2a4,
    'a3Ho': a3,
    'a2a4SCellThreshold': a2a4SCT,
    'a2a4neighborCellOffset': a2a4NO,
}
runs = 1

campaign.run_missing_simulations(params, runs=runs)
# ########################################################################################
script = 'task3-1a' # ns-3 script in ../ns-3/scratch/
res_path = 'results-sinrenb' + '-' + str(distance) + '-' + str(algo) + '-' + str(a2a4SCT) + '-' + str(a2a4NO);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab4', 'results', res_path)
campaign1 = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'd1': distance,
    'enablesinrue': True,
    'a2a4Ho': a2a4,
    'a3Ho': a3,
    'a2a4SCellThreshold': a2a4SCT,
    'a2a4neighborCellOffset': a2a4NO,
}
runs = 1

campaign1.run_missing_simulations(params, runs=runs)
# ########################################################################################
script = 'task3-1a' # ns-3 script in ../ns-3/scratch/
res_path = 'results-tput' + '-' + str(distance) + '-' + str(algo) + '-' + str(a2a4SCT) + '-' + str(a2a4NO);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab4', 'results', res_path)
campaign2 = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'd1': distance,
    'enableInstTput': True,
    'a2a4Ho': a2a4,
    'a3Ho': a3,
    'a2a4SCellThreshold': a2a4SCT,
    'a2a4neighborCellOffset': a2a4NO,
}
runs = 1

campaign2.run_missing_simulations(params, runs=runs)

# #for sinr values
# ########################################################################################
# ns_path1 = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ns-3')
# script1 = 'task2' # ns-3 script in ../ns-3/scratch/
# res_path1 = 'results-sinr'+ '-' + str(int(txpower)) + '-' + str(int(radius));
# campaign_dir1 = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab2', 'results', str(apnd),
#                             res_path1)
# campaign1 = sem.CampaignManager.new(ns_path1, script1, campaign_dir1, overwrite=True, #create a new sem campaign
#                                    check_repo = False, max_parallel_processes=8)

# params1 = {
#     'eNBTxPowerDbm': txpower,
#     'enbDist': radius,
#     'enablebuilding': building,
#     'enablesinrenb': True,
# }
# runs1 = 1

# campaign1.run_missing_simulations(params1, runs=runs1)
# ########################################################################################

# #for throughput
# ########################################################################################
# ns_path2 = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ns-3')
# script2 = 'task2' # ns-3 script in ../ns-3/scratch/
# res_path2 = 'results-flow'+ '-' + str(int(txpower)) + '-' + str(int(radius));
# campaign_dir2= os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab2',
#                             'results', str(apnd), res_path2)
# campaign2 = sem.CampaignManager.new(ns_path2, script2, campaign_dir2, overwrite=True, #create a new sem campaign
#                                    check_repo = False, max_parallel_processes=8)

# params2 = {
#     'eNBTxPowerDbm': txpower,
#     'enbDist': radius,
#     'enablebuilding': building,
# #     'enablersrp': True,
#     'enableflowstats': True,
# #     'enablesinrue': True,
# #     'enablesinrenb': True,
# }
# runs2 = 1

# campaign2.run_missing_simulations(params2, runs=runs2)
# ########################################################################################