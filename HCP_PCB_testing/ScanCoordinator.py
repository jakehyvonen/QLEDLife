import Keithley237Driver as KE237
import Keithley2010Driver as KE2010
import csv
import os
from pathlib import Path
from decimal import Decimal as dec


class IVSweepSpec:
    def __init__(self, startV, stepV, stopV):
        self.startV = startV
        self.stepV = stepV
        self.stopV = stopV


defaultSpec = IVSweepSpec(dec('0.1'),dec('0.1'),dec('5.01'))
defaultDir = os.path.expanduser('~/Documents/Lifetime_System/')
defaultName = 'Default.csv'
defaultPath = defaultDir + defaultName
def RunIVSweep(sweepSpec):
    KE2010.Initialize()
    KE237.Initialize()
    KE237.TurnOutputOn()
    presentV = sweepSpec.startV
    presentI = 0
    VIList = []
    while(presentV < sweepSpec.stopV):
        print('presentV: ' + str(presentV))
        KE237.SetVoltage(presentV)
        presentI = KE2010.MeasureCurrent()
        print('presentI: ' + str(presentI))
        VIList.append((presentV,presentI))
        presentV = presentV + sweepSpec.stepV
    KE237.TurnOutputOff()
    KE2010.GPIBReset()
    if not Path(defaultDir).exists():
        os.makedirs(defaultDir)
    with open(defaultPath,'w') as csvfile:
        ivwriter = csv.writer(csvfile, delimiter =',',
                              quotechar='|',quoting=csv.QUOTE_MINIMAL)
        for item in VIList:
            ivwriter.writerow([item[0],item[1]])
            print('V: '+str(item[0])+' I: '+str(item[1]))

RunIVSweep(defaultSpec)