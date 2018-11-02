#ifndef __CINT__
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TMath.h"
#include "TSystem.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom.h"
#include <iostream>
#include <fstream>
#include "include/uti.h"
#include "include/cppWatch.h"
#include "include/checkMakeDir.h"
#include "include/doGlobalDebug.h"
#include "include/etaPhiFunc.h"
#include "include/runLumiEventKey.h"
#include "include/getLinBins.h"
#include "include/L1Tools.h"
#include "include/mntToXRootdFileString.h"
#include "include/L1AnalysisEventDataFormat.h"
#include "include/L1AnalysisL1CaloTowerDataFormat.h"
#include "include/L1AnalysisL1UpgradeDataFormat.h"
#include "include/returnRootFileContentsList.h"
#include "include/histDefUtility.h"
#include "include/plotUtilities.h"
#include "include/L1AnalysisL1CaloTowerDataFormat.h"
#include "include/quickCentralityTable_20180919.h"
#include "include/parameters.h"
#include "TBranchElement.h"
#include <iostream>
#include <fstream>
//#include "parameters.h"
//#include "his.h"
#include <vector>
using namespace std;
using std::cout;
using std::endl;
#endif


void HLTEventMatching(){

	TFile * fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/HFTrigger/Forest/HiForestAOD_D0_Pthat10.root");   //Forest File - All Included//

	TTree * t1 = (TTree *) fin->Get("ppTrack/trackTree");
	TTree * t2 = (TTree *) fin->Get("hltanalysis/HltTree");
	TTree * t3 = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");

	TFile* outf = new TFile("HltMatchedTree.root","recreate");
	TDirectory* dppTrack = outf->mkdir("ppTrack", "");
	TDirectory* dhltanalysis = outf->mkdir("hltanalysis", "");
	TDirectory* dhiEvtAnalyzer = outf->mkdir("hiEvtAnalyzer", "");

	dppTrack->cd();
	TTree* new_trackTree = t1->CloneTree(0);
	dhltanalysis->cd();
	TTree* new_hlt = t2->CloneTree(0);
	dhiEvtAnalyzer->cd();
	TTree* new_hiroot = t3->CloneTree(0);



	int NEvent1 = t1->GetEntries();
	int NEvent2 = t2->GetEntries();
	int NEvent3 = t3->GetEntries();



	int run1;
	int lumi1;
	int event1;	

	int run2;
	int lumi2;
	int event2;	

	int run3;
	int lumi3;
	int event3;	

	std::vector<int> run1id;
	std::vector<int> run2id;
	std::vector<int> run3id;
	std::vector<int> event1id;
	std::vector<int> event2id;
	std::vector<int> event3id;
	std::vector<int> lumi1id;
	std::vector<int> lumi2id;
	std::vector<int> lumi3id;



	t1->SetBranchAddress("nEv",event1&);
	t1->SetBranchAddress("nLumi",lumi1&);
	t1->SetBranchAddress("nRun",run1&);

	t2->SetBranchAddress("Event",event2&);
	t2->SetBranchAddress("LumiBlock",lumi2&);
	t2->SetBranchAddress("Run",run2&);

	t3->SetBranchAddress("evt",event3&);
	t3->SetBranchAddress("lumi",lumi3&);
	t3->SetBranchAddress("run",run3&);


	for(int i = 0; i < NEvent1; i++)
	{
		if(j%10000==0) cout << "Now Working on ppTrack Tree =  " << i << endl;
		t1->GetEntry(i);
		cout << "run1  = " << run1 << "  lumi1 = " << lumi1 << "  event1= " << event1 << endl;
		run1id.push_back(run1);
		event1id.push_back(event1);
		lumi1id.push_back(lumi1);
	}

	for(int i = 0; i < NEvent2; i++)
	{
		if(j%10000==0) cout << "Now Working on ppTrack Tree =  " << i << endl;
		t2->GetEntry(i);
		cout << "run1  = " << run2 << "  lumi1 = " << lumi2 << "  event1= " << event2 << endl;
		run2id.push_back(run2);
		event2id.push_back(event2);
		lumi2id.push_back(lumi2);
	}

	for(int i = 0; i < NEvent3; i++)
	{
		if(j%10000==0) cout << "Now Working on HiEvent Tree =  " << i << endl;
		t3->GetEntry(i);
		cout << "run3  = " << run3 << "  lumi1 = " << lumi3 << "  event3= " << event3 << endl;
		run3id.push_back(run3);
		event3id.push_back(event3);
		lumi3id.push_back(lumi3);
	}
	
	for (int i = 0; i < NEvent1; i++)
	{

		cout << "run1New = " run1id[i] << endl;
	}




}
