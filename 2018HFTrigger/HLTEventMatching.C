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

	const int NPthat = 4;
	int Pthat[NPthat] = {10,20,50,100};
	for(int n = 0; n < NPthat; n++)
	{
	cout << "Now Working on Pthat " << 	Pthat[n] << endl;
	TString infile = Form("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/HFTrigger/Forest/HiForestAOD_D0_Pthat%d.root",Pthat[n]);
	TString infileOpenHLT = Form("/export/d00/scratch/jwang/HltPbPb2018MC/crab_openHLT_20181102_V60_D0_Pythia8_prompt_Pthat%d_Cymbal5Ev8_L1_1030.root",Pthat[n]);
	TFile * fin = new TFile(infile.Data());   //Forest File//
	TFile * finOpenHLT = new TFile(infileOpenHLT.Data());  //Open HLT File//

	TTree * t1 = (TTree *) fin->Get("ppTrack/trackTree");
	TTree * t2 = (TTree *) finOpenHLT->Get("hltbitanalysis/HltTree");
	TTree * t3 = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");

	TFile* outf = new TFile(Form("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/HFTrigger/HltMatched/D0MCMatchedPtaht%d.root",Pthat[n]),"recreate");
	TDirectory* dppTrack = outf->mkdir("ppTrack", "");
	TDirectory* dhltanalysis = outf->mkdir("hltanalysis", "");
	TDirectory* dhiEvtAnalyzer = outf->mkdir("hiEvtAnalyzer", "");

	dppTrack->cd();
	TTree* new_trackTree = t1->CloneTree(0);
	dhltanalysis->cd();
	TTree* new_hlt = t2->CloneTree(0);
	dhiEvtAnalyzer->cd();
	TTree* new_hiroot = t3->CloneTree(0);



	const int NEvent1 = t1->GetEntries();
	const int NEvent2 = t2->GetEntries();
	const int NEvent3 = t3->GetEntries();



	int Matched1;
	int Matched2;
	int NMatched1 = 0;
	int NMatched2 = 0;

	int MatchedEntry12[NEvent1];
	int MatchedEntry13[NEvent1];

	int run1;
	int lumi1;
	int event1;	

	int run2;
	int lumi2;
	ULong64_t event2;	

	UInt_t run3;
	UInt_t lumi3;
	ULong64_t event3;	

	std::vector<int> run1id;
	std::vector<int> run2id;
	std::vector<int> run3id;
	std::vector<int> event1id;
	std::vector<int> event2id;
	std::vector<int> event3id;
	std::vector<int> lumi1id;
	std::vector<int> lumi2id;
	std::vector<int> lumi3id;



	t1->SetBranchAddress("nEv",&event1);
	t1->SetBranchAddress("nLumi",&lumi1);
	t1->SetBranchAddress("nRun",&run1);

	t2->SetBranchAddress("Event",&event2);
	t2->SetBranchAddress("LumiBlock",&lumi2);
	t2->SetBranchAddress("Run",&run2);

	t3->SetBranchAddress("evt",&event3);
	t3->SetBranchAddress("lumi",&lumi3);
	t3->SetBranchAddress("run",&run3);


	for(int i = 0; i < NEvent1; i++)
	{
		if(i%10000==0) cout << "Now Working on ppTrack Tree =  " << i << endl;
		t1->GetEntry(i);
		//	cout << "run1  = " << run1 << "  lumi1 = " << lumi1 << "  event1= " << event1 << endl;
		run1id.push_back(int(run1));
		event1id.push_back(int(event1));
		lumi1id.push_back(int(lumi1));
	}

	for(int i = 0; i < NEvent2; i++)
	{
		if(i%10000==0) cout << "Now Working on ppTrack Tree =  " << i << endl;
		t2->GetEntry(i);
		//	cout << "run1  = " << run2 << "  lumi1 = " << lumi2 << "  event1= " << event2 << endl;
		run2id.push_back(int(run2));
		event2id.push_back(int(event2));
		lumi2id.push_back(int(lumi2));
	}

	for(int i = 0; i < NEvent3; i++)
	{
		if(i%10000==0) cout << "Now Working on HiEvent Tree =  " << i << endl;
		t3->GetEntry(i);
		//	cout << "run3  = " << run3 << "  lumi1 = " << lumi3 << "  event3= " << event3 << endl;
		run3id.push_back(int(run3));
		event3id.push_back(int(event3));
		lumi3id.push_back(int(lumi3));
	}

	for (int i = 0; i < NEvent1; i++)
	{
		Matched1 = 0;
		for (int j = 0; j < NEvent2; j++)
		{

			if(run1id[i] == run2id[j] && event1id[i] == event2id[j] && lumi1id[i] == lumi2id[j] )
			{
				cout << "Event Matched First Two !!!" << endl;
				cout << "Entry1 = " << i <<  "  Entry2 = " << j << endl;
				MatchedEntry12[i] = j;
				Matched1 = 1;
				NMatched1 = NMatched1 + 1;
			}
			if(Matched1 == 1) break;
		}
	}

	for (int i = 0; i < NEvent1; i++)
	{
		Matched2 = 0;
		for(int k = 0; k< NEvent3; k++)
		{
			if(run1id[i] == run3id[k] && event1id[i] == event3id[k] && lumi1id[i] == lumi3id[k])
			{
				cout << "Entry1 = " << i	<< "  Entry3 = " << k << endl;
				cout << "Event Matched Last Two !!!" << endl;
				MatchedEntry13[i] = k;
				Matched2 = 1;
				NMatched2 = NMatched2 + 1;
			}

			if(Matched2 == 1) break;
		}
	}


	for(int i = 0; i < NEvent1; i++)
	{
		if(i%1000==0)	cout << "Now Working on Entry = " << i << endl;
		t1->GetEntry(i); 
		t2->GetEntry(MatchedEntry12[i]); 
		t3->GetEntry(MatchedEntry13[i]); 
		dhiEvtAnalyzer->cd();
		new_hiroot->Fill();
		dppTrack->cd();
		new_trackTree->Fill();
		dhltanalysis->cd();
		new_hlt->Fill();
	}

	outf->Write();
	outf->Close();
	}


}
