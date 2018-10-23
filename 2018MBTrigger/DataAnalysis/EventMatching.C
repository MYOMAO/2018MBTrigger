//ROOT
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
#include "TBranchElement.h"
#include "TApplication.h"
#include "TInterpreter.h"
#include "TSystem.h"

//cpp dependencies
#include <iostream>
#include <fstream>
#include <map>

//local dependencies
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

//#include "his.h"
using namespace std;
using std::cout;
using std::endl;


void EventMatching()
{
	cppWatch timer;
	timer.start();

	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);

	int doZB = 0;
	int doEMBX = 1;
	int donew = 1;
	
	ULong64_t event2;
	UInt_t lumi2;
	UInt_t run2;

	TFile *fin1;

	TFile *fin2;  

	if(donew == 1){
		fin1 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/L1Ntuple_XeXe_EMBX_All_WithET.root", "READ");
		fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/finder_XeXe_EMBX_All_AllE.root", "READ");
	}

	fin1->cd();
	TTree * t1 = (TTree*) fin1->Get("l1EventTree/L1EventTree");
	TTree * t3 =  (TTree*) fin1->Get("ADC");
	TTree * t4 = (TTree*) fin1->Get("l1CaloTowerEmuTree/L1CaloTowerTree");

	fin2->cd();
	TTree * t2 = (TTree*) fin2->Get("hiEvtAnalyzer/HiTree");
	int doConversion = 1;
	int doDirect = 0;
	//const int NEvents1 = 1000000;
	const int NEvents1 = t1->GetEntries();
	const int NEvents2 = t2->GetEntries();


	std::cout << "Drawing to hist" << std::endl;
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

	delete hiEnergyHis;
	delete c;
	int nTowers;
	float hiHFEnergy[10000];
	int HFMatchedieta[10000];


	std::cout << "Completed draw" << std::endl;

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

	short ADCnTower;
	int ADCnTowerMatched;


	int ADCFront[10000];
	int ADCFrontMatched[10000];
	int ADCBack[10000];
	int ADCBackMatched[10000];

	short iet[10000];
	int ietMatched[10000];

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

	int event;
	int lumi;
	int run;

	L1Analysis::L1AnalysisEventDataFormat *Event1_ = new L1Analysis::L1AnalysisEventDataFormat();
//	L1Analysis::L1AnalysisL1CaloTowerDataFormat * ADCnTower = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
    L1Analysis::L1AnalysisL1CaloTowerDataFormat *towers_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();

	
	cout << "Pass 3" << endl;
	t4->SetBranchStatus("*", 1);

	t4->SetBranchAddress("L1CaloTower", &towers_);
