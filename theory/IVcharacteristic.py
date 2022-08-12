#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug  6 22:24:04 2022

@author: makischu

some datasheet engineering... 
describing IV-characteristic, including temperature dependency

"""

from scipy.optimize import fsolve
import numpy as np
from numpy import exp
import matplotlib.pyplot as plt

#datasheet values for my photovotaic module for E=800 W/m² ...
ISC0    = 9.42          #11.62 A
UOC0    = 38.93         #41.08 V
Impp0   = 8.71          #10.82 A
Umpp0   = 32.28         #34.63 V
T0      = 42.5          #273.15+25
Pmpp0   = Umpp0*Impp0;
# temperature coefficients (datasheet names in %/°C... i assume linearity...)
cISC    = +0.057/100.0 #100percent/K
cUOC    = -0.263/100.0 #100percent/K
cPmpp   = -0.347/100.0 #100percent/K 

#temperature range we are interested in, in Celsius, and Delta to reference.
Ts      = np.linspace(-10,60,100) # np.arange(10,60)
TsExt   = np.linspace(-20,70,100)
dTs     = Ts - T0
    
#determining characterstic's coefficients for certain temperatures
ILs     = np.zeros(np.size(Ts))
I0s     = np.zeros(np.size(Ts))
bs      = np.zeros(np.size(Ts))
Umpps   = np.zeros(np.size(Ts))
for i in range(0,len(Ts)):
    dT = dTs[i]

    ISC1    = ISC0 *(1+ dT*cISC) 
    UOC1    = UOC0 *(1+ dT*cUOC) 
    Pmpp1   = Pmpp0*(1+ dT*cPmpp)
    
    def equations(vars):
        IL,I0,b,Umpp1 = vars
        eq1 = IL - I0 - ISC1
        eq2 = IL - I0*exp(b*UOC1)
        eq3 = IL - I0*exp(b*Umpp1) - Pmpp1/Umpp1
        eq4 = Pmpp1/Umpp1 - Umpp1*exp(b*Umpp1)*I0*b
        return [eq1, eq2, eq3, eq4]

    estIL ,estI0 ,estb ,estUmpp    = (ISC1, 1e-6, 0.42, Umpp0)
    estIL,estI0,estb,estUmpp =  fsolve(equations, (estIL,estI0,estb,estUmpp))
            
    ILs[i]  = estIL
    I0s[i]  = estI0
    bs[i]   = estb
    Umpps[i]= estUmpp

    print(estIL,estI0,estb,estUmpp)
    

#fitting characterstic's coefficients
# linear is good for Umpp (not required for later)
cUm, Um0 = np.polyfit(Ts, Umpps, 1)
# linear is good for IL
cIL, IL0 = np.polyfit(Ts, ILs, 1)
# quadratic is good for b
ccb, cb, b0 = np.polyfit(Ts, bs, 2)
#I0 looks exponential but i failed to fit, so I use a high polynom
polyI0 = np.polyfit(Ts, I0s, 8)

print(cIL, IL0)
print(ccb, cb, b0)
print(polyI0)



#plotting the resuls
plt.close('all')
f, axs = plt.subplots(2,2)
axs[0,0].plot(Ts, ILs)
axs[0,0].plot(TsExt, cIL*TsExt+IL0, '--k')
axs[0,0].set_xlabel("Temp [°C]")
axs[0,0].set_ylabel("IL")
axs[0,0].grid()
axs[0,0].set_ylim((0,12))
axs[1,0].plot(Ts, I0s)
#axs[1,0].plot(TsExt, func_coeffI0(Ts, I0a, I0b, I0c), '--k')
#axs[1,0].plot(TsExt, ccI0*TsExt*TsExt+cI0*TsExt+I00, '--k')
axs[1,0].plot(TsExt, np.polyval(polyI0, TsExt), '--k')
axs[1,0].set_xlabel("Temp [°C]")
axs[1,0].set_ylabel("I0")
axs[1,0].grid()
axs[1,0].set_ylim((0,1e-5))
axs[0,1].plot(Ts, bs)
axs[0,1].plot(TsExt, ccb*TsExt*TsExt+cb*TsExt+b0, '--k')
axs[0,1].set_xlabel("Temp [°C]")
axs[0,1].set_ylabel("b")
axs[0,1].grid()
axs[0,1].set_ylim((0.3,0.6))
axs[1,1].plot(Ts, Umpps)
axs[1,1].plot(TsExt, cUm*TsExt+Um0, '--k')
axs[1,1].plot(T0, Umpp0, 'rx')
axs[1,1].set_xlabel("Temp [°C]")
axs[1,1].set_ylabel("Umpp")
axs[1,1].grid()
axs[1,1].set_ylim((0,60))
plt.show()



     
# so we can state:
def moduleI(U,TC=42.5,E=800):
    polyIOs = [ 1.87942600e-20, -4.86961403e-19 , 1.17433360e-16 , 2.68834696e-15,
       1.81678067e-13,  5.95891947e-12,  1.31406079e-10 , 1.85584490e-09,
       1.21572220e-08]
    IL = 0.00536952652515192*TC + 9.191799383372592
    I0 = np.polyval(polyIOs,TC)
    b  = 8.84231653091366e-06*TC*TC + -0.002355382115251495*TC + 0.4725462916388404
    ILE = IL*E/800
    I  = ILE - I0*np.exp(b*U)
    return I



f, axs = plt.subplots(1,1)
Us = np.linspace(0, 45, 100)
for TC in range(0,64,10): 
    axs.plot(Us, moduleI(Us,TC), label=str(TC)+'°C')
axs.plot(Us, moduleI(Us,42.5), '--k', label=str(42.5)+'°C')
axs.plot(0, ISC0, 'rx')
axs.plot(UOC0, 0, 'rx')
axs.plot(Umpp0, Impp0, 'rx')
axs.legend()
plt.xlabel("U [V]")
plt.ylabel("I [A]")
plt.xlim((0,45))
plt.ylim((0,12))
plt.grid()
plt.show()



#final check..
ISC0    = 11.62 
UOC0    = 41.08 
Impp0   = 10.82 
Umpp0   = 34.63 
f, axs = plt.subplots(1,1)
axs.plot(Us, moduleI(Us,42.5), '--k', label=str(42.5)+'°C, 800W/m²')
axs.plot(Us, moduleI(Us,25,1000), '--b', label=str(25)+'°C, 1000W/m²')
axs.plot(0, ISC0, 'rx')
axs.plot(UOC0, 0, 'rx')
axs.plot(Umpp0, Impp0, 'rx')
axs.legend()
plt.xlabel("U [V]")
plt.ylabel("I [A]")
plt.xlim((0,45))
plt.ylim((0,12))
plt.grid()
plt.show()




#illustration of multiple modules
f, axs = plt.subplots(1,2)
R = 35.33
Is = Us/R
axs[0].plot(6*Us, 6*Is, 'k', label='load')
axs[0].plot(6*Us, moduleI(Us), 'r', label='6 modules in series')
axs[0].plot(3*Us, 2*moduleI(Us), 'b--', label='2x 3modules in parallel')
axs[0].legend()
axs[0].set_xlabel("U [V]")
axs[0].set_ylabel("I [A]")
axs[0].set_xlim((0,250))
axs[0].set_ylim((0,20))
axs[0].grid()
axs[0].title.set_text("sunny (E=800)")
axs[1].plot(6*Us, 6*Is, 'k', label='load')
Elow = 100
axs[1].plot(6*Us, moduleI(Us,E=Elow), 'r--', label='6 modules in series')
axs[1].plot(3*Us, 2*moduleI(Us,E=Elow), 'b', label='2x 3modules in parallel')
axs[1].legend()
axs[1].set_xlabel("U [V]")
axs[1].set_ylabel("I [A]")
axs[1].set_xlim((0,250))
axs[1].set_ylim((0,20))
axs[1].title.set_text("cloudy (E=100)")
axs[1].grid()
plt.show()



#helper for the following
def moduleIcoeffs(TC=42.5,E=800):
    polyIOs = [ 1.87942600e-20, -4.86961403e-19 , 1.17433360e-16 , 2.68834696e-15,
       1.81678067e-13,  5.95891947e-12,  1.31406079e-10 , 1.85584490e-09,
       1.21572220e-08]
    IL = 0.00536952652515192*TC + 9.191799383372592
    I0 = np.polyval(polyIOs,TC)
    b  = 8.84231653091366e-06*TC*TC + -0.002355382115251495*TC + 0.4725462916388404
    ILE = IL*E/800
    return ILE, I0, b

#efficiency evaluation
Es = np.linspace(1,1000,1000)
T = 30
Pmpps = np.zeros(np.size(Es))
Ppars = np.zeros(np.size(Es))
Psers = np.zeros(np.size(Es))
for i in range(0,len(Es)):
    E = Es[i]
    
    #ermittle umpp & impp
    ILE, I0, b = moduleIcoeffs(TC=T,E=E)
    def dPdU(vars):
        U = vars
        eq = 1* (ILE - I0*np.exp(b*U) ) + U* (-I0*np.exp(b*U)*b )
        return eq
    Umpp =  fsolve(dPdU, 30)
    Impp = moduleI(Umpp, TC=T,E=E)
    Pmpp = 6*Umpp*Impp
    Pmpps[i] = Pmpp
    
    #ermittle schnittpunkt für seriell
    ILE, I0, b = moduleIcoeffs(TC=T,E=E)
    nI = 1
    nU = 6
    def fI(vars):
        U = vars
        eq = nI* (ILE - I0*np.exp(b*U/nU)) - U/R
        return eq
    User =  fsolve(fI, nU*Umpp)
    Iser = nI*moduleI(User/nU, TC=T,E=E)
    Pser = User*Iser
    Psers[i] = Pser
    
    #ermittle schnittpunkt für parallel
    ILE, I0, b = moduleIcoeffs(TC=T,E=E)
    nI = 2
    nU = 3
    def fI(vars):
        U = vars
        eq = nI* (ILE - I0*np.exp(b*U/nU)) - U/R
        return eq
    Upar =  fsolve(fI, nU*Umpp)
    Ipar = nI*moduleI(Upar/nU, TC=T,E=E)
    Ppar = Upar*Ipar
    Ppars[i] = Ppar
    

f, axs = plt.subplots(2,1)
Pheats = 1500*np.ones(np.size(Pmpps))
Pmpps2 = np.minimum(Pmpps,Pheats)
axs[0].plot(Es, Pmpps2, 'g--', label='at MPP, load limited')
axs[0].plot(Es, Pmpps, 'g', label='at MPP')
axs[0].plot(Es, Psers, 'r', label='at serial')
axs[0].plot(Es, Ppars, 'b', label='at parallel')
axs[0].legend()
axs[0].set_xlabel("E")
axs[0].set_ylabel("P")
axs[0].grid()
Pmaxs = np.maximum(Ppars,Psers)
Effs1 = Psers / Pmpps
Effs2 = Ppars / Pmpps
Effs3 = Pmaxs / Pmpps
Effs4 = Pmaxs / Pmpps2
axs[1].plot(Es, Effs1*100, 'r--', label='serial instead',linewidth=0.5)
axs[1].plot(Es, Effs2*100, 'b--', label='parallel instead',linewidth=0.5)
axs[1].plot(Es, Effs4*100, 'm--', label='load-limited max', linewidth=3)
axs[1].plot(Es, Effs3*100, 'm', label='max', linewidth=3)
axs[1].set_xlabel("E")
axs[1].set_ylabel("Efficiency [%]")
axs[1].set_ylim((0,110))
axs[1].legend()
axs[1].grid()
plt.show()



