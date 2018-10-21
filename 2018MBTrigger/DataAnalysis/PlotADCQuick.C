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
#include "include/parameters.h"
#include "TBranchElement.h"
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


	TFile * fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeZB/L1Ntuple_XeXe_2All.root");
	TFile * fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBX/Files/L1Ntuple_XeXe_All_EMBX.root");
//	TFile * fin = new TFile("L1Ntuple_XeXe_2All.root");
//	TFile * fin2 = new TFile("L1Ntuple_XeXe_All_EMBX.root");

	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";


	int ADCBoth1[10000];
	int ieta[10000];
	int iphi[10000];
	int ADCSize1;

	int ADCBoth2[10000];
	int ieta2[10000];
	int iphi2[10000];
	int ADCSize2;



	int HFietaCut = 28;


	const int NFG = 9;
	int Threshold[NFG+1] = {0,10,12,15,17,20,24,30,34,40};
	const double xValue[NFG+1] = {0,10.0,12.0,15.0,17.0,20.0,24.0,30.0,34.0,40.0};

	double ORPass[NFG+1];
	double ANDPass[NFG+1];
	double OReff[NFG+1]; 
	double ANDeff[NFG+1]; 

	for(int i = 0; i < NFG+1; i++){
		ORPass[i] = 0;
		ANDPass[i] = 0;
	}




	TH1D * hOR = new TH1D("hOR","hOR",NFG,xValue);
	hOR->SetTitle("MB OR Trigger Efficiency vs ADC Thrshold");
	hOR->GetXaxis()->SetTitle("ADC Threshold");
	hOR->GetYaxis()->SetTitle("Efficiency");


	TH1D * hAND = new TH1D("hAND","hAND",NFG,xValue);
	hAND->SetTitle("MB AND Trigger Efficiency vs ADC Thrshold");
	hAND->GetXaxis()->SetTitle("ADC Threshold");
	hAND->GetYaxis()->SetTitle("Efficiency");
 
	L1Analysis::L1AnalysisEventDataFormat *Event1_ = new L1Analysis::L1AnalysisEventDataFormat();
	
	TTree * t = (TTree*) fin->Get("ADC");
    t->SetBranchStatus("*", 1);
	t->SetBranchAddress("ADCSize",&ADCSize1);
	t->SetBranchAddress("ADCBoth",ADCBoth1);
	t->SetBranchAddress("ieta",ieta);
	t->SetBranchAddress("iphi",iphi);

	TTree * t2 = (TTree*) fin2->Get("ADC");
    t2->SetBranchStatus("*", 1);
	t2->SetBranchAddress("ADCSize",&ADCSize2);
	t2->SetBranchAddress("ADCBoth",ADCBoth2);
	t2->SetBranchAddress("ieta",ieta2);
	t2->SetBranchAddress("iphi",iphi2);

	double N1= t->GetEntries();
	double N2= t2->GetEntries();

	//N1 = 60000;
	//N2 = 60000;

	TH1D * h = new TH1D("h","h",40,0,40);
	h->GetXaxis()->SetTitle("ADC");
	h->GetXaxis()->SetTitle("Counts");
	h->SetTitle(Form("All ADC Distribution for %s ZB Data",colsyst.Data()));

	TH1D * h2 = new TH1D("h2","h2",40,0,40);
	h2->GetXaxis()->SetTitle("ADC");
	h2->GetXaxis()->SetTitle("Counts");
	h2->SetTitle(Form("All ADC Distribution for %s Empty Bunches Data",colsyst.Data()));


	TH1D *ADXMax1 = new TH1D("ADXMax1","ADXMax1",50,0,50);
	ADXMax1->SetTitle(Form("Max ADC Distribution for %s ZB Data",colsyst.Data()));
	ADXMax1->GetXaxis()->SetTitle("ADCMax");
	ADXMax1->GetYaxis()->SetTitle("Counts");


	TH1D *ADXMax2 = new TH1D("ADXMax2","ADXMax2",50,0,50);
	ADXMax2->SetTitle(Form("Max ADC Distribution for %s Empty Bunches Data",colsyst.Data()));
	ADXMax2->GetXaxis()->SetTitle("ADCMax");
	ADXMax2->GetYaxis()->SetTitle("Counts");


	h->SetTitle("ADC Distribution");
	h->GetXaxis()->SetTitle("ADC");
	h->GetYaxis()->SetTitle("Counts");


	TCanvas *b = new TCanvas("b","b",600,600);
	b->cd();


	t->Project("h","ADCBoth");
	h->SetMarkerColor(kRed);
	TLegend * l = new TLegend(0.35,0.52,0.85,0.73);
	l->AddEntry(h,Form("All ADC Distribution %s ZB Data",colsyst.Data()));
	l->SetTextFont(42);
	h->Draw();
	l->Draw("SAME");
	b->SaveAs(Form("plots%s/Day1Task/AllZBADCDisLinear.png",colsyst.Data()));
	b->SetLogy();
	b->SaveAs(Form("plots%s/Day1Task/AllZBADCDisLog.png",colsyst.Data()));


	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	t2->Project("h2","ADCBoth");

	h2->SetMarkerColor(kBlue);
	TLegend * l2 = new TLegend(0.42,0.58,0.92,0.79);
	l2->AddEntry(h2,Form("All ADC Distribution %s Empty Bunches Data",colsyst.Data()));
	l2->SetTextFont(42);

	h2->Draw();
	//	h2->Draw("SAME");
	l2->Draw("SAME");
	c->SaveAs(Form("plots%s/Day1Task/AllEMBXADCDisLinear.png",colsyst.Data()));
	c->SetLogy();
	c->SaveAs(Form("plots%s/Day1Task/AllEMBXADCDisLog.png",colsyst.Data()));

	TCanvas *d = new TCanvas("d","d",600,600);
	h2->SetFillColor(kBlue);
	h->SetFillColor(kRed);

	h2->Draw();
	h->Draw("SAME");
	TLegend * l7 = new TLegend(0.42,0.58,0.92,0.79);
	l7->AddEntry(h2,Form("All ADC Distribution %s Empty Bunches Data",colsyst.Data()));
	l7->AddEntry(h,Form("All ADC Distribution %s Zero Bias Data",colsyst.Data()));
	l7->SetTextFont(42);
	l7->Draw("SAME");
	d->cd();
	d->SaveAs(Form("plots%s/Day1Task/AllBothADCDisLinear.png",colsyst.Data()));
	d->SetLogy();
	d->SaveAs(Form("plots%s/Day1Task/AllBothADCDisLog.png",colsyst.Data()));



	int maxADC2;
	int maxADC1;
	int maxADCPlus;
	int maxADCMinus;
	//	int maxADC2;
	for(int j = 0; j < N1; j++)
	{
		if(j%10000==0) cout << "Now Working on ZB Event =  " << j << endl;
		maxADC1 = 0;
		maxADCPlus = 0;
		maxADCMinus = 0;
		t->GetEntry(j);
		//cout << "ADCSize = " <<  ADCSize1 << endl;

		for(int i =0; i < ADCSize1; i++)
		{
		//	cout << "ADCBoth = " << ADCBoth1[i] << endl;
			if(ADCBoth1[i] > maxADC1 && abs(ieta[i]) > HFietaCut) maxADC1 = ADCBoth1[i];
			if(ADCBoth1[i] > maxADCPlus && abs(ieta[i]) > HFietaCut && ieta[i] > 0) maxADCPlus = ADCBoth1[i];
			if(ADCBoth1[i] > maxADCMinus && abs(ieta[i]) > HFietaCut && ieta[i] < 0) maxADCMinus = ADCBoth1[i];

		}
		//cout << "maxADC1 = " <<  maxADC1 << endl;

		for(int i = 0; i < NFG+1; i++){

			if(maxADC1 > Threshold[i] - 1) ORPass[i] = ORPass[i] + 1;
			if(maxADCPlus > Threshold[i] - 1 && maxADCMinus > Threshold[i] - 1) ANDPass[i] = ANDPass[i] + 1;

		}


		ADXMax1->Fill(maxADC1);

	}


	for(int j = 0; j < N2; j++)
	{
		if(j%10000==0) cout << "Now Working on Empty Bunches Event =  " << j << endl;
		maxADC2 = 0;
		t2->GetEntry(j);

			for(int i =0; i < ADCSize2; i++)
		{
			//cout << "ADCBoth = " << ADCBoth1[i] << endl;
			if(ADCBoth2[i] > maxADC2 && abs(ieta2[i]) > HFietaCut) maxADC2 = ADCBoth2[i];

		}

		ADXMax2->Fill(maxADC2);
	}


	TCanvas *c2 = new TCanvas("c2","c2",600,600);
	c2->cd();
	TLegend * l3 = new TLegend(0.40,0.52,0.90,0.73);
	l3->AddEntry(ADXMax1,Form("Max ADC distribution for %s ZB Data",colsyst.Data()),"lp");
	ADXMax1->Draw();
	l3->Draw("SAME");
	c2->SaveAs(Form("plots%s/Day1Task/ADCMaxZBLinear.png",colsyst.Data()));
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/Day1Task/ADCMaxZBLog.png",colsyst.Data()));

	TCanvas *d2 = new TCanvas("d2","d2",600,600);
	d2->cd();
	TLegend * l4 = new TLegend(0.40,0.52,0.90,0.73);
	l4->AddEntry(ADXMax2,Form("Max ADC distribution for %s Empty Bunches Data",colsyst.Data()));
	ADXMax2->Draw();
	l4->Draw("SAME");
	d2->SaveAs(Form("plots%s/Day1Task/ADCMaxEMBXLinear.png",colsyst.Data()));
	d2->SetLogy();
	d2->SaveAs(Form("plots%s/Day1Task/ADCMaxEMBXLog.png",colsyst.Data()));

	TCanvas *b2 = new TCanvas("b2","b2",600,600);
	b2->cd();
	TLegend * l5 = new TLegend(0.35,0.52,0.85,0.73);
	ADXMax1->SetFillColor(kRed);
	ADXMax2->SetFillColor(kBlue);
	l5->AddEntry(ADXMax1,Form("Max ADC distribution for %s ZB Data",colsyst.Data()),"lp");
	l5->AddEntry(ADXMax2,Form("Max ADC distribution for %s Empty Bunches Data",colsyst.Data()));
	ADXMax2->Draw();
	ADXMax1->Draw("SAME");
	l5->Draw("SAME");
	b2->SaveAs(Form("plots%s/Day1Task/ADCMaxBothLinear.png",colsyst.Data()));
	b2->SetLogy();
	b2->SaveAs(Form("plots%s/Day1Task/ADCMaxBothLog.png",colsyst.Data()));


	for(int i = 0; i < NFG+1; i ++){
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

	TLegend * lOR = new TLegend(0.35,0.52,0.85,0.73);
	lOR->AddEntry(hOR,Form("MB OR Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	hOR->SetMarkerStyle(21);
	hOR->SetMarkerSize(1.5);
	hOR->SetMarkerColor(kBlue);
	hOR->Draw("p");
	lOR->Draw("SAME");
	c3->SaveAs(Form("plots%s/Day1Task/MBOREfficiency.png",colsyst.Data()));

	TLegend * lAND = new TLegend(0.35,0.52,0.85,0.73);
	lAND->AddEntry(hAND,Form("MB AND Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
	hAND->SetMarkerStyle(22);
	hAND->SetMarkerColor(kRed);
	hAND->SetMarkerSize(1.5);

	hAND->Draw("p");
	lAND->Draw("SAME");
	c3->SaveAs(Form("plots%s/Day1Task/MBANDEfficiency.png",colsyst.Data()));


	TLegend * lBoth = new TLegend(0.35,0.52,0.85,0.73);

	hOR->Draw("p");


	hAND->Draw("pSAME");

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






