#from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import config
#config = Configuration()
#config.section_('General')
config = config()
config.General.transferOutputs = True
config.section_('JobType')
config.JobType.psetName = 'XeXeDum.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['L1Ntuple.root']
config.JobType.maxMemoryMB = 4000
#config.section_('User')
#config.section_('Site')
config.Site.whitelist = ['T2_US_MIT', 'T2_DE_RWTH', 'T2_UK_London_Brunel', 'T2_FR_GRIF_LLR', 'T2_US_Nebraska', 'T2_CH_CERN', 'T2_US_Vanderbilt', 'T2_US_Purdue']
#TEMPORARY USE REMOVE AFTER
#config.Site.ignoreGlobalBlacklist = True
config.Data.outLFNDirBase = '/store/group/phys_heavyions/zshi/MBTrigger/L1NutpleXeXeEMBX'
config.Site.storageSite = 'T2_CH_CERN'
#config.section_('Data')
config.Data.inputDataset = '/HIEmptyBX/XeXeRun2017-v1/RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.runRange = '304878-304890'
config.Data.unitsPerJob = 1
config.Data.totalUnits = -1
config.Data.publication = False
config.Data.outputDatasetTag = 'XeXeEMB'
config.Site.whitelist = ['T2_US_MIT', 'T2_BR_SPRACE']
