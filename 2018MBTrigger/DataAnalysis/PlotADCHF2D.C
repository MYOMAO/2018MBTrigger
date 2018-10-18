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
#include "include/TBranchElement.h"
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


void PlotADCHF2D(){

	int doEMBX = 0;
	int domax = 1;
	int doall = 0;
	int doped = 1;

	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";

	int nTowersMatched;
	int ADCBothMatched[10000];
	int ADCBackMatched[10000];
	int ADCFrontMatched[10000];

	int ADCSizeMatched;
	int ADCMatchedieta[10000];
	int HFMatchedieta[10000];
	int ADCMatchediphi[10000];
	int HFMatchediphi[10000];
	int ADCMatcheddepth[10000];
	float hiHFEnergyMatched[10000];

	double ADCMatchedcharge[10000];
	double ADCMatchedchargeped[10000];
	double ADCMatchedenergy[10000];
	double ADCMatchedenergyped[10000];

	TString infile;
	if(doPbPb == 2) 
	{
		if(doEMBX == 1) infile ="EventMatchedXeXe.root";
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

	t->SetBranchAddress("ADCMatchedcharge",ADCMatchedcharge);
	t->SetBranchAddress("ADCMatchedchargeped",ADCMatchedchargeped);
	t->SetBranchAddress("ADCMatchedenergy",ADCMatchedenergy);
	t->SetBranchAddress("ADCMatchedenergyped",ADCMatchedenergyped);



	int NEvents = t->GetEntries();

	TFile *fout = new TFile("ADCEnergy2D.root","RECREATE");
	fout->cd();
	//NEvents = 20000;
	TH2D * ADCEnergy = new TH2D("ADCEnergy","ADCEnergy",100,15,115,100,0,60);
	//TH2D * ADCEnergySum = new TH2D("ADCEnergySum","ADCEnergySum",100,140000,17000,100,0,4000);
	//	TH2D * ADCAllEnergy = new TH2D("ADCAllEnergy","ADCAllEnergy",40,0,40,100,0,20);
	TH2D * ADCADCEnergy = new TH2D("ADCADCEnergy","ADCADCEnergy",40,0,40,100,0,20);
	//	TH2D * ADCADCEnergyped = new TH2D("ADCADCEnergyped","ADCADCEnergyped",100,0,60,100,0,60);
	TH2D * ADCAllEnergy = new TH2D("ADCAllEnergy","ADCAllEnergy",40,0,30,100,0,20);

	TH2D * ADCADCEnergyped = new TH2D("ADCADCEnergyped","ADCADCEnergyped",100,0,30,100,0,20);

	ADCEnergy->GetXaxis()->SetTitle("ADC");
	ADCEnergy->GetYaxis()->SetTitle("HFMax Energy (GeV)");
	ADCAllEnergy->GetXaxis()->SetTitle("ADC");
	ADCAllEnergy->GetYaxis()->SetTitle("HF Energy (GeV)");

	//ADCEnergySum->GetXaxis()->SetTitle("ADC Sum");
	//ADCEnergySum->GetYaxis()->SetTitle("HF Energy Sum (GeV)");

	ADCEnergy->SetTitle("HFMax Energy vs ADC for Low PU PP Data");

	ADCADCEnergy->GetXaxis()->SetTitle("ADC");
	ADCADCEnergy->GetYaxis()->SetTitle("HF Energy from Charge Calculation - With Ped Substraction (GeV)");

	ADCADCEnergyped->GetXaxis()->SetTitle("ADC");
	ADCADCEnergyped->GetYaxis()->SetTitle("HF Energy from Charge Calculation - No Ped Substraction (GeV)");

	if(doPbPb == 0){	
		ADCAllEnergy->SetTitle("HF All Energy vs ADC for Low PU PP Data");
		ADCADCEnergyped->SetTitle("HF Energy from Charge Calculation (No Ped Substraction) vs ADC for Low PU PP Data");
		ADCADCEnergy->SetTitle("HF Energy from Charge Calculation (With Ped Substraction) vs ADC for Low PU PP Data");
	}

	if(doPbPb == 1)
	{
		ADCAllEnergy->SetTitle("HF All Energy vs ADC for PbPb MC Data");
		ADCADCEnergyped->SetTitle("HF Energy from Charge Calculation (No Ped Substraction) vs ADC for PbPb MC Data");
		ADCADCEnergy->SetTitle("HF Energy from Charge Calculation (With Ped Substraction) vs ADC for PbPb MC Data");

	}
	if(doPbPb == 2)
	{
		ADCAllEnergy->SetTitle("HF All Energy vs ADC for XeXe ZB Data");
		ADCADCEnergyped->SetTitle("HF Energy from Charge Calculation (No Ped Substraction) vs ADC for PbPb MC Data");
		ADCADCEnergy->SetTitle("HF Energy from Charge Calculation (With Ped Substraction) vs ADC for PbPb MC Data");

	}
	int HFietaCut = 28;


	for(int j = 0; j< NEvents; j++){
		if(j%10000==0) cout << "Now Work on Event = " << j << endl;
		
		float etmax = 0;
		int maxADC = 0;
		int ADCSum =0;
		float etsum=0.0;
		int kmax =0;


		int MaxADCindex = 0;
		int ADCMaxEnergy = 0;
		int MaxADCindexped = 0;
		int ADCMaxEnergyped = 0;

		double maxADCMatchedenergy = -10;
		double maxADCMatchedenergyped = -10;

		int HFMatchedietaMaxE;
		int HFMatchediphiMaxE;

		t->GetEntry(j);
		if(domax == 1){
			for(int k = 0; k < nTowersMatched; k++){
				if(abs(HFMatchedieta[k]) > HFietaCut){
					if (hiHFEnergyMatched[k] > etmax)
					{
						etmax = hiHFEnergyMatched[k];
						kmax = k;
					}
					//	etsum = etsum + hiHFEnergyMatched[k];
				}
			}
			HFMatchedietaMaxE = HFMatchedieta[kmax];
			HFMatchediphiMaxE = HFMatchediphi[kmax];

			for(int l = 0; l < ADCSizeMatched; l++)
			{
				if(abs(ADCMatchedieta[l]) > HFietaCut){
					if(ADCMatchedieta[l] == HFMatchedietaMaxE && ADCMatchediphi[l]  == HFMatchediphiMaxE)
						maxADC = maxADC + ADCBothMatched[l];
					//	ADCSum = ADCSum +  ADCBothMatched[l];
				}
			}

			if(doped == 1){
				for(int l = 0; l < ADCSizeMatched; l++)
				{
					if(abs(ADCMatchedieta[l]) > HFietaCut){

						if(ADCMatchedenergy[l] > maxADCMatchedenergy) 
						{
							maxADCMatchedenergy = ADCMatchedenergy[l];
							MaxADCindex = l;
						}

						ADCMaxEnergy = ADCBothMatched[MaxADCindex];

						if(ADCMatchedenergyped[l] > maxADCMatchedenergyped) 
						{
							maxADCMatchedenergyped = ADCMatchedenergyped[l];
							MaxADCindexped = l;
						}

						ADCMaxEnergyped = ADCBothMatched[MaxADCindexped];

						//	ADCSum = ADCSum +  ADCBothMatched[l];
					}
				}

/*
				cout << "Entry = " << j << endl;
				//	cout << "ADCSUm = " << ADCSum << endl;
				cout << "etmax = " << etmax << endl;
				cout << "maxADC = " << maxADC << endl;
*/
				//cout << "etsum = " << etsum << endl;


				//	cout << "	ADCMaxEnergyped = "	<<	ADCMaxEnergyped << endl;

				ADCADCEnergy->Fill(ADCMaxEnergy,maxADCMatchedenergy);
				ADCADCEnergyped->Fill(ADCMaxEnergyped,maxADCMatchedenergyped);
			}
			ADCEnergy->Fill(maxADC,etmax);
		}
		//		ADCEnergySum->Fill(ADCSum,etsum);

		//Doing the Sum of All Four ADCs//
		if(doall == 1){
			for(int k = 0; k < nTowersMatched; k++){
				int allADC = 0;
				if(abs(HFMatchedieta[k]) > HFietaCut){
					//	cout << "k = " << k << "   hiHFEnergyMatched[k] =" << hiHFEnergyMatched[k] << endl;

					for(int l = 0; l < ADCSizeMatched; l++)
					{
						if(abs(ADCMatchedieta[l]) > HFietaCut){
							if(ADCMatchedieta[l] == HFMatchedieta[k] && ADCMatchediphi[l]  == HFMatchediphi[k])
								allADC = allADC + ADCBothMatched[l];
							//	ADCSum = ADCSum +  ADCBothMatched[l];
						}
						//allADC = allADC/4;
					}
					ADCAllEnergy->Fill(allADC,hiHFEnergyMatched[k]);
				}

			}

		}

	}


TCanvas *d = new TCanvas("d","d",600,600);
d->cd();


ADCEnergy->Draw("Colz");
d->SaveAs(Form("plots%s/HF2D/ADCSumHFMax.png",colsyst.Data()));
//	ADCEnergySum->Draw();
//	d->SaveAs("plots%s/ADCSumHFSum.png");

TF1 *f1 = new TF1("f1","1.0/3.0*x",0,40);
TF1 *f2 = new TF1("f2","1.0/6.0*x",0,40);

f1->SetLineStyle(4);
f2->SetLineStyle(4);

ADCADCEnergy->Draw();
f1->Draw("SAME");
f2->Draw("SAME");
d->SaveAs(Form("plots%s/ped/ADCChargeEnergy.png",colsyst.Data()));

ADCADCEnergyped->Draw();
f1->Draw("SAME");
f2->Draw("SAME");
d->SaveAs(Form("plots%s/ped/ADCChargeEnergyped.png",colsyst.Data()));

//ADCAllEnergy->Draw();
//f1->Draw("SAME");
//f2->Draw("SAME");

//d->SaveAs(Form("plots%s/ADCSumHFAll.png",colsyst.Data()));

ADCEnergy->Write();
ADCADCEnergy->Write();
ADCADCEnergyped->Write();
ADCAllEnergy->Write();

//	ADCEnergySum->Write();

fout->Close();


}
