# # for s in range(1):
# #     tmp = result[s]
# #     display(tmp['output']['stdout'].split(" ")[0].split("<")[1].split(">")[0])
# #     display(tmp['output']['stdout'].split(" ")[7].split("\n")[0])
# #     display(tmp['output']['stdout'].split(" ")[10])
# #     display(tmp['output']['stdout'].split(" ")[16])
# #     display(tmp['output']['stdout'].split(" ")[22])
# #     display(tmp['output']['stdout'].split(" ")[23].split("<")[1].split(">")[0])
# #     display(tmp['output']['stdout'].split(" ")[30].split("\n")[0])
# #     display(tmp['output']['stdout'].split(" ")[33])
# #     display(tmp['output']['stdout'].split(" ")[39])
# #     display(tmp['output']['stdout'].split(" ")[45])
    
    
# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf1(result):
#     return [float(result['output']['stdout'].split(" ")[10])]

# @sem.utils.output_labels(['Throughput [Mbit/s]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_throughputf2(result):
#     return [float(result['output']['stdout'].split(" ")[33])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf1(result):
#     return [float(result['output']['stdout'].split(" ")[16])]

# @sem.utils.output_labels(['Mean delay [ms]'])
# @sem.utils.only_load_some_files(['stdout'])
# def get_average_delayf2(result):
#     return [float(result['output']['stdout'].split(" ")[39])]

# tput_f1 = []
# tput_f2 = []
# delay_f1 = []
# delay_f2 = []

# for s in range(len(result)):
#     tput_f1.append(campaign.get_results_as_dataframe(get_average_throughputf1, params=params, verbose=True, parallel_parsing=True))
#     tput_f2.append(campaign.get_results_as_dataframe(get_average_throughputf2, params=params, verbose=True, parallel_parsing=True))


# for s in range(len(result)):
#     delay_f1.append(campaign.get_results_as_dataframe(get_average_delayf1, params=params, verbose=True, parallel_parsing=True))
#     delay_f2.append(campaign.get_results_as_dataframe(get_average_delayf2, params=params, verbose=True, parallel_parsing=True))
    
    

##########################################################################
tcol = []
tput1 = []
tput2 = []
delay1 = []
delay2 = []
time1 = []
time2 = []
def get_tput(exa):
    for line in exa['output']['stdout'].splitlines():
        if (line.startswith("#")):
            continue
        tcol = line.split()
        if(tcol[1] == 'PC'):
            time1.append(float(tcol[0]))
            tput1.append(float(tcol[2]))
            delay1.append(float(tcol[3]))
        elif(tcol[1] == 'CP'):
            time2.append(float(tcol[0]))
            tput2.append(float(tcol[2]))
            delay2.append(float(tcol[3]))

        
    t1 = pd.DataFrame({'time': time1, 'tput': tput1, 'delay': delay1, 'flow': 'UL'}, columns=['time', 'tput', 'delay', 'flow'])
    t2 = pd.DataFrame({'time': time2, 'tput': tput2, 'delay': delay2, 'flow': 'DL'}, columns=['time', 'tput', 'delay', 'flow'])
    
    result = pd.concat([t1, t2], keys=["UL", "DL"])
    
    return result

def preprocess_tput(dat):
    for i in range(1):
        num = dat[i]
        res22 = get_tput(num)
        return res22
    
res = preprocess_tput(result)
