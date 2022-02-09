##rsrp pre processing
cellid = []

rsrp11 = []
rsrp12 = []
rsrp13 = []
rsrp21 = []
rsrp22 = []
rsrp23 = []
rsrp31 = []
rsrp32 = []
rsrp33 = []

rsrq11 = []
rsrq12 = []
rsrq13 = []
rsrq21 = []
rsrq22 = []
rsrq23 = []
rsrq31 = []
rsrq32 = []
rsrq33 = []

rnti = []
time = []

t11 = []
t12 = []
t13 = []
t21 = []
t22 = []
t23 = []
t31 = []
t32 = []
t33 = []
rncol = []
rncol1 = []
rncol2 = []
rncol3 = []

def eval_rsrp_rsrq(exa,r):
#     example_result = result[10]
    for line in exa['output']['stdout'].splitlines():
        if (line.startswith("#")):
            continue
        col = line.split()
        if col[3] == '-nan':
            col[3] = -85.100010
        if col[4] == '-nan':
            col[4] = -12.500010
        cellid = int(col[1])
        rnti = int(col[2])

        if cellid == 1:
            if rnti == 1:
                rsrp11.append(float(col[3]))
                rsrq11.append(float(col[4]))
                t11.append(col[0])
            elif rnti == 2:
                rsrp12.append(float(col[3]))
                rsrq12.append(float(col[3]))
                t12.append(col[0])
            elif rnti == 3:
                rsrp13.append(float(col[3]))
                rsrq13.append(float(col[3]))
                t13.append(col[0])
        elif cellid == 2:
            if rnti == 1:
                rsrp21.append(float(col[3]))
                rsrq21.append(float(col[4]))
                t21.append(col[0])
            elif rnti == 2:
                rsrp22.append(float(col[3]))
                rsrq22.append(float(col[4]))
                t22.append(col[0])
            elif rnti == 3:
                rsrp23.append(float(col[3]))
                rsrq23.append(float(col[4]))
                t23.append(col[0])
        elif cellid == 3:
            if rnti == 1:
                rsrp31.append(float(col[3]))
                rsrq31.append(float(col[4]))
                t31.append(col[0])
            elif rnti == 2:
                rsrp32.append(float(col[3]))
                rsrq32.append(float(col[4]))
                t32.append(col[0])
            elif rnti == 3:
                rsrp33.append(float(col[3]))
                rsrq33.append(float(col[4]))
                t33.append(col[0])
                
        rncol.append(r)
        rncol1.append(exa['params']['enbDist'])
        rncol2.append(exa['params']['enablebuilding'])
        rncol3.append(exa['params']['eNBTxPowerDbm'])

    r11 = pd.DataFrame({'time': t11, 'rsrp': rsrp11, 'rsrq': rsrq11, 'uid': 'user1'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
    r12 = pd.DataFrame({'time': t12, 'rsrp': rsrp12, 'rsrq': rsrq12, 'uid': 'user2'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
    r13 = pd.DataFrame({'time': t13, 'rsrp': rsrp13, 'rsrq': rsrq13, 'uid': 'user3'}, columns=['time', 'rsrp', 'rsrq', 'uid'])

#     r21 = pd.DataFrame({'time': t21, 'rsrp': rsrp21, 'rsrq': rsrq21, 'uid': 'u21'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
#     r22 = pd.DataFrame({'time': t22, 'rsrp': rsrp22, 'rsrq': rsrq22, 'uid': 'u22'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
#     r23 = pd.DataFrame({'time': t23, 'rsrp': rsrp23, 'rsrq': rsrq23, 'uid': 'u23'}, columns=['time', 'rsrp', 'rsrq', 'uid'])

#     r31 = pd.DataFrame({'time': t31, 'rsrp': rsrp31, 'rsrq': rsrq31, 'uid': 'u31'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
#     r32 = pd.DataFrame({'time': t32, 'rsrp': rsrp32, 'rsrq': rsrq32, 'uid': 'u32'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
#     r33 = pd.DataFrame({'time': t33, 'rsrp': rsrp33, 'rsrq': rsrq33, 'uid': 'u33'}, columns=['time', 'rsrp', 'rsrq', 'uid'])

#     frames_c1 = [r11, r12, r13]
#     frames_c2 = [r21, r22, r23]
#     frames_c3 = [r31, r32, r33]
#     result_c1 = pd.concat(frames_c1, keys=["u11", "u12", "u13"])
#     result_c2 = pd.concat(frames_c2, keys=["u21", "u22", "u23"])
#     result_c3 = pd.concat(frames_c3, keys=["u31", "u32", "u33"])
    
#     result = pd.concat([r11, r12, r13, r21, r22, r23, r31, r32, r33],
#                        keys=["u11", "u12", "u13", "u21", "u22", "u23", "u31", "u32", "u33"])
    
    result = pd.concat([r11, r12, r13], keys=["user1", "user2", "user3"])
    
    return result

def preprocess_file(dat):
    for i in range(len(dat)):
        num = dat[i]
        ress = eval_rsrp_rsrq(num, i)



    return ress

#############################################################################################################
# ##sinr pre processing

scellid = []
snr11 = []
snr12 = []
snr13 = []
snr21 = []
snr22 = []
snr23 = []
snr31 = []
snr32 = []
snr33 = []

srnti = []
time = []
st11 = []
st12 = []
st13 = []
st21 = []
st22 = []
st23 = []
st31 = []
st32 = []
st33 = []
scol = []
sncol = []
sncol1 = []
sncol2 = []
sncol3 = []

def eval_snr(exa,r):
    # example_result1 = result1[0]
    for line in exa['output']['stdout'].splitlines():
        if (line.startswith("#")):
            continue
        scol = line.split()
        scellid = int(scol[1])
        srnti = int(scol[2])

        if scellid == 1:
            if srnti == 1:
                # snr11.append(float(scol[3]))
                snr11.append(float(scol[4]))
                st11.append(scol[0])
            elif srnti == 2:
                # snr12.append(float(scol[3]))
                snr12.append(float(scol[4]))
                st12.append(scol[0])
            elif srnti == 3:
                # snr13.append(float(scol[3]))
                snr13.append(float(scol[4]))
                st13.append(scol[0])
        elif scellid == 2:
            if srnti == 1:
                # snr21.append(float(scol[3]))
                snr21.append(float(scol[4]))

                st21.append(scol[0])
            elif srnti == 2:
                # snr22.append(float(scol[3]))
                snr22.append(float(scol[4]))

                st22.append(scol[0])
            elif srnti == 3:
                # snr23.append(float(scol[3]))
                snr23.append(float(scol[4]))

                st23.append(scol[0])
        elif scellid == 3:
            if srnti == 1:
                # snr31.append(float(scol[3]))
                snr31.append(float(scol[4]))

                st31.append(scol[0])
            elif srnti == 2:
                # snr32.append(float(scol[3]))
                snr32.append(float(scol[4]))

                st32.append(scol[0])
            elif srnti == 3:
                # snr33.append(float(scol[3]))
                snr33.append(float(scol[4]))

                st33.append(scol[0])
                


    sr11 = pd.DataFrame({'time': st11, 'snr': snr11, 'uid': 'user1'}, columns=['time', 'snr', 'uid'])
    sr12 = pd.DataFrame({'time': st12, 'snr': snr12, 'uid': 'user2'}, columns=['time', 'snr', 'uid'])
    sr13 = pd.DataFrame({'time': st13, 'snr': snr13, 'uid': 'user3'}, columns=['time', 'snr', 'uid'])
#     sr21 = pd.DataFrame({'time': st21, 'snr': snr21, 'uid': 'u21'}, columns=['time', 'snr', 'uid'])
#     sr22 = pd.DataFrame({'time': st22, 'snr': snr22, 'uid': 'u22'}, columns=['time', 'snr', 'uid'])
#     sr23 = pd.DataFrame({'time': st23, 'snr': snr23, 'uid': 'u23'}, columns=['time', 'snr', 'uid'])
#     sr31 = pd.DataFrame({'time': st31, 'snr': snr31, 'uid': 'u31'}, columns=['time', 'snr', 'uid'])
#     sr32 = pd.DataFrame({'time': st32, 'snr': snr32, 'uid': 'u32'}, columns=['time', 'snr', 'uid'])
#     sr33 = pd.DataFrame({'time': st33, 'snr': snr33, 'uid': 'u33'}, columns=['time', 'snr', 'uid'])

#     frames1_c1 = [r11, r12, r13]
#     frames1_c2 = [r21, r22, r23]
#     frames1_c3 = [r31, r32, r33]
#     result1_c1 = pd.concat(frames1_c1, keys=["u11", "u12", "u13"])
#     result1_c2 = pd.concat(frames1_c2, keys=["u21", "u22", "u23"])
#     result1_c3 = pd.concat(frames1_c3, keys=["u31", "u32", "u33"])

#     result1 = pd.concat([sr11, sr12, sr13, sr21, sr22, sr23, sr31, sr32, sr33], keys=["u11", "u12", "u13", "u21", "u22", "u23", "u31", "u32", "u33"])

    result1 = pd.concat([sr11, sr12, sr13], keys=["user1", "user2", "user3"])

                             
    return result1

def preprocess_file1(dat):
    for i in range(len(dat)):
        num1 = dat[i]
        res11 = eval_snr(num1, i)


    return res11



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
        res22 = get_tput(num)
        return res22
    

######################################################################main functions#################################



res = preprocess_file(result_rsrp)
res_cu11 = pd.DataFrame({'time': res.loc[res['uid'] == 'user1', 'time'], 'rsrp': res.loc[res['uid'] == 'user1', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'user1', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])
res_cu12 = pd.DataFrame({'time': res.loc[res['uid'] == 'user2', 'time'], 'rsrp': res.loc[res['uid'] == 'user2', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'user2', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])
res_cu13 = pd.DataFrame({'time': res.loc[res['uid'] == 'user3', 'time'], 'rsrp': res.loc[res['uid'] == 'user3', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'user3', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])

# res_cu11 = pd.DataFrame({'time': res.loc[res['uid'] == 'u11', 'time'], 'rsrp': res.loc[res['uid'] == 'u11', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u11', 'rsrq'], 'eNBDist': res.loc[res['uid'] == 'u11', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u11', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u11', 'eNBTxPowerDbm'], 'run': res.loc[res['uid'] == 'u11', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])
# res_cu12 = pd.DataFrame({'time': res.loc[res['uid'] == 'u12', 'time'], 'rsrp': res.loc[res['uid'] == 'u12', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u12', 'rsrq'], 'eNBDist': res.loc[res['uid'] == 'u12', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u12', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u12', 'eNBTxPowerDbm'], 'run': res.loc[res['uid'] == 'u12', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])
# res_cu13 = pd.DataFrame({'time': res.loc[res['uid'] == 'u13', 'time'], 'rsrp': res.loc[res['uid'] == 'u13', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u13', 'rsrq'], 'eNBDist': res.loc[res['uid'] == 'u13', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u13', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u13', 'eNBTxPowerDbm'], 'run': res.loc[res['uid'] == 'u13', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])


# res_cu21 = pd.DataFrame({'time': res.loc[res['uid'] == 'u21', 'time'], 'rsrp': res.loc[res['uid'] == 'u21', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u21', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u21', 'eNBDist'], 'run': res.loc[res['uid'] == 'u21', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])
# res_cu22 = pd.DataFrame({'time': res.loc[res['uid'] == 'u22', 'time'], 'rsrp': res.loc[res['uid'] == 'u22', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u22', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u22', 'eNBDist'], 'run': res.loc[res['uid'] == 'u22', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])
# res_cu23 = pd.DataFrame({'time': res.loc[res['uid'] == 'u23', 'time'], 'rsrp': res.loc[res['uid'] == 'u23', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u23', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u23', 'eNBDist'], 'run': res.loc[res['uid'] == 'u23', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])

# res_cu31 = pd.DataFrame({'time': res.loc[res['uid'] == 'u31', 'time'], 'rsrp': res.loc[res['uid'] == 'u31', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u31', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u31', 'eNBDist'], 'run': res.loc[res['uid'] == 'u31', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])
# res_cu32 = pd.DataFrame({'time': res.loc[res['uid'] == 'u32', 'time'], 'rsrp': res.loc[res['uid'] == 'u32', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u32', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u32', 'eNBDist'], 'run': res.loc[res['uid'] == 'u32', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])
# res_cu33 = pd.DataFrame({'time': res.loc[res['uid'] == 'u33', 'time'], 'rsrp': res.loc[res['uid'] == 'u33', 'rsrp'], 'rsrq': res.loc[res['uid'] == 'u33', 'rsrq'],'eNBDist': res.loc[res['uid'] == 'u33', 'eNBDist'], 'run': res.loc[res['uid'] == 'u33', 'run']} , columns=['time', 'rsrp', 'rsrq', 'eNBDist', 'run'])


res1 = preprocess_file1(result_snr)
res1_cu11 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'user1', 'time'], 'snr': res1.loc[res1['uid'] == 'user1', 'snr']} , columns=['time', 'snr'])
res1_cu12 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'user2', 'time'], 'snr': res1.loc[res1['uid'] == 'user2', 'snr']} , columns=['time', 'snr'])
res1_cu13 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'user3', 'time'], 'snr': res1.loc[res1['uid'] == 'user3', 'snr']} , columns=['time', 'snr'])

