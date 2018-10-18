#file path will be:
#outLFNDirBase/inputDataset/requestName/time_tag/...
from CRABClient.UserUtilities import config
config = config()
config.General.transferOutputs = True
config.General.requestName = 'LowPUPP'
config.General.workArea = 'crab_projects'
config.JobType.psetName = 'finder_pp_75X_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['rssLimit']
config.JobType.pyCfgParams = ['noprint']
config.JobType.outputFiles = ['finder_pp.root']
#config.Data.inputDataset = '/Pythia8_BuToJpsiK_TuneCUEP8M1_5020GeV_BPHMod_filter_GEN_SIM_PU_20151105/twang-Pythia8_BuToJpsiK_TuneCUEP8M1_5020GeV_BPHMod_filter_step3_20151105-61b88e8365cfdee4d0bfdfd20dfa5ba1/USER'

config.Data.inputDataset= '/ZeroBias/wangj-lowPUppRECO_Run319462_319464-eb03d3c4b9dc2f7e961c6da67074b1a2/USER'
config.Data.totalUnits = -1
config.Data.unitsPerJob = 200
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'EventAwareLumiBased'
#config.Data.outLFNDirBase = '/store/user/twang/BfinderRun2'
#config.Data.outLFNDirBase = '/store/group/phys_heavyions/HeavyFlavourRun2/BfinderRun2'
config.Data.outLFNDirBase ='store/group/phys_heavyions/zshi/MBTrigger'
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/5TeV/Cert_262081-262273_5TeV_PromptReco_Collisions15_25ns_JSON_MuonPhys_v2.txt'
#config.Site.storageSite = 'T2_US_MIT'
config.Site.storageSite = 'T2_CH_CERN'


#config.Site.whitelist = ['T2_US_MIT']
