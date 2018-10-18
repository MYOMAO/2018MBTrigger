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
#include "uti.h"
#include "TBranchElement.h"
#include "include/checkMakeDir.h"
#include "include/doGlobalDebug.h"
#include "include/etaPhiFunc.h"
#include "include/runLumiEvtKey.h"
#include "include/getLinBins.h"
#include "include/L1Tools.h"
#include "include/mntToXRootdFileString.h"
#include "include/L1AnalysisEventDataFormat.h"
#include "include/L1AnalysisL1CaloTowerDataFormat.h"
#include "include/L1AnalysisL1UpgradeDataFormat.h"
#include "include/returnRootFileContentsList.h"
#include "include/histDefUtility.h"
#include "include/plotUtilities.h"
#include "L1AnalysisL1CaloTowerDataFormat.h"
#include "TApplication.h"
#include "TInterpreter.h"
#include "TSystem.h"
#include "quickCentralityTable_20180919.h"
#include "parameters.h"
//#include "his.h"
using namespace std;
using std::cout;
using std::endl;


void EventMatching()
{
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);


	cout <<"Start" << endl;
	/*
	   ULong64_t event1;
	   UInt_t lumi1;
	   UInt_t run1;
	   ULong64_t event2;
	   UInt_t lumi2;
	   UInt_t run2;
	   */
	//	gROOT->ProcessLine("#include <vector>");
	//	Short_t nTower;
	//	vector<short> iet;
	ULong64_t event2;
	UInt_t lumi2;
	UInt_t run2;
	//UInt_t hiBin;

	//	TFile * fin1 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1Ntuple.root");
	//	TFile * fin2 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/finder_pp_All.root");
	//	TFile * fin1 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1NtupleNewPed56.root");
	
	//TFile * fin1 = new TFile("/eos/cms/store/group/phys_heavyions/zshi/MBTrigger/L1NtupleNewPed/L1NtupleNewPed56.root");
//	TFile * fin1 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1NtupleNewPed.root");
//	TFile * fin2 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/finder_pp_All_NewUpdated.root");
//	TFile * fin1 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1NtuplePbPbNewMC.root");
//	TFile * fin1 = new TFile("/eos/cms/store/group/phys_heavyions/zshi/MBTrigger/L1NtuplePbPbMCAll/Hydjet_Quenched_Drum5Ev8_PbPbMinBias_5020GeV_2018/crab_PbPbMCL1NtuplePedAll2/L1NtuplePbPbMCNew2.root");
//	TFile * fin2 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/finder_PbPb_All_New.root");
//	TFile *fin1= new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/XeXe/ZBSamples/L1Ntuple_120.root ");

//	TFile *fin2= new TFile("/eos/cms//store/group/phys_heavyions/zshi/MBTrigger/XeXeZB/finder_XeXe_All.root");

TFile *fin1= new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeZB/L1Ntuple_XeXe_2All.root");
TFile *fin2= new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/finder_XeXe_All.root");

	TTree * t1 = (TTree*) fin1->Get("l1EventTree/L1EventTree");
	TTree * t2 = (TTree*) fin2->Get("hiEvtAnalyzer/HiTree");
	TTree * t3 =  (TTree*) fin1->Get("ADC");
	int doConversion = 1;
	int doDirect = 0;
	const int NEvents1 = t1->GetEntries();
	const int NEvents2 = t2->GetEntries();
