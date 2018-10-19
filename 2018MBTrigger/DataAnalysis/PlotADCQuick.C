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


void PlotADCQuick(){

	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);


	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";


	int ADCBoth1[10000];
	int ieta[10000];
	int iphi[10000];
	int ADCSize1;
	int HFietaCut = 28;

	const int NFG = 9;
	const int Threshold[NFG+1] = {0,10,12,15,17,20,24,30,34,40};
	const double xValue[NFG+1] = {0,10.0,12.0,15.0,17.0,20.0,24.0,30.0,34.0,40.0};

	int ORPass[NFG];
	int ANDPass[NFG];

	for(int i = 0; i < NFG+1; i ++){
		ORPass[i] = 0;
		ANDPass[i] = 0;
	}


	double OReff[NFG]; 
	double ANDeff[NFG]; 


	TH1D * hOR = new TH1D("hOR","hOR",NFG,xValue);
	hOR->SetTitle("MB OR Trigger Efficiency vs ADC Thrshold");
	hOR->GetXaxis()->SetTitle("ADC Threshold");
	hOR->GetYaxis()->SetTitle("Efficiency");


	TH1D * hAND = new TH1D("hAND","hAND",NFG,xValue);
	hAND->SetTitle("MB AND Trigger Efficiency vs ADC Thrshold");
	hAND->GetXaxis()->SetTitle("ADC Threshold");
	hAND->GetYaxis()->SetTitle("Efficiency");


	TFile * fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/PbPbMC/L1NtuplePbPbMCNew2.root");



	TTree * t = (TTree*) fin->Get("ADC");

	t->SetBranchAddress("ADCSize",&ADCSize1);
	t->SetBranchAddress("ADCBoth",ADCBoth1);
	t->SetBranchAddress("ieta",ieta);
	t->SetBranchAddress("iphi",iphi);

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	TH1D * h = new TH1D("h","h",40,0,40);

	TH1D *ADXMax1 = new TH1D("ADXMax1","ADXMax1",50,0,50);
	ADXMax1->SetTitle("Max ADC Distribution for Low PU PP");
	ADXMax1->GetXaxis()->SetTitle("ADCMax");
	ADXMax1->GetYaxis()->SetTitle("Counts");


	h->SetTitle("ADC Distribution");
	h->GetXaxis()->SetTitle("ADC");
	h->GetYaxis()->SetTitle("Counts");


	t->Project("h","ADCBoth");


	h->SetMarkerColor(kRed);
	TLegend * l = new TLegend(0.55,0.52,0.75,0.73);
	l->AddEntry(h,Form("All ADC Distribution %s Data Empty Bunches",colsyst.Data()),"lp");

	h->Draw();
	//	h2->Draw("SAME");
	l->Draw("SAME");
	c->SaveAs(Form("plots%s/Day1Task/AllEMBXADCDisLinear.png",colsyst.Data()));
	c->SetLogy();
	c->SaveAs(Form("plots%s/Day1Task/AllEMBXADCDisLog.png",colsyst.Data()));
	int maxADC1;
	int maxADCPlus;
	int maxADCMinus;
	//	int maxADC2;
	int N1= t->GetEntries();
	//	int N2= t2->GetEntries();

	for(int j = 0; j < N1; j++)
	{
		if(j%10000==0) cout << "Now Working on Event =  " << j << endl;
		maxADC1 = 0;
		maxADCPlus = 0;
		maxADCMinus = 0;
		t->GetEntry(j);
		//cout << "ADCSize = " <<  ADCSize1 << endl;

		for(int i =0; i < ADCSize1; i++)
		{
			//cout << "ADCBoth = " << ADCBoth1[i] << endl;
			if(ADCBoth1[i] > maxADC1 && abs(ieta[i]) > HFietaCut) maxADC1 = ADCBoth1[i];
			if(ADCBoth1[i] > maxADCPlus && abs(ieta[i]) > HFietaCut && ieta[i] > 0) maxADCPlus = ADCBoth1[i];
			if(ADCBoth1[i] > maxADCMinus && abs(ieta[i]) > HFietaCut && ieta[i] < 0) maxADCMinus = ADCBoth1[i];

		}

		for(int i = 0; i < NFG; i ++){

			if(maxADC1 > Threshold[i] - 1) ORPass[i] = ORPass[i] + 1;
			if(maxADCPlus > Threshold[i] - 1 && maxADCMinus > Threshold[i] - 1) ANDPass[i] = ANDPass[i] + 1;

		}


		ADXMax1->Fill(maxADC1);

	}

	TCanvas *c2 = new TCanvas("c2","c2",600,600);
	c2->cd();
	TLegend * l2 = new TLegend(0.55,0.52,0.75,0.73);
	l2->AddEntry(ADXMax1,Form("Max ADC distribution for %s Data Empty Bunches",colsyst.Data()),"lp");
	ADXMax1->Draw();
	l2->Draw("SAME");
	c2->SaveAs(Form("plots%s/Day1Task/ADCMaxEMBXLinear.png",colsyst.Data()));
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/Day1Task/ADCMaxEMBXLog.png",colsyst.Data()));

	for(int i = 0; i < NFG; i ++){
		OReff[i] =  ORPass[i]/N1;
		ANDeff[i] =  ANDPass[i]/N1;
		ofstream out1(Form("datFiles%s/Info%dOR.dat",colsyst.Data(),i));
		cout << "ADC OR Thrshold  =  " << Threshold[i] << endl;
		cout << "Total ZB Events =  " << N1 << endl;
		cout << "NEvents Passing MB OR Trigger = " << ORPass[i] << endl;
		cout << "MB OR Efficiency = " << OReff[i] << endl;

		ofstream out2(Form("datFiles%s/Info%dAND.dat",colsyst.Data(),i));
		cout << "ADC AND Thrshold  =  " << Threshold[i] << endl;
		cout << "Total ZB Events =  " << N1 << endl;
		cout << "NEvents Passing MB AND Trigger = " << ANDPass[i] << endl;
		cout << "MB AND Efficiency = " << ANDeff[i] << endl;	

		hOR->SetBinContent(i+1,OReff[i]);
		hAND->SetBinContent(i+1,ANDeff[i]);
	}

	TCanvas *c3 = new TCanvas("c3","c3",600,600);
	c3->cd();

	TLegend * lOR = new TLegend(0.55,0.52,0.75,0.73);
	lOR->AddEntry(hOR,Form("MB OR Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	hOR->Draw("p");
	lOR->Draw("SAME");
	c3->SaveAs(Form("plots%s/Day1Task/MBOREfficiency.png",colsyst.Data()));

	TLegend * lAND = new TLegend(0.55,0.52,0.75,0.73);
	lAND->AddEntry(hAND,Form("MB AND Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	hAND->Draw("p");
	lAND->Draw("SAME");
	c3->SaveAs(Form("plots%s/Day1Task/MBANDEfficiency.png",colsyst.Data()));


	TLegend * lBoth = new TLegend(0.55,0.52,0.75,0.73);

	hOR->Draw("p");
	hOR->SetMarkerStyle(21);
	hOR->SetMarkerSize(2);
	hOR->SetMarkerColor(kBlue);

	hAND->Draw("pSAME");
	hAND->SetMarkerStyle(22);
	hAND->SetMarkerColor(kRed);

	lBoth->AddEntry(hOR,Form("MB OR Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	lBoth->AddEntry(hAND,Form("MB AND Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	lBoth->Draw("SAME");

	c3->SaveAs(Form("plots%s/Day1Task/MBEfficiencyBoth.png",colsyst.Data()));

	TFile * fout = new TFile(Form("Day1Task%sData.root",colsyst.Data()),"RECREATE");
	fout->cd();
	h->Write();
	ADXMax1->Write();
	hOR->Write();
	hAND->Write();


	fout->Close();

}






