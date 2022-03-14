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
            continue
        if col[4] == '-nan':
            continue
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


    r11 = pd.DataFrame({'time': t11, 'rsrp': rsrp11, 'rsrq': rsrq11, 'eNBi': 'eNB1'}, columns=['time', 'rsrp', 'rsrq', 'eNBi'])
    
    
#     r12 = pd.DataFrame({'time': t12, 'rsrp': rsrp12, 'rsrq': rsrq12, 'uid': 'u12'}, columns=['time', 'rsrp', 'rsrq', 'uid'])

    r21 = pd.DataFrame({'time': t21, 'rsrp': rsrp21, 'rsrq': rsrq21, 'eNBi': 'eNB2'}, columns=['time', 'rsrp', 'rsrq', 'eNBi'])
#     r22 = pd.DataFrame({'time': t22, 'rsrp': rsrp22, 'rsrq': rsrq22, 'uid': 'u22'}, columns=['time', 'rsrp', 'rsrq', 'uid'])
#     r23 = pd.DataFrame({'time': t23, 'rsrp': rsrp23, 'rsrq': rsrq23, 'uid': 'u23'}, columns=['time', 'rsrp', 'rsrq', 'uid'])

    r31 = pd.DataFrame({'time': t31, 'rsrp': rsrp31, 'rsrq': rsrq31, 'eNBi': 'eNB3'}, columns=['time', 'rsrp', 'rsrq', 'eNBi'])
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
    
    rr = pd.concat([r11, r21, r31], keys=["eNB1", "eNB2", "eNB3"])
    
    return rr

def preprocess_rsrp(dat):
    for i in range(len(dat)):
        num = dat[i]
        r = eval_rsrp_rsrq(num, i)

    return r

##########################################################################
##########################################################################

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
                snr11.append(float(scol[4]))
                st11.append(scol[0])
            elif srnti == 2:
                snr12.append(float(scol[4]))
                st12.append(scol[0])
            elif srnti == 3:
                snr13.append(float(scol[4]))
                st13.append(scol[0])
        elif scellid == 2:
            if srnti == 1:
                snr21.append(float(scol[4]))
                st21.append(scol[0])
            elif srnti == 2:
                snr22.append(float(scol[4]))
                st22.append(scol[0])
            elif srnti == 3:
                snr23.append(float(scol[4]))
                st23.append(scol[0])
        elif scellid == 3:
            if srnti == 1:
                snr31.append(float(scol[4]))
                st31.append(scol[0])
            elif srnti == 2:
                snr32.append(float(scol[4]))
                st32.append(scol[0])
            elif srnti == 3:
                snr33.append(float(scol[4]))
                st33.append(scol[0])
                


    sr11 = pd.DataFrame({'time': st11, 'snr': snr11, 'eNBi': 'eNB1'}, columns=['time', 'snr', 'eNBi'])
#     sr12 = pd.DataFrame({'time': st12, 'snr': snr12, 'uid': 'u12'}, columns=['time', 'snr', 'uid'])

    sr21 = pd.DataFrame({'time': st21, 'snr': snr21, 'eNBi': 'eNB2'}, columns=['time', 'snr', 'eNBi'])
#     sr22 = pd.DataFrame({'time': st22, 'snr': snr22, 'uid': 'u22'}, columns=['time', 'snr', 'uid'])
#     sr23 = pd.DataFrame({'time': st23, 'snr': snr23, 'uid': 'u23'}, columns=['time', 'snr', 'uid'])
    sr31 = pd.DataFrame({'time': st31, 'snr': snr31, 'eNBi': 'eNB3'}, columns=['time', 'snr', 'eNBi'])
#     sr32 = pd.DataFrame({'time': st32, 'snr': snr32, 'uid': 'u32'}, columns=['time', 'snr', 'uid'])
#     sr33 = pd.DataFrame({'time': st33, 'snr': snr33, 'uid': 'u33'}, columns=['time', 'snr', 'uid'])