//	const int NEvents1 = 200000;
//	const int NEvents2 = 200000;
	//	int NEvents1 =3;
	//	int NEvents2 = t2->GetEntries();


	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();


	TH1D * hiEnergyHis = new TH1D("hiEnergyHis","hiEnergyHis",200,0,500);
	hiEnergyHis->GetXaxis()->SetTitle("hiHFEnergy (GeV)");
	hiEnergyHis->GetYaxis()->SetTitle("Counts");
	hiEnergyHis->SetTitle("Single HF Tower Energy Distribution");
	t2->Project("hiEnergyHis","hiHFEnergy");
	hiEnergyHis->Draw();
	c->SetLogy();
	c->SaveAs("SingleHFE.png");






	cout << "NEvents2 = " << NEvents2 << endl;
	int EVENT1[NEvents1];
	int LUMI1[NEvents1];
	int RUN1[NEvents1];
	int EVENT2[NEvents2];
	int LUMI2[NEvents2];
	int RUN2[NEvents2];
	cout << "Pass 2" << endl;
	int ADCSize;
	int ADCSizeMatched;
	int ADCSizeSave[NEvents1];

	int nTowers;
	int nTowersMatched;
	int nTowersSave[NEvents2];

	float hiHFEnergy[10000];
	float hiHFEnergyMatched[10000];
	float hiHF;
	int ADCBoth[10000];
	int ADCBothMatched[10000];
	
	int ADCFront[10000];
	int ADCFrontMatched[10000];
	int ADCBack[10000];
	int ADCBackMatched[10000];

	int ADCieta[10000];
	int ADCiphi[10000];
	int ADCdepth[10000];
	int HFieta[10000];
	int HFiphi[10000];

	int ADCMatchedieta[10000];
	int ADCMatchediphi[10000];
	int ADCMatcheddepth[10000];
	double ADCcharge[10000];
	double ADCchargeped[10000];
	double ADCenergy[10000];
	double ADCenergyped[10000];

	int HFMatchedieta[10000];
	int HFMatchediphi[10000];
	double ADCMatchedcharge[10000];
	double ADCMatchedchargeped[10000];
	double ADCMatchedenergy[10000];
	double ADCMatchedenergyped[10000];

	int hiBin;
	double Cent;






	cout << "Pass 3" << endl;



	cout << "Pass 4 "<< endl;


	int isave[NEvents1];
	int jsave[NEvents1];
	int indexM[NEvents2];

	int event;
	int lumi;
	int run;




	//	t->SetBranchAddress("nTower",&nTower);

	//TBranchElement *nTower = (TBranchElement*)t->GetBranch("nTower");
	//TBranchElement *iet = (TBranchElement*)t->GetBranch("iet");
	L1Analysis::L1AnalysisEventDataFormat *Event1_ = new L1Analysis::L1AnalysisEventDataFormat();
	cout << "Pass 3" << endl;

	t1->SetBranchStatus("*", 0);

	t1->SetBranchStatus("Event", 1);
	t1->SetBranchStatus("run", 1);
	t1->SetBranchStatus("lumi", 1);
	t1->SetBranchStatus("event", 1);

	t1->SetBranchAddress("Event", &Event1_);

	t1->SetBranchStatus("*", 1);
	t3->SetBranchAddress("ADCSize",&ADCSize);
	t3->SetBranchAddress("ADCBoth",ADCBoth);
	t3->SetBranchAddress("ieta",ADCieta);
	t3->SetBranchAddress("iphi",ADCiphi);
	t3->SetBranchAddress("depth",ADCdepth);
	t3->SetBranchAddress("ADCBack",ADCBack);
	t3->SetBranchAddress("ADCFront",ADCFront);

	t3->SetBranchAddress("charge",ADCcharge);
	t3->SetBranchAddress("chargeped",ADCchargeped);
	t3->SetBranchAddress("energy",ADCenergy);
	t3->SetBranchAddress("energyped",ADCenergyped);

	t2->SetBranchStatus("*", 0);

	t2->SetBranchStatus("evt", 1);
	t2->SetBranchStatus("run", 1);
	t2->SetBranchStatus("lumi", 1);
	t2->SetBranchStatus("nTowers", 1);
	t2->SetBranchStatus("hiHFEnergy", 1);
	t2->SetBranchStatus("ieta", 1);
	t2->SetBranchStatus("iphi", 1);
	t2->SetBranchStatus("hiBin", 1);
	t2->SetBranchStatus("hiHF", 1);


	t2->SetBranchAddress("evt", &event2);
	t2->SetBranchAddress("run", &run2);
	t2->SetBranchAddress("lumi", &lumi2);
	t2->SetBranchAddress("hiBin",&hiBin);
	t2->SetBranchAddress("hiHF",&hiHF);

	t2->SetBranchAddress("nTowers", &nTowers);
	t2->SetBranchAddress("hiHFEnergy", hiHFEnergy);
	t2->SetBranchAddress("ieta", HFieta);
	t2->SetBranchAddress("iphi", HFiphi);

	TString output;
	if(doPbPb == 2) output ="EventMatchedXeXe.root";
	if(doPbPb == 1) output ="EventMatchedPbPb.root";
	if(doPbPb == 0) output ="EventMatchedpp.root";

	TFile *fout = new TFile(output.Data(),"RECREATE");
	fout->cd();

	/*
	   t1->SetBranchAddress("event", &event1);
	   t1->SetBranchAddress("lumi", &lumi1);
	   t1->SetBranchAddress("run", &run1);

	   t2->SetBranchStatus("*", 0);
	   t2->SetBranchStatus("run", 1);
	   t2->SetBranchStatus("lumi", 1);
	   t2->SetBranchStatus("event", 1);


	   t2->SetBranchAddress("event", &event2);
	   t2->SetBranchAddress("lumi", &lumi2);
	   t2->SetBranchAddress("run", &run2);


*/


	cout <<"Total Events = "<< NEvents1 << endl;

	TTree* EVTMatchTree = new TTree("EVTMatchTree","EVTMatchTree");
	EVTMatchTree->Branch("ADCSizeMatched",&ADCSizeMatched,"ADCSizeMatched/I");
	EVTMatchTree->Branch("ADCBothMatched",ADCBothMatched,"ADCBothMatched[ADCSizeMatched]/I");
	EVTMatchTree->Branch("ADCBackMatched",ADCBackMatched,"ADCBackMatched[ADCSizeMatched]/I");
	EVTMatchTree->Branch("ADCFrontMatched",ADCFrontMatched,"ADCFrontMatched[ADCSizeMatched]/I");

	EVTMatchTree->Branch("nTowersMatched",&nTowersMatched,"nTowersMatched/I");
	EVTMatchTree->Branch("hiHFEnergyMatched",hiHFEnergyMatched,"hiHFEnergyMatched[nTowersMatched]/F");
	EVTMatchTree->Branch("event",&event,"event/I");
	EVTMatchTree->Branch("lumi",&lumi,"lumi/I");
	EVTMatchTree->Branch("run",&run,"run/I");
	EVTMatchTree->Branch("Cent",&Cent,"Cent/D");

	EVTMatchTree->Branch("ADCMatchedieta",ADCMatchedieta,"ADCMatchedieta[ADCSizeMatched]/I");
	EVTMatchTree->Branch("ADCMatchediphi",ADCMatchediphi,"ADCMatchediphi[ADCSizeMatched]/I");
	EVTMatchTree->Branch("ADCMatcheddepth",ADCMatcheddepth,"ADCMatcheddepth[ADCSizeMatched]/I");
	EVTMatchTree->Branch("HFMatchedieta",HFMatchedieta,"HFMatchedieta[nTowersMatched]/I");
	EVTMatchTree->Branch("HFMatchediphi",HFMatchediphi,"HFMatchediphi[nTowersMatched]/I");

	EVTMatchTree->Branch("ADCMatchedcharge",ADCMatchedcharge,"ADCMatchedcharge[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedchargeped",ADCMatchedchargeped,"ADCMatchedchargeped[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedenergy",ADCMatchedenergy,"ADCMatchedenergy[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedenergyped",ADCMatchedenergyped,"ADCMatchedenergyped[ADCSizeMatched]/D");

	float hiHFColl[NEvents2];
	float hiHFOrdered[NEvents2];

	for(int j = 0; j< NEvents1; j++){
		t1->GetEntry(j);
		t3->GetEntry(j);

		cout << "Now Saving Event " << j << endl;

		//t2->GetEntry(j);
		//	cout << " lumi1 = " << int (Event1_->lumi) << endl; 
		//	cout << " event1 = " << int(Event1_->event) << endl; 
		//	cout << " run1 = " << int(Event1_->run) << endl; 


		LUMI1[j] = int (Event1_->lumi);
		EVENT1[j] = int(Event1_->event);
		RUN1[j] = int (Event1_->run);


	}

	for(int i = 0; i< NEvents2; i++)
	{
		t2->GetEntry(i);

		LUMI2[i] = int (lumi2);
		EVENT2[i] = int(event2);
		RUN2[i] = int (run2);
		hiHFColl[i] = hiHF;


		//cout << "hiBin = " << hiBin << endl;

	}
	//	cout << " lumi2 = " << int (lumi2) << endl; 
	//		cout << " event2 = " << int(event2) << endl; 
	//		cout << " run2 = " << int(run2) << endl; 
	int matchedentries = 0;

	//	int indexP = 0;
	cout << "NEvents2 = " << NEvents2 << endl;
	for(int i = 0; i< NEvents2; i++)
	{
		indexM[i] = 0;
		for(int j = 0; j< NEvents2;j++)
		{
			// if(hiHFColl[i] > hiHFColl[j]) indexP = indexP + 1;
			if(hiHFColl[i] < hiHFColl[j]) indexM[i] = indexM[i] + 1;


		}
		cout << "indexM = " << indexM[i] << endl; 
		cout << "HIHF = " << hiHFColl[i]  << endl;

		hiHFOrdered[i] = hiHFColl[indexM[i]];

	}



	for(int j = 0; j < NEvents1; j++)
	{

		//cout << "Processing Event1 " << j << endl; 

		for(int i = 0; i < NEvents2; i++)
		{
			bool matched = false;
			//cout << "Processing Event2 " << i << endl; 
	

				if( LUMI1[j] == LUMI2[i] && EVENT1[j] == EVENT2[i] && RUN1[j] == RUN2[i] )
				{
					cout << "Event 1  = "<< j <<"   and Event 2 = " << i << "    Matched !!!" << endl;
					matched = true;

					isave[matchedentries] = i; 	
					jsave[matchedentries] = j; 	

					matchedentries = matchedentries + 1;


				}

			if(matched) break;

		}
	}

	for(int n = 0; n< matchedentries; n++){
		t1->GetEntry(jsave[n]);
		t3->GetEntry(jsave[n]);
		t2->GetEntry(isave[n]);
		cout << "jsave = " << jsave[n] << 	 "   isave = " << isave[n] << endl;	

		//cout << "Now Matching Entry " << j << endl;

		//t2->GetEntry(j);
		//	cout << " lumi1 = " << int (Event1_->lumi) << endl; 
		//	cout << " event1 = " << int(Event1_->event) << endl; 
		//	cout << " run1 = " << int(Event1_->run) << endl; 

		event = int(event2);
		lumi = int(lumi2);
		run = int(run2);
		cout << "hiHF = " << hiHF << endl;




		if(doConversion==1){

			/*
			   if(hiHF > 9.37139 && hiHF <= 13.6986) Cent = 97.5;
			   if(hiHF > 13.6986 && hiHF <= 17.4492) Cent = 92.5;
			   if(hiHF > 17.4492 && hiHF <= 22.8702) Cent = 87.5;
			   if(hiHF > 22.8702 && hiHF <= 33.6576) Cent = 82.5;
			   if(hiHF > 33.6576 && hiHF <= 52.9814) Cent = 77.5;
			   if(hiHF > 52.9814 && hiHF <= 78.2659) Cent = 72.5;
			   if(hiHF > 78.2659 && hiHF <= 117.024) Cent = 67.5;
			   if(hiHF > 117.024 && hiHF <= 171.06) Cent = 62.5;
			   if(hiHF > 171.06 && hiHF <= 245.175) Cent = 57.5;
			   if(hiHF > 245.175 && hiHF <= 336.999) Cent = 52.5;
			   if(hiHF > 336.999 && hiHF <= 453.073) Cent = 47.5;
			   if(hiHF > 453.073 && hiHF <= 608.359) Cent = 42.5;
			   if(hiHF > 608.359 && hiHF <= 793.882) Cent = 37.5;
			   if(hiHF > 793.882 && hiHF <= 1023.19) Cent = 32.5;
			   if(hiHF > 1023.19 && hiHF <= 1305.29) Cent = 27.5;
			   if(hiHF > 1305.29 && hiHF <= 1625.64) Cent = 22.5;
			   if(hiHF > 1625.64 && hiHF <= 2027.56) Cent = 17.5;
			   if(hiHF > 2027.56 && hiHF <= 2501.61) Cent = 12.5;
			   if(hiHF > 2501.61 && hiHF <= 3096.57) Cent = 7.5;
			   if(hiHF > 3096.57 && hiHF <= 4685.85) Cent = 2.5;
			   */
			Cent = getHiBinFromHiHF(hiHF)/2.0;


		}
		//cout << "hiBin = " << int(hiBin) << endl;

		if(doDirect==1){
			Cent = indexM[n]/9857.0*100.0;
		}
		cout << "Cent = " << Cent << endl;

		ADCSizeMatched = ADCSize;
		cout << "ADCSize = " << ADCSize << endl;
		cout << "nTowers = " << nTowers << endl;
		for(int k = 0; k < ADCSize; k++)
		{
			ADCBothMatched[k] = ADCBoth[k];
			ADCMatchedieta[k]=ADCieta[k];
			ADCMatchediphi[k]=ADCiphi[k];
			ADCMatcheddepth[k]=ADCdepth[k];
			ADCBackMatched[k] = ADCBack[k];
			ADCFrontMatched[k] = ADCFront[k];

			ADCMatchedcharge[k]=ADCcharge[k];
			ADCMatchedchargeped[k]=ADCchargeped[k];
			ADCMatchedenergy[k]=ADCenergy[k];
			ADCMatchedenergyped[k]=ADCenergyped[k];

		}
		nTowersMatched = nTowers;


		for(int l = 0; l < nTowers; l++)
		{
			hiHFEnergyMatched[l] = hiHFEnergy[l];
			//	cout << "hiHFEnergy = " << hiHFEnergy[l] << endl;
			HFMatchedieta[l] = HFieta[l];
			HFMatchediphi[l] = HFiphi[l];

		}


		EVTMatchTree->Fill();




	}


	EVTMatchTree->Write();
	fout->Close();

}
