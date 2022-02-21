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

distance = float(args[1])
# radius = float(args[2])
# buidling =[]
# apnd = []
# if args[3] == 'True':
#     building = True
#     apnd = 'urban'
# elif args[3] == 'False':
#     building = False
#     apnd = 'rural'


#for rsrp values
########################################################################################
ns_path = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ns-3')
script = 'task2-1' # ns-3 script in ../ns-3/scratch/
res_path = 'results-uestates' + str(distance);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab3', 'results', res_path)
campaign = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True,optimized=False, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'd1': distance,
}
log_components = {
    'LteEnbMac': 'info',
    'LteEnbRrc': 'info',
    'LteRrcProtocolIdeal': 'info',
    'LteUeRrc': 'info',
    'LteEnbRrc': 'debug',
    'LteRrcProtocolIdeal': 'debug',
}
runs = 1

campaign.run_missing_simulations(params, runs=runs, log_components=log_components)
########################################################################################
script = 'task2-1' # ns-3 script in ../ns-3/scratch/
res_path = 'results-enbstates' + str(distance);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab3', 'results', res_path)
campaign1 = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True,optimized=False, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'd1': distance,
}
runs = 1
log_components = {
    'EpcMmeApplication': 'debug',
    'EpcPgwApplication': 'debug',
    'EpcEnbApplication': 'info',
    'EpcSgwApplication': 'info',
}
campaign1.run_missing_simulations(params, runs=runs, log_components=log_components)
########################################################################################
script = 'task2-1' # ns-3 script in ../ns-3/scratch/
res_path = 'results-flow' + str(distance);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab3', 'results', res_path)
campaign2 = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'enableInstTput': True,
    'd1': distance,
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