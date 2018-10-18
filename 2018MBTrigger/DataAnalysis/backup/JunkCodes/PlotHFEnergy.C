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
#include "uti.h"
#include "TBranchElement.h"
#include "L1AnalysisL1CaloTowerDataFormat.h"
//#include "parameters.h"
//#include "his.h"
using namespace std;
using std::cout;
using std::endl;
#endif


void PlotHFEnergy()
{
	//	gROOT->ProcessLine("#include <vector>");
	//	Short_t nTower;
	//	vector<short> iet;
	int ADCBoth[100000];
	int ADCSize;
	int Pass;
	TFile * fin = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/L1Ntuple.root");

	TTree * t = (TTree*) fin->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
	TTree *t2 =  (TTree*) fin->Get("ADC");


	//	t->SetBranchAddress("nTower",&nTower);

	//TBranchElement *nTower = (TBranchElement*)t->GetBranch("nTower");
	//TBranchElement *iet = (TBranchElement*)t->GetBranch("iet");

	L1Analysis::L1AnalysisL1CaloTowerDataFormat *towers_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();

	t->SetBranchStatus("*", 0);
	t->SetBranchStatus("L1CaloTower", 1);
	t->SetBranchStatus("iet", 1);
	t->SetBranchStatus("ieta", 1);
	t->SetBranchStatus("et", 1);

	t->SetBranchAddress("L1CaloTower", &towers_);
	//int nTowerValue;
	//t->SetBranchAddress("iet",&iet);
	t2->SetBranchAddress("ADCSize",&ADCSize);
	t2->SetBranchAddress("ADCBoth",ADCBoth);


	t->AddFriend(t2);
	//return;
	int NEvents = t->GetEntries();

	const int NFG = 5;
	TFile *fout = new TFile("output.root","RECREATE");
	fout->cd();
	double FGThres[NFG] = {0,10.0,12.0,15.0,17.0};
	int ADCLabel[NFG] = {0,10,12,15,17};
	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	cout <<"Total Events = "<< NEvents << endl;
	for(int i = 0; i < NFG; i++)
	{

		TH1D * hetmax = new TH1D("hetamax","hetamax",20,0,20);
		hetmax->GetXaxis()->SetTitle("Max HF Energy (GeV)");
		hetmax->GetYaxis()->SetTitle("Number of Events");
		hetmax->SetTitle(Form("Max HF Energy (GeV) Distribution with ADC > %d", ADCLabel[i]));

		for(int j = 0; j< NEvents; j++){
			float etmax = 0;
			int ietmax = 0;
			int maxADC = 0;
				t->GetEntry(j);
			t2->GetEntry(j);
			//cout << "ADCSize = " << ADCSize << endl;
			//	cout << "Event = " << j << endl;
			//	cout << "Total Towers = " << towers_->et.size() << endl;
			Pass = 0;
			for(int l = 0; l < ADCSize; l++)
			{

				//	cout << " l = " << l << "ADCBoth = " << ADCBoth[l] << endl;
				//if(ADCBoth[l] > FGThres[i] || ADCBoth[l] ==  FGThres[i]){
				//		Pass = 1;
				//	}
				if (ADCBoth[l]  > maxADC) maxADC = ADCBoth[l];
			}
			
			if(maxADC > FGThres[i] - 1)
			{
				Pass = 1;
			}
					cout << "Now Doing Event " << j << "  Pass = " << Pass << endl;
			
			if(Pass == 1){
				for(unsigned int k = 0; k< towers_->et.size(); k++){
					//	if ( towers_->et[k] > etmax && abs(towers_->ieta[k]) > 29)	etmax = towers_->iet[k];
					if ( ((int) towers_->iet[k]) > ietmax && abs(towers_->ieta[k]) > 29)	ietmax = towers_->iet[k];

					//cout <<"k = " << k <<  "   current et value = " << towers_->iet[k] << endl;

					//	cout << "ietmax = " << ietmax << endl;
				}
			
			cout << "ieta max after = " << ietmax << endl;
				hetmax->Fill(ietmax);

			}
			/*
		TH1D * ADCmax = new TH1D("ADCmax","ADCmax",200,0,100);
		hetmax->GetXaxis()->SetTitle("Max ADC");
		hetmax->GetYaxis()->SetTitle("Number of Events");
		hetmax->SetTitle("Max ADC Distribution with ADC");


						ADCmax->Fill(maxADC);
	*/
						}
		//if(ietmax > 0)	hetmax->Fill(ietmax);


		hetmax->Draw();
		TString SaveName = Form("plots/MHFaxEtADC%d.png",ADCLabel[i]);
		c->SaveAs(SaveName.Data());
	//	ADCmax->Draw();
	//	c->SaveAs("ADCmax.png");
		hetmax->Scale(1.0/hetmax->Integral());
		SaveName = Form("plots/MHFaxEtADCNormalized%d.png",ADCLabel[i]);
		c->SaveAs(SaveName.Data());


		hetmax->Write();
	}


	fout->Close();

}

