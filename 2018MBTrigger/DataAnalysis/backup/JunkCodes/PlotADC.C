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


void PlotADC(){

	int ADCBoth1[100000];
	int ADCSize1;
	int ADCBoth2[100000];
	int ADCSize2;
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);

	TFile * fin = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1Ntuple.root");
	TFile * fin2 = new TFile("/afs/cern.ch/work/s/szhaozho/DataReEmulation/CMSSW_10_1_5/src/L1PbPb2018/MBSamples/L1NtupleMTBX.root");
	

	TTree * t = (TTree*) fin->Get("ADC");
	TTree * t2 = (TTree*) fin2->Get("ADC");

	t->SetBranchAddress("ADCSize",&ADCSize1);
	t->SetBranchAddress("ADCBoth",ADCBoth1);
	t2->SetBranchAddress("ADCSize",&ADCSize2);
	t2->SetBranchAddress("ADCBoth",ADCBoth2);
	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	TH1D * h = new TH1D("h","h",40,0,40);
	TH1D * h2 = new TH1D("h2","h2",40,0,40);

	TH1D *ADXMax1 = new TH1D("ADXMax1","ADXMax1",40,0,40);
	ADXMax1->SetTitle("Max ADC Distribution for Low PU PP");
	ADXMax1->GetXaxis()->SetTitle("ADCMax");
	ADXMax1->GetYaxis()->SetTitle("Counts");


	TH1D *ADXMax2 = new TH1D("ADXMax2","ADXMax2",40,0,40);
	ADXMax2->SetTitle("Max ADC Distribution for Empty Bunches");
	ADXMax2->GetXaxis()->SetTitle("ADCMax");
	ADXMax2->GetYaxis()->SetTitle("Counts");

	h->SetTitle("ADC Distribution");
	h->GetXaxis()->SetTitle("ADC");
	h->GetYaxis()->SetTitle("Counts");
	

	t->Project("h","ADCBoth");
	t2->Project("h2","ADCBoth");
	

	h->SetMarkerColor(kRed);
	h2->SetMarkerColor(kBlue);
	TLegend * l = new TLegend(0.55,0.52,0.75,0.73);
		l->AddEntry(h,"Low PU PP","lp");
		l->AddEntry(h2,"Empty Bunches","lp");

	h->Draw();
	h2->Draw("SAME");
	l->Draw("SAME");
	c->SetLogy();
	c->SaveAs("ADCplots/ADCBothDis.png");
	int maxADC1;
	int maxADC2;
	int N1= t->GetEntries();
	int N2= t2->GetEntries();

	for(int j = 0; j < N1; j++)
	{
		maxADC1 = 0;
		t->GetEntry(j);
		//cout << "ADCSize = " <<  ADCSize1 << endl;

		for(int i =0; i < ADCSize1; i++)
		{
			//cout << "ADCBoth = " << ADCBoth1[i] << endl;
			if(ADCBoth1[i] > maxADC1) maxADC1 = ADCBoth1[i];
		}
		ADXMax1->Fill(maxADC1);

	}
	
		for(int j = 0; j < N2; j++)
	{
		maxADC2 = 0;
		t2->GetEntry(j);
		for(int i =0; i < ADCSize2; i++)
		{
			if(ADCBoth2[i] > maxADC2) maxADC2 = ADCBoth2[i];
		}
		ADXMax2->Fill(maxADC2);

	}

		ADXMax1->Draw();
		c->SaveAs("ADCplots/ADCMaxLPUPP.png");
		ADXMax2->Draw();
		c->SaveAs("ADCplots/ADCMaxEMBX.png");




}