# res1_cu11 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u11', 'time'], 'snr': res1.loc[res1['uid'] == 'u11', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u11', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u11', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u11', 'eNBTxPowerDbm'], 'run': res1.loc[res1['uid'] == 'u11', 'run']} , columns=['time', 'snr', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])
# res1_cu12 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u12', 'time'], 'snr': res1.loc[res1['uid'] == 'u12', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u12', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u12', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u12', 'eNBTxPowerDbm'], 'run': res1.loc[res1['uid'] == 'u12', 'run']} , columns=['time', 'snr', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])
# res1_cu13 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u13', 'time'], 'snr': res1.loc[res1['uid'] == 'u13', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u13', 'eNBDist'], 'enablebuilding': res.loc[res['uid'] == 'u13', 'enablebuilding'], 'eNBTxPowerDbm': res.loc[res['uid'] == 'u13', 'eNBTxPowerDbm'], 'run': res1.loc[res1['uid'] == 'u13', 'run']} , columns=['time', 'snr', 'eNBDist', 'enablebuilding', 'eNBTxPowerDbm', 'run'])

# res1_cu21 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u21', 'time'], 'snr': res1.loc[res1['uid'] == 'u21', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u21', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u21', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])
# res1_cu22 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u22', 'time'], 'snr': res1.loc[res1['uid'] == 'u22', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u22', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u22', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])
# res1_cu23 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u23', 'time'], 'snr': res1.loc[res1['uid'] == 'u23', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u23', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u23', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])

# res1_cu31 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u31', 'time'], 'snr': res1.loc[res1['uid'] == 'u31', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u31', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u31', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])
# res1_cu32 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u32', 'time'], 'snr': res1.loc[res1['uid'] == 'u32', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u32', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u32', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])
# res1_cu33 = pd.DataFrame({'time': res1.loc[res1['uid'] == 'u33', 'time'], 'snr': res1.loc[res1['uid'] == 'u33', 'snr'], 'eNBDist': res1.loc[res1['uid'] == 'u33', 'eNBDist'], 'run': res1.loc[res1['uid'] == 'u33', 'run']} , columns=['time', 'snr', 'eNBDist', 'run'])

res2 = preprocess_tput(result_flow)