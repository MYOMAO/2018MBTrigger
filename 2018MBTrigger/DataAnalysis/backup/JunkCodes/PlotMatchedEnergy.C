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
#include "parameters.h"
//#include "his.h"
using namespace std;
using std::cout;
using std::endl;
#endif


void PlotMatchedEnergy()
{
	//	gROOT->ProcessLine("#include <vector>");
	//	Short_t nTower;
	//	vector<short> iet;
	//
	int doNormalized =0;
	int doEMBX=0;
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
	int ADCBothMatched[10000];
	int ADCSizeMatched;
	float hiHFEnergyMatched[10000];
	int nTowersMatched;
	int ADCMatchedieta[10000];
	int ADCMatchediphi[10000];
	int HFMatchediphi[10000];
	int ADCMatcheddepth[10000];
	int HFMatchedieta[10000];

	int Pass;
	int doOR = 1;
	int doAND = 0;

	TString infile;
	TString colsyst;

	//if(doPbPb == 0) infile = "/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/2018MBTrigger/EventMatchedpp.root";
	//	if(doPbPb == 1) infile = "/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/2018MBTrigger/EventMatchedPbPb.root";
	if(doPbPb == 2) 
	{
		if(doEMBX == 1) infile ="EventMatchedXeXe.root";
		if(doEMBX == 0) infile ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/NewVersion/EventMatchedXeXe.root";
	}
	if(doPbPb == 1) infile ="EventMatchedPbPb.root";
	if(doPbPb == 0) infile ="EventMatchedpp.root";

	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";

	TString label;

	if(doOR == 1) label = "OR";
	if(doAND == 1) label = "AND";


	TFile * fin = new TFile(infile.Data());

	TTree * t = (TTree*) fin->Get("EVTMatchTree");


	//	t->SetBranchAddress("nTower",&nTower);

	//TBranchElement *nTower = (TBranchElement*)t->GetBranch("nTower");
	//TBranchElement *iet = (TBranchElement*)t->GetBranch("iet");


	t->SetBranchStatus("*", 1);
	t->SetBranchAddress("ADCSizeMatched",&ADCSizeMatched);
	t->SetBranchAddress("nTowersMatched",&nTowersMatched);
	t->SetBranchAddress("ADCBothMatched",ADCBothMatched);
	t->SetBranchAddress("hiHFEnergyMatched",hiHFEnergyMatched);
	t->SetBranchAddress("ADCMatchedieta",ADCMatchedieta);
	t->SetBranchAddress("ADCMatchediphi",ADCMatchediphi);
	t->SetBranchAddress("HFMatchedieta",HFMatchedieta);
	t->SetBranchAddress("HFMatchediphi",HFMatchediphi);
	t->SetBranchAddress("ADCMatcheddepth",ADCMatcheddepth);

	int NEvents = t->GetEntries();

	const int NFG = 5;
	TFile *fout = new TFile("Results.root","RECREATE");
	fout->cd();
	double FGThres[NFG] = {0,10.0,12.0,15.0,17.0};
	int ADCLabel[NFG] = {0,10,12,15,17};
	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	cout <<"Total Events = "<< NEvents << endl;
	TH1D * hetmax[NFG]; 
	TH1D * MaxADC = new TH1D("MaxADC","MaxADC",30,0,30);
	MaxADC->GetXaxis()->SetTitle("Max ADC");
	MaxADC->GetYaxis()->SetTitle("Counts");
	MaxADC->SetTitle("Max ADC Distribution");


	int ADCSumLong;
	int ADCSumShort;
	int HFietaCut = 28;
	for(int i = 0; i < NFG; i++)
	{

		hetmax[i] = new TH1D("hetamax","hetamax",50,0,50);
		hetmax[i]->GetXaxis()->SetTitle("Max HF Energy (GeV)");
		hetmax[i]->GetYaxis()->SetTitle("Number of Events");
		hetmax[i]->SetTitle(Form("Max HF Energy (GeV) Distribution with ADC > %d", ADCLabel[i]));

		for(int j = 0; j< NEvents; j++){
			float etmax = 0;
			int maxADC = 0;
			int maxADCPlus = 0;
			int maxADCMinus = 0;

			t->GetEntry(j);
			//cout << "ADCSize = " << ADCSize << endl;
			//	cout << "Event = " << j << endl;
			//	cout << "Total Towers = " << towers_->et.size() << endl;
			Pass = 0;
			for(int l = 0; l < ADCSizeMatched; l++)
			{

				//	cout << " l = " << l << "ADCBoth = " << ADCBoth[l] << endl;
				//if(ADCBoth[l] > FGThres[i] || ADCBoth[l] ==  FGThres[i]){
				//		Pass = 1;
				//	}
				if(doOR == 1){
					if (abs(ADCMatchedieta[l]) > HFietaCut && ADCBothMatched[l]  > maxADC) maxADC = ADCBothMatched[l];
				}

				if(doAND == 1){
					if ( abs(ADCMatchedieta[l]) > HFietaCut && ADCMatchedieta[l] > 0  && ADCBothMatched[l]  > maxADCPlus) maxADCPlus = ADCBothMatched[l];
					if ( abs(ADCMatchedieta[l]) > HFietaCut && ADCMatchedieta[l] < 0  && ADCBothMatched[l]  > maxADCMinus) maxADCMinus = ADCBothMatched[l];
				}
				


			}

			if(doOR == 1){
				if(maxADC > FGThres[i] - 1)
				{
					Pass = 1;
				}
			}

			if(doAND == 1){
				if(maxADCPlus > FGThres[i] - 1 && maxADCMinus > FGThres[i] - 1)
				{
					Pass = 1;
				}
			}


			if(j%10000==0) cout << "Now Doing Event " << j << "  Pass = " << Pass << endl;

			if(Pass == 1){
				for(int k = 0; k < nTowersMatched; k++){
					//	if ( towers_->et[k] > etmax && abs(towers_->ieta[k]) > 29)	etmax = towers_->iet[k];
					if (abs(HFMatchedieta[k]) > HFietaCut  && hiHFEnergyMatched[k] > etmax)	etmax = hiHFEnergyMatched[k];

					//cout <<"k = " << k <<  "   current et value = " << towers_->iet[k] << endl;

					//	cout << "ietmax = " << ietmax << endl;
				}

				//cout << "ieta max after = " << etmax << endl;
				hetmax[i]->Fill(etmax);

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


		hetmax[i]->Draw();
		TString SaveName = Form("plots%s/MHFaxEtADC%s%d.png",colsyst.Data(),label.Data(),ADCLabel[i]);
		c->SaveAs(SaveName.Data());
		//	ADCmax->Draw();
		//	c->SaveAs("ADCmax.png");

		if(doNormalized == 1){
			hetmax[i]->Scale(1.0/hetmax[i]->Integral());
			SaveName = Form("plots%s/MHFaxEtADCNormalized%d.png",colsyst.Data(),ADCLabel[i]);
			c->SaveAs(SaveName.Data());
		}


		hetmax[i]->Write();
	}
	TCanvas *d = new TCanvas("d","d",600,600);
	d->cd();
	TLegend * l = new TLegend(0.55,0.67,0.75,0.88);

	int Color[NFG] = {1,2,3,4,6};
	for(int i = 0; i <NFG; i++)
	{
		hetmax[i]->SetMarkerSize(2);
		hetmax[i]->SetMarkerColor(Color[i]);
		hetmax[i]->SetLineColor(Color[i]);

		l->AddEntry(hetmax[i],Form("ADC > %d",ADCLabel[i]),"lp");
		hetmax[i]->SetTitle("Max HF Energy (GeV) Distribution with Various ADCs Thresholds");
		if(i == 0) hetmax[i]->Draw();
		if(i> 0)	hetmax[i]->Draw("SAME");

	}
	l->Draw("SAME");
	d->SetLogy();
	d->SaveAs(Form("plots%s/AllHFvsADC%s.png",colsyst.Data(),label.Data()));

	fout->Close();

}