//	t4->SetBranchAddress("iet", iet);
	t4->SetBranchAddress("nTower", &ADCnTower);




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
	/*
	   t2->SetBranchStatus("nTowers", 1);
	   t2->SetBranchStatus("hiHFEnergy", 1);
	   t2->SetBranchStatus("ieta", 1);
	   t2->SetBranchStatus("iphi", 1);
	   t2->SetBranchStatus("hiBin", 1);
	   t2->SetBranchStatus("hiHF", 1);
	   */

	t2->SetBranchAddress("evt", &event2);
	t2->SetBranchAddress("run", &run2);
	t2->SetBranchAddress("lumi", &lumi2);
	/*
	   t2->SetBranchAddress("hiBin",&hiBin);
	   t2->SetBranchAddress("hiHF",&hiHF);

	   t2->SetBranchAddress("nTowers", &nTowers);
	   t2->SetBranchAddress("hiHFEnergy", hiHFEnergy);
	   t2->SetBranchAddress("ieta", HFieta);
	   t2->SetBranchAddress("iphi", HFiphi);
	   */

	//Lets build the map w/ offline ttree (t2)
	std::cout << "Build map" << std::endl;
	std::map<unsigned long long, int> runLumiEvtMap;
	for(Int_t entry = 0; entry < NEvents2; ++entry){
		t2->GetEntry(entry);

		unsigned long long key = keyFromRunLumiEvent(run2, lumi2, event2);
		runLumiEvtMap[key] = entry;
	}

	t2->SetBranchStatus("*", 0);
	t2->SetBranchStatus("nTowers", 1);
	t2->SetBranchStatus("hiHFEnergy", 1);
	t2->SetBranchStatus("ieta", 1);
	t2->SetBranchStatus("iphi", 1);
	t2->SetBranchStatus("hiBin", 1);
	t2->SetBranchStatus("hiHF", 1);

	t2->SetBranchAddress("hiBin",&hiBin);
	t2->SetBranchAddress("hiHF",&hiHF);  
	t2->SetBranchAddress("nTowers", &nTowers);
	t2->SetBranchAddress("hiHFEnergy", hiHFEnergy);
	t2->SetBranchAddress("ieta", HFieta);
	t2->SetBranchAddress("iphi", HFiphi);

	TString output;
	if(doPbPb == 2)
	{
		if(doZB == 1)  output ="EventMatchedXeXe.root";
		if(doEMBX == 1)  {
			if(donew == 0) output ="EventMatchedXeXeEMBX.root";
			if(donew == 1) output ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/EventMatchedXeXeEMBXNew.root";
		}

	}
	if(doPbPb == 1) output ="EventMatchedPbPb.root";
	if(doPbPb == 0) output ="EventMatchedpp.root";

	TFile *fout = new TFile(output.Data(),"RECREATE");
	fout->cd();
	TTree* EVTMatchTree = new TTree("EVTMatchTree","EVTMatchTree");
	EVTMatchTree->Branch("ADCSizeMatched",&ADCSizeMatched,"ADCSizeMatched/I");
	EVTMatchTree->Branch("ADCnTowerMatched",&ADCnTowerMatched,"ADCnTowerMatched/I");

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
	EVTMatchTree->Branch("ietMatched",ietMatched,"ietMatched[ADCnTowerMatched]/I");

	EVTMatchTree->Branch("ADCMatcheddepth",ADCMatcheddepth,"ADCMatcheddepth[ADCSizeMatched]/I");
	EVTMatchTree->Branch("HFMatchedieta",HFMatchedieta,"HFMatchedieta[nTowersMatched]/I");
	EVTMatchTree->Branch("HFMatchediphi",HFMatchediphi,"HFMatchediphi[nTowersMatched]/I");

	EVTMatchTree->Branch("ADCMatchedcharge",ADCMatchedcharge,"ADCMatchedcharge[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedchargeped",ADCMatchedchargeped,"ADCMatchedchargeped[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedenergy",ADCMatchedenergy,"ADCMatchedenergy[ADCSizeMatched]/D");
	EVTMatchTree->Branch("ADCMatchedenergyped",ADCMatchedenergyped,"ADCMatchedenergyped[ADCSizeMatched]/D");

	std::cout << "Processing" << std::endl;
	const int nDiv = TMath::Max(NEvents1/40, 1);

	for(int entry = 0; entry < NEvents1; entry++){
		if(entry%nDiv == 0) std::cout << " Entry " << entry << std::endl;

		t1->GetEntry(entry);
		t3->GetEntry(entry);
		t4->GetEntry(entry);
		unsigned long long key = keyFromRunLumiEvent(Event1_->run, Event1_->lumi, Event1_->event);
		int entry2 = runLumiEvtMap[key];
		t2->GetEntry(entry2);


		event = int(Event1_->event);
		lumi = int(Event1_->lumi);
		run = int(Event1_->run);
		//    cout << "hiHF = " << hiHF << endl;            

		if(doConversion==1) Cent = getHiBinFromHiHF(hiHF)/2.0;

		ADCSizeMatched = ADCSize;

		for(int k = 0; k < ADCSize; k++){
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

		for(int l = 0; l < nTowers; l++){
			hiHFEnergyMatched[l] = hiHFEnergy[l];
			HFMatchedieta[l] = HFieta[l];
			HFMatchediphi[l] = HFiphi[l];
		}

		ADCnTowerMatched = int(ADCnTower);
       //	cout << "ADCnTowerMatched = " <<  ADCnTowerMatched << endl;
		/*
		for(int n = 0; n < ADCnTowerMatched; n++){

		ietMatched[n] = int(towers_->iet);
		cout << "iet = " <<  	towers_->iet  << endl;
		cout << "ietMatched = " <<  	ietMatched[n]  << endl;
		
		}
		*/
		int n = 0;
		for (std::vector<short>::const_iterator i = towers_->iet.begin(); i != towers_->iet.end(); i++)
		{
			ietMatched[n] = int(*i);
			// cout << "iet = " << *i << endl;
			 n = n + 1;
		}

		EVTMatchTree->Fill();
	}

	EVTMatchTree->Write();
	fout->Close();

	timer.stop();
	std::cout << "Total: " << timer.total() << std::endl;

	return;
}
