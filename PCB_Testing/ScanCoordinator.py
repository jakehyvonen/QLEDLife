import Keithley237Driver as KE237
import Keithley2010Driver as KE2010
import DataProcessor as dp
from decimal import Decimal as dec


class IVSweepSpec:
    def __init__(self, startV, stepV, stopV):
        self.startV = startV
        self.stepV = stepV
        self.stopV = stopV


defaultSpec = IVSweepSpec(dec('0.1'),dec('0.77'),dec('5.01'))
HCPNumber = '004'
HCPName = 'HCP'+str(HCPNumber)
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
    dp.SaveListToCsv(VIList, name=HCPName)
    dp.FitLineFromCsv(name = HCPName)

RunIVSweep(defaultSpec)