#     frames1_c1 = [r11, r12, r13]
#     frames1_c2 = [r21, r22, r23]
#     frames1_c3 = [r31, r32, r33]
#     result1_c1 = pd.concat(frames1_c1, keys=["u11", "u12", "u13"])
#     result1_c2 = pd.concat(frames1_c2, keys=["u21", "u22", "u23"])
#     result1_c3 = pd.concat(frames1_c3, keys=["u31", "u32", "u33"])

#     result1 = pd.concat([sr11, sr12, sr13, sr21, sr22, sr23, sr31, sr32, sr33], keys=["u11", "u12", "u13", "u21", "u22", "u23", "u31", "u32", "u33"])

    rr1 = pd.concat([sr11, sr21, sr31], keys=["eNB1", "eNB2", "eNB3"])

                             
    return rr1

def preprocess_snr(dat):
    for i in range(len(dat)):
        num = dat[i]
        r1 = eval_snr(num, i)

    return r1

##########################################################################        
##########################################################################        
time11 = []
time21 = []
time31 = []

tput11 = []
tput21 = []
tput31 = []

tcellid = []
tcol = []
def get_tput(exa):
    for line in exa['output']['stdout'].splitlines():
        if (line.startswith("#")):
            continue
        tcol = line.split()
        tcellid = int(tcol[1])
        
        if tcellid == 1:
#             if srnti == 1:
                time11.append(float(tcol[0]))
                tput11.append(float(tcol[2]))
        elif tcellid == 2:
#             if srnti == 1:
                time21.append(float(tcol[0]))
                tput21.append(float(tcol[2]))
        elif tcellid == 3:
#             if srnti == 1:
                time31.append(float(tcol[0]))
                tput31.append(float(tcol[2]))

    tp11 = pd.DataFrame({'time': time11, 'tput': tput11, 'eNBi': 'eNB1'}, columns=['time', 'tput', 'eNBi'])
    tp21 = pd.DataFrame({'time': time21, 'tput': tput21, 'eNBi': 'eNB2'}, columns=['time', 'tput', 'eNBi'])
    tp31 = pd.DataFrame({'time': time31, 'tput': tput31, 'eNBi': 'eNB3'}, columns=['time', 'tput', 'eNBi'])

    tp = pd.concat([tp11, tp21, tp31], keys=["eNB1", "eNB2", "eNB3"])

    return tp
    
def preprocess_tput(dat):
    for i in range(1):
        num = dat[i]
        r2 = get_tput(num)
    return r2
        
##########################################################################   


res = preprocess_rsrp(res_rsrp)
res_cu11 = pd.DataFrame({'time': res.loc[res['eNBi'] == 'eNB1', 'time'], 'rsrp': res.loc[res['eNBi'] == 'eNB1', 'rsrp'], 'rsrq': res.loc[res['eNBi'] == 'eNB1', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])
res_cu21 = pd.DataFrame({'time': res.loc[res['eNBi'] == 'eNB2', 'time'], 'rsrp': res.loc[res['eNBi'] == 'eNB2', 'rsrp'], 'rsrq': res.loc[res['eNBi'] == 'eNB2', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])
res_cu31 = pd.DataFrame({'time': res.loc[res['eNBi'] == 'eNB3', 'time'], 'rsrp': res.loc[res['eNBi'] == 'eNB3', 'rsrp'], 'rsrq': res.loc[res['eNBi'] == 'eNB3', 'rsrq']} , columns=['time', 'rsrp', 'rsrq'])

res1 = preprocess_snr(res_sinr)
res1_cu11 = pd.DataFrame({'time': res1.loc[res1['eNBi'] == 'eNB1', 'time'], 'snr': res1.loc[res1['eNBi'] == 'eNB1', 'snr']} , columns=['time', 'snr'])
res1_cu21= pd.DataFrame({'time': res1.loc[res1['eNBi'] == 'eNB2', 'time'], 'snr': res1.loc[res1['eNBi'] == 'eNB2', 'snr']} , columns=['time', 'snr'])
res1_cu31= pd.DataFrame({'time': res1.loc[res1['eNBi'] == 'eNB3', 'time'], 'snr': res1.loc[res1['eNBi'] == 'eNB3', 'snr']} , columns=['time', 'snr'])

res2 = preprocess_tput(res_tput)
