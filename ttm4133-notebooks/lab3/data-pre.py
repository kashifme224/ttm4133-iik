import sys
args = sys.argv

disp = []
if args[2] == 'True':
    disp = True
elif args[2] == 'False':
    disp = False
distance = float(args[1])

ran_log = []
##########################################################################        
def get_ran_states(exa):
    for line in exa['output']['stderr'].splitlines():
        print(line)

def preprocess_ran_logs(dat):
    for i in range(1):
        num = dat[i]
        get_ran_states(num)    
##########################################################################
##########################################################################        
def get_epc_states(exa):
    for line in exa['output']['stderr'].splitlines():
        print(line)

#         if (line.startswith("#")):
#             print(line)
#             continue
#         scol = line.split()
#         time = float(scol[0])
#         imsi = int(scol[2])
#         olds.append(scol[4])
#         news.append(scol[5])
        
    
def preprocess_epc_logs(dat):
    for i in range(1):
        num = dat[i]
        get_epc_states(num)    
##########################################################################
tcol = []
tput1 = []
tput2 = []
tput3 = []
time = []

def get_tput(exa):
    for line in exa['output']['stdout'].splitlines():
        if (line.startswith("#")):
            continue
        tcol = line.split()
        time.append(float(tcol[0]))
        tput1.append(float(tcol[1]))
        tput2.append(float(tcol[2]))
        tput3.append(float(tcol[3]))
        
    t1 = pd.DataFrame({'time': time, 'tput': tput1, 'uid': 'user1'}, columns=['time', 'tput', 'uid'])
    t2 = pd.DataFrame({'time': time, 'tput': tput2, 'uid': 'user2'}, columns=['time', 'tput', 'uid'])
    t3 = pd.DataFrame({'time': time, 'tput': tput3, 'uid': 'user3'}, columns=['time', 'tput', 'uid'])
    
    result = pd.concat([t1, t2, t3], keys=["user1", "user2", "user3"])
    
    return result

def preprocess_tput(dat):
    for i in range(1):
        num = dat[i]
        res = get_tput(num)
        return res
    
##########################################################################  
##flows pre processing
# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf1(result):
#     return [float(result['output']['stdout'].split(" ")[10])]

# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf2(result):
#     return [float(result['output']['stdout'].split(" ")[33])]

# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf3(result):
#     return [float(result['output']['stdout'].split(" ")[56])]

# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf4(result):
#     return [float(result['output']['stdout'].split(" ")[79])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf1(result):
#     return [float(result['output']['stdout'].split(" ")[16])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf2(result):
#     return [float(result['output']['stdout'].split(" ")[39])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf3(result):
#     return [float(result['output']['stdout'].split(" ")[62])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf4(result):
#     return [float(result['output']['stdout'].split(" ")[85])]


# tput_f1 =[]
# tput_f2 = []
# tput_f3 = []
# tput_f4 = []

# delay_f1 =[]
# delay_f2 = []
# delay_f3 = []
# delay_f4 = []

# for s in range(1):
#     tput_f1 = campaign2.get_results_as_dataframe(get_average_throughputf1, params=params, verbose=True, parallel_parsing=True)
#     tput_f2 = campaign2.get_results_as_dataframe(get_average_throughputf2, params=params, verbose=True, parallel_parsing=True)
#     tput_f3 = campaign2.get_results_as_dataframe(get_average_throughputf3, params=params, verbose=True, parallel_parsing=True)
#     tput_f4 = campaign2.get_results_as_dataframe(get_average_throughputf4, params=params, verbose=True, parallel_parsing=True)
#     delay_f1 = campaign2.get_results_as_dataframe(get_average_delayf1, params=params, verbose=True, parallel_parsing=True)
#     delay_f2 = campaign2.get_results_as_dataframe(get_average_delayf2, params=params, verbose=True, parallel_parsing=True)
#     delay_f3 = campaign2.get_results_as_dataframe(get_average_delayf3, params=params, verbose=True, parallel_parsing=True)
#     delay_f4 = campaign2.get_results_as_dataframe(get_average_delayf4, params=params, verbose=True, parallel_parsing=True)


######################################################################main functions#################################
if(disp):
    print('####################################################')
    print('RAN Connection Setup Logs [MAC, RRC for eNB and UE]')
    print('####################################################')
    preprocess_ran_logs(ran_logs)
    print('\n')
    print('####################################################')
    print('EPC Connection Setup Logs [SGW, PGW, MME]')
    print('####################################################')
    preprocess_epc_logs(epc_logs)



res = preprocess_tput(flow_result)

# t_ue1 = pd.concat([tput_f1, tput_f3], keys=["DL", "UL"])
# t_ue2 = pd.concat([tput_f2, tput_f4], keys=["DL", "UL"])
# d_ue1 = pd.concat([delay_f1, delay_f3], keys=["DL", "UL"])
# d_ue2 = pd.concat([delay_f2, delay_f4], keys=["DL", "UL"])
