#!/bin/bash

if [[ $# -ne 1 ]]
then
    echo "Usage: bash runMultiAlgo.sh <isMC>. exit 1"
    exit 1
elif [[ "$1" == 1 ]]
then
    echo "Running MC mode"
elif [[ "$1" == 0 ]]
then
    echo "Running Data mode"
else
    echo "Input $i must be 0 or 1. exit 1"
    exit 1
fi

hiMode=(0 1)
algos=(ChunkyDonut ChunkySandwich2 ChunkySandwich2Ave ChunkySandwich4 ChunkySandwich8)

dir=configs
mkdir -p $dir
DATE=`date +%Y%m%d`
mcDataTag=MC
mcDataOpt=--mc
eraTag=Run2_2018
dummyFile=/data/cmcginn/RAW/Dijet_pThat-30_HydjetCymbMB_pp_5p02_Pythia6_2018/step1_DIGI_L1_DIGI2RAW_HLT_PU_1.root
conditionStr=101X_upgrade2018_realistic_v7
emulFromRawStr=L1TReEmulMCFromRAWSimCalTP
additionalCustomise=--customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleGEN
customiseCommands=--customise_commands="process.simTwinMuxDigis.DTDigi_Source = cms.InputTag('bmtfDigis')\n process.simTwinMuxDigis.DTThetaDigi_Source = cms.InputTag('bmtfDigis')\n process.simTwinMuxDigis.RPC_Source = cms.InputTag('muonRPCDigis')\n"
inputDataSet="/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV_2018/HINPbPbSpring18DR-NoPU_100X_upgrade2018_realistic_v10-v1/GEN-SIM-RAW"
splitting=EventAwareLumiBased
unitsPerJob=1000
outTagPre=Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV_2018

if [[ "$1" == 0 ]]
then
    mcDataTag=XeXe
    eraTag=Run2_2017_pp_on_XeXe
    dummyFile=/data/cmcginn/RAW/HIMinimumBias2_XeXe2017/F6910069-C0AF-E711-99B8-02163E0138F9.root
    mcDataOpt=--data
    conditionStr=auto:run2_data
#    conditionStr=100X_dataRun2_v1
    emulFromRawStr=L1TReEmulFromRAW
    additionalCustomise=--customise=FWCore/ParameterSet/MassReplace.massReplaceInputTag
    customiseCommands=""
    inputDataSet="/HIMinimumBias2/XeXeRun2017-v1/RAW"
    splitting=LumiBased
    unitsPerJob=50
    outTagPre="HIMinimumBias2_XeXeRun2017_v1"
fi


for j in "${hiMode[@]}"
do
    for i in "${algos[@]}"
    do
	#cmsDriver
	echo cmsDriver.py l1Ntuple_$mcDataTag\_$i\_HiMode$j -s RAW2DIGI --era=$eraTag --customise=L1Trigger/Configuration/customiseReEmul.$emulFromRawStr --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU $additionalCustomise $customiseCommands --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --conditions=$conditionStr -n 100 $mcDataOpt --no_exec --no_output --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_2 --filein=file:$dummyFile
	exit

	#Post driver mods
#	echo "process.caloStage2Params.jetPUSType = cms.string('$i')" >> l1Ntuple_$mcDataTag\_$i\_HiMode$j\_RAW2DIGI.py
#	echo "process.caloStage2Params.hiMode = cms.uint32($j)" >> l1Ntuple_$mcDataTag\_$i\_HiMode$j\_RAW2DIGI.py
	mv l1Ntuple_$mcDataTag\_$i\_HiMode$j\_RAW2DIGI.py $dir/
	
	cp crabConfig_TEMPLATE.py crabConfig_$mcDataTag\_$i\_HiMode$j.py
	
	sed -i -e "s@INSERTNTUPLECONFIG@l1Ntuple_$mcDataTag\_$i\_HiMode$j\_RAW2DIGI.py@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py
	sed -i -e "s@INSERTOUTPUT@L1Ntuple.root@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py
	sed -i -e "s@INSERTINPUTDATASET@$inputDataSet@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py
	sed -i -e "s@INSERTSPLITTING@$splitting@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py
	sed -i -e "s@INSERTUNITSPERJOB@$unitsPerJob@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py
	sed -i -e "s@INSERTOUTTAG@$outTagPre\_$i\_HiMode$j\_$DATE@g" crabConfig_$mcDataTag\_$i\_HiMode$j.py

	if [[ "$1" == 0 ]]
	then
	    echo "config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/HI/Cert_304899-304907_5TeV_PromptReco_XeXe_Collisions17_JSON.txt'" >> crabConfig_$mcDataTag\_$i\_HiMode$j.py
	fi
	
	mv crabConfig_$mcDataTag\_$i\_HiMode$j.py $dir/    
	echo $i
    done
done