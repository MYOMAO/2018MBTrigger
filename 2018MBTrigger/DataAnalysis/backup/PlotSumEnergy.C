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
#include "include/L1AnalysisL1CaloTowerDataFormat.h"
#include "include/parameters.h"
#include <iostream>
#include <fstream>
//#include "parameters.h"
//#include "his.h"
using namespace std;
using std::cout;
using std::endl;
#endif

void PlotSumEnergy(){

	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);

	int doEMBX = 1;
	int donew = 1;
	int doZB = 0;
	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";

	int nTowersMatched;
	int ADCSizeMatched;
	int ADCnTowerMatched;

	int ADCBothMatched[10000];
	int ADCBackMatched[10000];
	int ADCFrontMatched[10000];
	int ietMatched[10000];

	int ADCMatchedieta[10000];
	int HFMatchedieta[10000];
	int ADCMatchediphi[10000];
	int HFMatchediphi[10000];
	int ADCMatcheddepth[10000];
	float hiHFEnergyMatched[10000];
	double Cent;

	TString infile;
	if(doPbPb == 2) 
	{
		if(doEMBX == 1) 
		{
			if(donew == 0) infile ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBX/EventMatchedXeXe.root";
			if(donew == 1) infile ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/EventMatchedXeXeEMBXNew.root";

		}
		if(doEMBX == 0) infile ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/NewVersion/EventMatchedXeXe.root";
	}
	if(doPbPb == 1) infile ="EventMatchedPbPb.root";
	if(doPbPb == 0) infile ="EventMatchedpp.root";
	TFile * fin = new TFile(infile.Data());

	TTree * t = (TTree*) fin->Get("EVTMatchTree");
	t->SetBranchStatus("*", 1);
	t->SetBranchAddress("ADCSizeMatched",&ADCSizeMatched);

	t->SetBranchAddress("nTowersMatched",&nTowersMatched);
	t->SetBranchAddress("ADCBothMatched",ADCBothMatched);
	t->SetBranchAddress("hiHFEnergyMatched",hiHFEnergyMatched);
	t->SetBranchAddress("ADCMatcheddepth",ADCMatcheddepth);
	t->SetBranchAddress("ADCBackMatched",ADCBackMatched);
	t->SetBranchAddress("ADCFrontMatched",ADCFrontMatched);
	t->SetBranchAddress("ADCMatchedieta",ADCMatchedieta);
	t->SetBranchAddress("ADCMatchediphi",ADCMatchediphi);
	t->SetBranchAddress("HFMatchedieta",HFMatchedieta);
	t->SetBranchAddress("HFMatchediphi",HFMatchediphi);
	t->SetBranchAddress("ADCnTowerMatched",&ADCnTowerMatched);
	t->SetBranchAddress("ietMatched",ietMatched);
	int NEvents = t->GetEntries();

	NEvents = 100;


	const int NCut = 2;
	int ietaCut[NCut] = {25, 28};

	int iETSum[NCut];
	float EnergySum[NCut];

	TH1D * iETSumHis[NCut]; 
	TH1D * EnergySumHis[NCut]; 

	for(int i = 0; i < NCut; i++){
		iETSumHis[i] = new TH1D(Form("iETSumHis%d",i),Form("iETSumHis%d",i),50,0,50);
		iETSumHis[i]->GetXaxis()->SetTitle("iet Sum");
		iETSumHis[i]->GetYaxis()->SetTitle("Counts");
		iETSumHis[i]->SetTitle(Form("iet Sum for |ieta| < %d",ietaCut[i]));

		EnergySumHis[i] = new TH1D(Form("EnergySumHis%d",i),Form("EnergySumHis%d",i),200,20,25);
		EnergySumHis[i]->GetXaxis()->SetTitle("Calo Tower Energy Sum (GeV)");
		EnergySumHis[i]->GetYaxis()->SetTitle("Counts");
		EnergySumHis[i]->SetTitle(Form("Calo Tower Energy Sum for |ieta| < %d",ietaCut[i]));
	}

	//NEvents = 100;

	for(int j = 0; j< NEvents; j++){
		if(j%10000==0) cout << "Now Working on Event =  " << j << endl;
		t->GetEntry(j);

		for(int k = 0; k < NCut; k++){
			iETSum[k] = 0;
			EnergySum[k] = 0.0;
		}
		for(int i =0; i < ADCnTowerMatched; i++)
		{	
			for(int k = 0; k< NCut;k++){
				if(abs(ADCMatchedieta[i]) < ietaCut[k])  iETSum[k] = iETSum[k] + ietMatched[i];
			}

		}
		cout << "nTowersMatched = " <<  nTowersMatched << endl;

		for(int i = 0; i < nTowersMatched; i++)
		{	
		 		//	cout << "HFMatchedieta[i] = " << HFMatchedieta[i] << endl;
			for(int k = 0; k< NCut;k++){
				if(abs(HFMatchedieta[i]) < ietaCut[k]){
					EnergySum[k] = EnergySum[k] + hiHFEnergyMatched[i];
			//	cout << "Entry = " << j <<  "   i = " << i  << "  hiHFEnergyMatched[i] = " <<  hiHFEnergyMatched[i] << endl;


				}

			}
				//cout << "iTower =  "<<  i << "  EnergySum[0] = " << EnergySum[0] << endl;

		}


		for(int k = 0; k< NCut;k++){
	//		cout << "Entry = " << j  << "  EnergySum = " << EnergySum[k] << endl;
	//	    cout << "iETSum = " << iETSum[k] << endl;
			iETSumHis[k]->Fill(iETSum[k]);
			EnergySumHis[k]->Fill(EnergySum[k]);
		}

	}

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	for(int k = 0; k< NCut;k++){
		iETSumHis[k]->Draw();
		c->SaveAs(Form("plots%s/EnergySum/iETSumHisLT%d.png",colsyst.Data(),ietaCut[k]));
		EnergySumHis[k]->Draw();
		c->SaveAs(Form("plots%s/EnergySum/EnergySumHis%d.png",colsyst.Data(),ietaCut[k]));
	}


	TFile * fout = new TFile("EnergySum.root","RECREATE");
	fout->cd();

	for(int k = 0; k< NCut;k++){
		iETSumHis[k]->Write();
		EnergySumHis[k]->Write();
	}

	fout->Close();


}
