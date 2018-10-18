set i=1
set f=2

#g++ EventMatchingStep.C $(root-config --cflags --libs) -g -o EventMatchingStep.exe 


while ( $i < $f )

	root -b -l -q EventMatchingStep.C++'("'"/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/2018MBTrigger/L1SmallFiles/L1NtuplePbPbMC_$i.root"'")'
#	./ "/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/2018MBTrigger/L1SmallFiles/L1NtuplePbPbMC_$i.root"
	mv EventMatched.root EventMatchedPart$i.root

	@ i = $i + 1

end

hadd EventMatched.root EventMatchedPart*.root

#rm EventMatchingStep.exe 


