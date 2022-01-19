import os
import sem
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import pprint
import sys
sns.set_style("whitegrid")

args = sys.argv

pk_rate = float(args[1])
pk_size = float(args[2])
p_delay = float(args[3])
c_delay = float(args[4])

ns_path = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ns-3')
script = 'basic-test1' # ns-3 script in ../ns-3/scratch/
res_path = 'sim' + 'results-' + str(pk_rate) + '-' + str(pk_size) + str(p_delay) + '-' + str(c_delay);
campaign_dir = os.path.join(os.path.expanduser('~'), 'repos', 'sem-example', 'ttm4133-notebooks', 'lab1',
                            'results', res_path)
campaign = sem.CampaignManager.new(ns_path, script, campaign_dir, overwrite=True, #create a new sem campaign
                                   check_repo = False, max_parallel_processes=8)

params = {
    'lambda': pk_rate, #packets per second
    'udppksize': pk_size, #packet size  
    'p2pdelay': p_delay,  #ms
    'csmadelay': c_delay, #ms
#     'p2prate': list(range(1000, 1000, 5)),  #ms
#     'csmadelay': list(range(100, 100, 500)), #ms
}
runs = 1

campaign.run_missing_simulations(params, runs=runs)