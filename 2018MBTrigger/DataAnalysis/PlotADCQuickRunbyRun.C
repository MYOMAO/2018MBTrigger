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
#include "include/quickCentralityTable_20180919.h"
#include "include/parameters.h"
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


void PlotADCQuickRunbyRun(){

	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);


	//	TFile * fin = new TFile("L1Ntuple_XeXe_2All.root");
	//	TFile * fin2 = new TFile("L1Ntuple_XeXe_All_EMBX.root");

	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";

	TFile *fin;
	TFile *fin2;
	TFile *fin3;
	if(doNewRun==0){
		if(doPbPb == 2){
			fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeZB/L1Ntuple_XeXe_2All.root");
			fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBX/Files/L1Ntuple_XeXe_All_EMBX.root");
			fin3 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeMB/L1Ntuple_XeXe_MB.root");
		}

		if(doPbPb == 0){
			fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/LowPUPP/L1NtupleNewPed56.root");
			fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/LPUPPEMBX/L1NtupleEMBXLPUppSmaller.root");
		}

		if(doPbPb == 1){
			fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/LowPUPP/L1NtupleNewPed56.root");
			fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/LPUPPEMBX/L1NtupleEMBXLPUppSmaller.root");
		}
	}


	if(doNewRun==1){
		if(doPbPb == 1){
			fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/2018PbPbData/Day1/L1Ntuple2018PbPbAll.root");
			fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/2018PbPbData/Day1/L1Ntuple2018PbPbAll.root");
			fin3 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/2018PbPbData/Day1/L1Ntuple2018PbPbAll.root");

		}
	if(doPbPb == 1 ) colsyst="2018PbPb";

	}




	int ADCBoth1[10000];
	int ieta[10000];
	int iphi[10000];
	int ADCSize1;

	int ADCBoth2[10000];
	int ieta2[10000];
	int iphi2[10000];
	int ADCSize2;

	int ADCBoth3[10000];
	int ieta3[10000];
	int iphi3[10000];
	int ADCSize3;


	int run1;
	int run2;
	int run3;

	std::vector<int> run1id;
	std::vector<int> run2id;
	std::vector<int> run3id;


	//Begin Run ID Collection//


	TTree * EventTree1 = (TTree*) fin->Get("l1EventTree/L1EventTree");
	TTree * EventTree2 = (TTree*) fin2->Get("l1EventTree/L1EventTree");
	TTree * EventTree3 = (TTree*) fin3->Get("l1EventTree/L1EventTree");

	double N1 = EventTree1->GetEntries();
	double N2 = EventTree2->GetEntries();
	double N3 = EventTree2->GetEntries();


	L1Analysis::L1AnalysisEventDataFormat *Event1_ = new L1Analysis::L1AnalysisEventDataFormat();
	L1Analysis::L1AnalysisEventDataFormat *Event2_ = new L1Analysis::L1AnalysisEventDataFormat();
	L1Analysis::L1AnalysisEventDataFormat *Event3_ = new L1Analysis::L1AnalysisEventDataFormat();

	EventTree1->SetBranchStatus("*", 0);
	EventTree1->SetBranchStatus("Event", 1);
	EventTree1->SetBranchStatus("run", 1);
	EventTree1->SetBranchStatus("lumi", 1);
	EventTree1->SetBranchStatus("event", 1);
	EventTree1->SetBranchAddress("Event", &Event1_);


	EventTree2->SetBranchStatus("*", 0);
	EventTree2->SetBranchStatus("Event", 1);
	EventTree2->SetBranchStatus("run", 1);
	EventTree2->SetBranchStatus("lumi", 1);
	EventTree2->SetBranchStatus("event", 1);
	EventTree2->SetBranchAddress("Event", &Event2_);


	EventTree3->SetBranchStatus("*", 0);
	EventTree3->SetBranchStatus("Event", 1);
	EventTree3->SetBranchStatus("run", 1);
	EventTree3->SetBranchStatus("lumi", 1);
	EventTree3->SetBranchStatus("event", 1);
	EventTree3->SetBranchAddress("Event", &Event3_);



	int m = 0;
	int n = 0;
	int p = 0;
	int pushed;
	int mNE;
	int nNE;
	int pNE;
	for(int j = 0; j < N1; j++)
	{
		if(j%10000==0) cout << "Now Working on ZB Event to Generate Run List =  " << j << endl;
		EventTree1->GetEntry(j);
		run1 =  int(Event1_->run);
		//cout << "run 1  = " <<  run1 << endl; 
		if(m == 0){
			run1id.push_back(run1);
			m = m + 1;
		}

		pushed = 0;
		mNE = 0;

		if(m > 0){
			for (std::vector<int>::iterator it = run1id.begin(); it != run1id.end(); ++it)
			{
				//	cout << " j = " << j << "   run 1  = " <<  run1 << " run inside = " << int(*it) << endl;

				if(run1 != int(*it)) 
				{
					mNE = mNE + 1;
				}

				if(mNE > m - 1){
					run1id.push_back(run1);
					m = m + 1;
					pushed = 1;
				}
				if(pushed == 1) break;
			}

		}
	}

	for(int j = 0; j < N2; j++)
	{
		if(j%10000==0) cout << "Now Working on EMBX Event to Generate Run List =  " << j << endl;
		EventTree2->GetEntry(j);
		run2 =  int(Event2_->run);
		if(n == 0){
			run2id.push_back(run2);
			n = n + 1;
		}


		pushed = 0;
		nNE = 0;
		if(n > 0){
			for (std::vector<int>::iterator it = run2id.begin(); it != run2id.end(); ++it)
			{
				if(run2 != int(*it)) 
				{
					nNE = nNE + 1;
				}
				if(nNE > n - 1){
					run2id.push_back(run2);
					n = n + 1;
					pushed = 1;
				}

				if(pushed == 1) break;
			}
		}
	}


	for(int j = 0; j < N3; j++)
	{
		if(j%10000==0) cout << "Now Working on MB Event to Generate Run List =  " << j << endl;
		EventTree3->GetEntry(j);
		run3 =  int(Event3_->run);
		if(p == 0){
			run3id.push_back(run3);
			p = p + 1;
		}


		pushed = 0;
		pNE = 0;
		if(n > 0){
			for (std::vector<int>::iterator it = run3id.begin(); it != run3id.end(); ++it)
			{
				if(run3 != int(*it)) 
				{
					pNE = pNE + 1;
				}
				if(pNE > p - 1){
					run3id.push_back(run3);
					p = p + 1;
					pushed = 1;
				}

				if(pushed == 1) break;
			}
		}
	}


	cout << "Total Distinct Runs ZB = " << m << endl;

	const int NRun1 = m;
	int Run1List[m];
	int index1 = 0;

	for (std::vector<int>::iterator it = run1id.begin(); it != run1id.end(); ++it)
	{
		Run1List[index1] = int(*it);
		cout << "index1  = " << index1 << "   Run ID = " <<  Run1List[index1] << endl;
		index1 = index1 + 1;
	}

	cout << "Total Distinct Runs EMBX = " << n << endl;
	const int NRun2 = n;
	int Run2List[n];
	int index2 = 0;

	for (std::vector<int>::iterator it = run2id.begin(); it != run2id.end(); ++it)
	{
		Run2List[index2] = int(*it);
		cout << "index2  = " << index2 << "   Run ID = " <<  Run2List[index2] << endl;
		index2 = index2 + 1;
	}

	cout << "Total Distinct Runs MB = " << p << endl;
	const int NRun3 = p;
	int Run3List[p];
	int index3 = 0;

	for (std::vector<int>::iterator it = run3id.begin(); it != run3id.end(); ++it)
	{
		Run3List[index3] = int(*it);
		cout << "index3  = " << index3 << "   Run ID = " <<  Run3List[index3] << endl;
		index3 = index3 + 1;
	}



	//End Run ID Collection//


	int HFietaCut = 28;


	const int NFG = 9;
	int Threshold[NFG+1] = {0,10,12,15,17,20,24,30,34,40};
	const double xValue[NFG+1] = {0,10.0,12.0,15.0,17.0,20.0,24.0,30.0,34.0,40.0};

	//	const int NFG = 4;
	//	int Threshold[NFG+1] = {0,10,12,15};
	//	const double xValue[NFG+1] = {0,10.0,12.0,15.0};

	double ORPass[NFG+1][NRun1];
	double ANDPass[NFG+1][NRun1];
	double OReff[NFG+1][NRun1]; 
	double ANDeff[NFG+1][NRun1]; 

	double ORPass3[NFG+1][NRun3];
	double ANDPass3[NFG+1][NRun3];
	double OReff3[NFG+1][NRun3]; 
	double ANDeff3[NFG+1][NRun3]; 


	for(int l = 0; l< NRun1; l++){
		for(int i = 0; i < NFG+1; i++){
			ORPass[i][l]= 0;
			ANDPass[i][l] = 0;
		}
	}

	for(int l = 0; l< NRun3; l++){
		for(int i = 0; i < NFG+1; i++){
			ORPass3[i][l]= 0;
			ANDPass3[i][l] = 0;
		}
	}



	TH1D * hOR[NRun1];
	TH1D * hAND[NRun1];

	TH1D * hORReal[NRun3];
	TH1D * hANDReal[NRun3];

	for(int i=0; i< NRun1; i++){
		hOR[i]= new TH1D(Form("hORRun%d",Run1List[i]),Form("hORRun%d",Run1List[i]),NFG,xValue);
		hOR[i]->SetTitle(Form("MB OR Trigger Efficiency vs ADC Thrshold Run %d (Reemulated)",Run1List[i]));
		hOR[i]->GetXaxis()->SetTitle("ADC Threshold");
		hOR[i]->GetYaxis()->SetTitle("Efficiency");

		hAND[i] = new TH1D(Form("hANDRun%d",Run1List[i]),Form("hANDRun%d",Run1List[i]),NFG,xValue);
		hAND[i]->SetTitle(Form("MB AND Trigger Efficiency vs ADC Thrshold Run %d (Reemulated)",Run1List[i]));
		hAND[i]->GetXaxis()->SetTitle("ADC Threshold");
		hAND[i]->GetYaxis()->SetTitle("Efficiency");
	}



	for(int i=0; i< NRun3; i++){
		hORReal[i]= new TH1D(Form("hORRun%d",Run3List[i]),Form("hORRun%d",Run3List[i]),NFG,xValue);
		hORReal[i]->SetTitle(Form("MB OR Trigger Efficiency vs ADC Thrshold Run %d",Run1List[i]));
		hORReal[i]->GetXaxis()->SetTitle("ADC Threshold");
		hORReal[i]->GetYaxis()->SetTitle("Efficiency");

		hANDReal[i] = new TH1D(Form("hANDRun%d",Run3List[i]),Form("hANDRun%d",Run3List[i]),NFG,xValue);
		hANDReal[i]->SetTitle(Form("MB AND Trigger Efficiency vs ADC Thrshold Run %d",Run3List[i]));
		hANDReal[i]->GetXaxis()->SetTitle("ADC Threshold");
		hANDReal[i]->GetYaxis()->SetTitle("Efficiency");
	}


	TTree * t = (TTree*) fin->Get("ADC");
	t->SetBranchStatus("*", 1);
	t->SetBranchAddress("ADCSize",&ADCSize1);
	t->SetBranchAddress("ADCBoth",ADCBoth1);
	t->SetBranchAddress("ieta",ieta);
	t->SetBranchAddress("iphi",iphi);
	t->AddFriend(EventTree1);

	TTree * t2 = (TTree*) fin2->Get("ADC");
	t2->SetBranchStatus("*", 1);
	t2->SetBranchAddress("ADCSize",&ADCSize2);
	t2->SetBranchAddress("ADCBoth",ADCBoth2);
	t2->SetBranchAddress("ieta",ieta2);
	t2->SetBranchAddress("iphi",iphi2);
	t2->AddFriend(EventTree2);


	TTree * t3 = (TTree*) fin3->Get("ADC");
	t3->SetBranchStatus("*", 1);
	t3->SetBranchAddress("ADCSize",&ADCSize3);
	t3->SetBranchAddress("ADCBoth",ADCBoth3);
	t3->SetBranchAddress("ieta",ieta3);
	t3->SetBranchAddress("iphi",iphi3);
	t3->AddFriend(EventTree3);



	//Declare ADC Max Distribution Histograms//

	TH1D *ADXMax1[NRun1];
	TH1D *ADXMax2[NRun2];
	TH1D *ADXMax3[NRun3];

	for(int i = 0 ; i < NRun1; i++){
		ADXMax1[i] = new TH1D(Form("ADXMax1Run%d",Run1List[i]),Form("ADXMax1Run%d",Run1List[i]),80,0,80);
		ADXMax1[i]->SetTitle(Form("Max ADC Distribution for %s ZB Data Run %d",colsyst.Data(),Run1List[i]));
		ADXMax1[i]->GetXaxis()->SetTitle("ADCMax");
		ADXMax1[i]->GetYaxis()->SetTitle("Counts");
	}

	for(int i = 0 ; i < NRun2; i++){
		ADXMax2[i] = new TH1D(Form("ADXMax2Run%d",Run2List[i]),Form("ADXMax2Run%d",Run2List[i]),80,0,80);
		ADXMax2[i]->SetTitle(Form("Max ADC Distribution for %s Empty Bunches Data Run %d",colsyst.Data(),Run2List[i]));
		ADXMax2[i]->GetXaxis()->SetTitle("ADCMax");
		ADXMax2[i]->GetYaxis()->SetTitle("Counts");
	}

	for(int i = 0 ; i < NRun3; i++){
		ADXMax3[i] = new TH1D(Form("ADXMax3Run%d",Run3List[i]),Form("ADXMax3Run%d",Run3List[i]),80,0,80);
		ADXMax3[i]->SetTitle(Form("Max ADC Distribution for %s MB Data Run %d",colsyst.Data(),Run3List[i]));
		ADXMax3[i]->GetXaxis()->SetTitle("ADCMax");
		ADXMax3[i]->GetYaxis()->SetTitle("Counts");
	}




	//N1 = 60000;
	//N2 = 60000;

	TH1D * h = new TH1D("h","h",80,0,80);
	h->GetXaxis()->SetTitle("ADC");
	h->GetXaxis()->SetTitle("Counts");
	h->SetTitle(Form("All ADC Distribution for %s ZB Data",colsyst.Data()));

	TH1D * h2 = new TH1D("h2","h2",80,0,80);
	h2->GetXaxis()->SetTitle("ADC");
	h2->GetXaxis()->SetTitle("Counts");
	h2->SetTitle(Form("All ADC Distribution for %s Empty Bunches Data",colsyst.Data()));


	TH1D * h3 = new TH1D("h3","h3",80,0,80);
	h3->GetXaxis()->SetTitle("ADC");
	h3->GetXaxis()->SetTitle("Counts");
	h3->SetTitle(Form("All ADC Distribution for %s MB Data",colsyst.Data()));

	/*
	   h->SetTitle("ADC Distribution");
	   h->GetXaxis()->SetTitle("ADC");
	   h->GetYaxis()->SetTitle("Counts");
	   */

	TCanvas *b = new TCanvas("b","b",600,600);
	b->cd();
	TString RunCuts;
	for(int i = 0; i < NRun1; i++){
		RunCuts = Form("run == %d",Run1List[i]);
		t->Project("h","ADCBoth",RunCuts.Data());
		h->SetMarkerColor(kRed);
		TLegend * l = new TLegend(0.35,0.52,0.85,0.73);
		l->AddEntry(h,Form("All ADC Distribution %s ZB Data",colsyst.Data()));
		l->SetTextFont(42);
		h->Draw();
		l->Draw("SAME");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s ZB Run = %d",colsyst.Data(),Run1List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		b->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllZBADCDisLinearRun%d.png",colsyst.Data(),Run1List[i]));
		b->SetLogy();
		b->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllZBADCDisLogRun%d.png",colsyst.Data(),Run1List[i]));
	}	




	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	for(int i = 0; i < NRun2; i++){
		RunCuts = Form("run == %d",Run2List[i]);
		t2->Project("h2","ADCBoth",RunCuts.Data());
		h2->SetMarkerColor(kBlue);
		TLegend * l2 = new TLegend(0.42,0.58,0.88,0.79);
		l2->AddEntry(h2,Form("All ADC Distribution %s Empty Bunches Data",colsyst.Data()));
		l2->SetTextFont(42);
		h2->Draw();
		//	h2->Draw("SAME");
		l2->Draw("SAME");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s EMBX Run = %d",colsyst.Data(),Run2List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		c->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllEMBXADCDisLinearRun%d.png",colsyst.Data(),Run2List[i]));
		c->SetLogy();
		c->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllEMBXADCDisLogRun%d.png",colsyst.Data(),Run2List[i]));
	}



	TCanvas *cMB = new TCanvas("cMB","cMB",600,600);
	cMB->cd();

	for(int i = 0; i < NRun3; i++){
		RunCuts = Form("run == %d",Run3List[i]);
		t3->Project("h3","ADCBoth",RunCuts.Data());
		h3->SetMarkerColor(kBlue);
		TLegend * l2MB = new TLegend(0.42,0.58,0.88,0.79);
		l2MB->AddEntry(h3,Form("All ADC Distribution %s MB Data",colsyst.Data()));
		l2MB->SetTextFont(42);
		h3->Draw();
		//	h2->Draw("SAME");
		l2MB->Draw("SAME");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s MB Run = %d",colsyst.Data(),Run2List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		c->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllMBADCDisLinearRun%d.png",colsyst.Data(),Run3List[i]));
		c->SetLogy();
		c->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllMBADCDisLogRun%d.png",colsyst.Data(),Run3List[i]));
	}





	int RunBoth;
	if(NRun2 > NRun1 || NRun2 == NRun1) RunBoth = NRun1;
	if(NRun2 < NRun1 || NRun2 == NRun1) RunBoth = NRun2;

	TCanvas *d = new TCanvas("d","d",600,600);

	for(int i = 0; i < RunBoth; i++){

		RunCuts = Form("run == %d",Run1List[i]);
		t->Project("h","ADCBoth",RunCuts.Data());

		RunCuts = Form("run == %d",Run2List[i]);
		t2->Project("h2","ADCBoth",RunCuts.Data());

		RunCuts = Form("run == %d",Run3List[i]);
		t3->Project("h3","ADCBoth",RunCuts.Data());

		h2->SetFillColor(kBlue);
		h->SetFillColor(kRed);
		h3->SetFillColor(kGreen);

		h2->Draw();
		h->Draw("SAME");
		TLegend * l7 = new TLegend(0.42,0.58,0.88,0.79);
		l7->AddEntry(h2,Form("All ADC Distribution %s Empty Bunches Data Run = %d",colsyst.Data(),Run2List[i]));
		l7->AddEntry(h,Form("All ADC Distribution %s Zero Bias Data Run = %d",colsyst.Data(),Run1List[i]));
		l7->AddEntry(h3,Form("All ADC Distribution %s MB Data Run = %d",colsyst.Data(),Run3List[i]));
		l7->SetTextFont(42);
		l7->Draw("SAME");
		d->cd();
		d->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllBothADCDisLinear%d.png",colsyst.Data(),i));
		d->SetLogy();
		d->SaveAs(Form("plots%s/RunbyRun/Day1Task/AllBothADCDisLog%d.png",colsyst.Data(),i));
	}

	int maxADC3[NRun3];
	int maxADC2[NRun2];
	int maxADC1[NRun1];
	int maxADCPlus[NRun1];
	int maxADCMinus[NRun1];
	int maxADCPlus3[NRun3];
	int maxADCMinus3[NRun3];
	//	int maxADC2;
	for(int j = 0; j < N1; j++)
	{
		for(int i = 0; i < NRun1; i++){
			maxADC1[i] = 0;
			maxADCPlus[i] = 0;
			maxADCMinus[i] = 0;
		}




		if(j%10000==0) cout << "Now Working on ZB Event =  " << j << endl;
		t->GetEntry(j);
		//cout << "ADCSize = " <<  ADCSize1 << endl;

		for(int i = 0; i < ADCSize1; i++)
		{
			//	cout << "ADCSize1 = " << ADCSize1 << "   i = " << i << "  ADCBoth = " << ADCBoth1[i] << endl;
			for(int l = 0; l < NRun1; l++){
				//cout << "l = " << l << "  ADCBoth1  = " << ADCBoth1[i] << "   maxADC1 = " <<  maxADC1[l]  <<  endl;
				if(ADCBoth1[i] > maxADC1[l] && abs(ieta[i]) > HFietaCut && int(Event1_->run) == Run1List[l]) maxADC1[l] = ADCBoth1[i];
				if(ADCBoth1[i] > maxADCPlus[l] && abs(ieta[i]) > HFietaCut && ieta[i] > 0  && int(Event1_->run) == Run1List[l]) maxADCPlus[l] = ADCBoth1[i];
				if(ADCBoth1[i] > maxADCMinus[l] && abs(ieta[i]) > HFietaCut && ieta[i] < 0  && int(Event1_->run) == Run1List[l]) maxADCMinus[l] = ADCBoth1[i];
			}

		}
		//cout << "maxADC1 = " <<  maxADC1 << endl;

		for(int i = 0; i < NFG+1; i++){
			for(int l = 0; l < NRun1; l++){

				if(maxADC1[l] > Threshold[i] - 1) ORPass[i][l] = ORPass[i][l] + 1;
				if(maxADCPlus[l] > Threshold[i] - 1 && maxADCMinus[l] > Threshold[i] - 1) ANDPass[i][l] = ANDPass[i][l] + 1;
			}

		}

		for(int l = 0; l < NRun1; l++){
			if(maxADC1[l] > 0) ADXMax1[l]->Fill(maxADC1[l]);
		}

	}


	for(int j = 0; j < N2; j++)
	{
		if(j%10000==0) cout << "Now Working on Empty Bunches Event =  " << j << endl;
		t2->GetEntry(j);
		for(int i = 0; i < NRun2; i++){
			maxADC2[i] = 0;
		}

		for(int i =0; i < ADCSize2; i++)
		{
			//cout << "ADCBoth = " << ADCBoth1[i] << endl;

			for(int l = 0; l < NRun2; l++){
				if(ADCBoth2[i] > maxADC2[l] && abs(ieta2[i]) > HFietaCut && int(Event2_->run) == Run2List[l]) maxADC2[l] = ADCBoth2[i];
			}

		}

		for(int l = 0; l < NRun2; l++){
			if(maxADC2[l] > 0) ADXMax2[l]->Fill(maxADC2[l]);
		}
	}


	for(int j = 0; j < N3; j++)
	{
		for(int i = 0; i < NRun3; i++){
			maxADC3[i] = 0;
			maxADCPlus3[i] = 0;
			maxADCMinus3[i] = 0;
		}


		if(j%10000==0) cout << "Now Working on ZB Event =  " << j << endl;
		t3->GetEntry(j);
		//cout << "ADCSize = " <<  ADCSize1 << endl;

		for(int i = 0; i < ADCSize3; i++)
		{
			//	cout << "ADCSize1 = " << ADCSize1 << "   i = " << i << "  ADCBoth = " << ADCBoth1[i] << endl;
			for(int l = 0; l < NRun3; l++){
				//cout << "l = " << l << "  ADCBoth1  = " << ADCBoth1[i] << "   maxADC1 = " <<  maxADC1[l]  <<  endl;
				if(ADCBoth3[i] > maxADC3[l] && abs(ieta3[i]) > HFietaCut && int(Event3_->run) == Run3List[l]) maxADC3[l] = ADCBoth3[i];
				if(ADCBoth3[i] > maxADCPlus3[l] && abs(ieta3[i]) > HFietaCut && ieta3[i] > 0  && int(Event3_->run) == Run3List[l]) maxADCPlus3[l] = ADCBoth3[i];
				if(ADCBoth3[i] > maxADCMinus3[l] && abs(ieta3[i]) > HFietaCut && ieta3[i] < 0  && int(Event3_->run) == Run3List[l]) maxADCMinus3[l] = ADCBoth3[i];
			}

		}
		//cout << "maxADC1 = " <<  maxADC1 << endl;

		for(int i = 0; i < NFG+1; i++){
			for(int l = 0; l < NRun3; l++){

				if(maxADC3[l] > Threshold[i] - 1) ORPass3[i][l] = ORPass3[i][l] + 1;
				if(maxADCPlus3[l] > Threshold[i] - 1 && maxADCMinus3[l] > Threshold[i] - 1) ANDPass3[i][l] = ANDPass3[i][l] + 1;
			}

		}

		for(int l = 0; l < NRun3; l++){
			if(maxADC3[l] > 0) ADXMax3[l]->Fill(maxADC3[l]);
		}

	}





	TCanvas *c2 = new TCanvas("c2","c2",600,600);
	c2->cd();

	for(int i = 0; i < NRun1; i++){
		TLegend * l3 = new TLegend(0.40,0.55,0.88,0.76);
		l3->AddEntry(ADXMax1[i],Form("Max ADC distribution for %s ZB Data",colsyst.Data()),"lp");
		ADXMax1[i]->Draw();
		l3->Draw("SAME");


		TLatex* tex = new TLatex(0.30,0.78,Form("%s ZB Run = %d",colsyst.Data(),Run1List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		c2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxZBLinearRun%d.png",colsyst.Data(),Run1List[i]));
		c2->SetLogy();
		c2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxZBLogRun%d.png",colsyst.Data(),Run1List[i]));
	}


	TCanvas *d2 = new TCanvas("d2","d2",600,600);
	d2->cd();

	for(int i = 0; i < NRun2; i++){
		TLegend * l4 = new TLegend(0.40,0.65,0.88,0.81);
		l4->AddEntry(ADXMax2[i],Form("Max ADC distribution for %s Empty Bunches Data",colsyst.Data()));
		ADXMax2[i]->Draw();
		l4->Draw("SAME");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s EMBX Run = %d",colsyst.Data(),Run2List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		d2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxEMBXLinearRun%d.png",colsyst.Data(),Run2List[i]));
		d2->SetLogy();
		d2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxEMBXLogRun%d.png",colsyst.Data(),Run2List[i]));

	}


	TCanvas *e2 = new TCanvas("e2","e2",600,600);
	e2->cd();

	for(int i = 0; i < NRun3; i++){
		TLegend * l31 = new TLegend(0.40,0.55,0.88,0.76);
		l31->AddEntry(ADXMax3[i],Form("Max ADC distribution for %s MB Data",colsyst.Data()),"lp");
		ADXMax3[i]->Draw();
		l31->Draw("SAME");


		TLatex* tex = new TLatex(0.30,0.78,Form("%s MB Run = %d",colsyst.Data(),Run3List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		e2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxMBLinearRun%d.png",colsyst.Data(),Run3List[i]));
		e2->SetLogy();
		e2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxMBLogRun%d.png",colsyst.Data(),Run3List[i]));
	}


	TCanvas *b2 = new TCanvas("b2","b2",600,600);
	b2->cd();

	for(int i =0; i < RunBoth; i++){
		TLegend * l5 = new TLegend(0.38,0.65,0.88,0.81);
		ADXMax1[i]->SetFillColor(kRed);
		ADXMax2[i]->SetFillColor(kBlue);
		ADXMax3[i]->SetFillColor(kGreen);

		l5->AddEntry(ADXMax1[i],Form("Max ADC distribution for %s ZB Data",colsyst.Data()));
		l5->AddEntry(ADXMax2[i],Form("Max ADC distribution for %s Empty Bunches Data",colsyst.Data()));
		l5->AddEntry(ADXMax3[i],Form("Max ADC distribution for %s MB Data",colsyst.Data()));

		TLatex* tex = new TLatex(0.24,0.78,Form("%s ZB Run = %d, EMBX Run = %d, and MB Run = %d",colsyst.Data(),Run1List[i],Run2List[i],Run3List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		ADXMax2[i]->Draw();
		ADXMax1[i]->Draw("SAME");
		ADXMax3[i]->Draw("SAME");

		l5->Draw("SAME");
		b2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxBothLinearRun%d.png",colsyst.Data(),i));
		b2->SetLogy();
		b2->SaveAs(Form("plots%s/RunbyRun/Day1Task/ADCMaxBothLogRun%d.png",colsyst.Data(),i));
	}

	for(int l = 0; l < NRun1; l++){
		for(int i = 0; i < NFG+1; i ++){
			OReff[i][l] =  ORPass[i][l]/ORPass[0][l];
			ANDeff[i][l] =  ANDPass[i][l]/ORPass[0][l];
			ofstream out1(Form("datFiles%s/RunbyRun/Info%dORRun%d.dat",colsyst.Data(),i,Run1List[l]));
			cout << "RUNID = " << Run1List[l] << endl;
			cout << "ADC OR Thrshold  =  " << Threshold[i] << endl;
			cout << "Total ZB Events =  " << N1 << endl;
			cout << "Total ZB Events For Run =  " << ORPass[0][l] << endl;
			cout << "NEvents Passing MB OR Trigger (Reemulated) = " << ORPass[i][l] << endl;
			cout << "MB OR Efficiency = " << OReff[i][l] << endl;

			ofstream out2(Form("datFiles%s/RunbyRun/Info%dANDRun%d.dat",colsyst.Data(),i,Run1List[l]));
			cout << "RUNID = " << Run1List[l] << endl;
			cout << "ADC AND Thrshold  =  " << Threshold[i] << endl;
			cout << "Total ZB Events =  " << N1 << endl;
			cout << "Total ZB Events For Run =  " << ANDPass[0][l] << endl;
			cout << "NEvents Passing MB AND Trigger (Reemulated)  = " << ANDPass[i][l] << endl;
			cout << "MB AND Efficiency = " << ANDeff[i][l] << endl;	

			hOR[l]->SetBinContent(i+1,OReff[i][l]);
			hAND[l]->SetBinContent(i+1,ANDeff[i][l]);
		}
	}


	for(int l = 0; l < NRun3; l++){
		for(int i = 0; i < NFG+1; i ++){
			OReff3[i][l] =  ORPass3[i][l]/ORPass[0][l];
			ANDeff3[i][l] =  ANDPass3[i][l]/ANDPass[0][l];
			//OReff3[i][l] =  ORPass3[i][l]/N1;
			//ANDeff3[i][l] =  ANDPass3[i][l]/N1;

			ofstream out1(Form("datFiles%s/RunbyRun/Info%dORRun%dReal.dat",colsyst.Data(),i,Run3List[l]));
			cout << "RUNID = " << Run3List[l] << endl;
			cout << "ADC OR Thrshold  =  " << Threshold[i] << endl;
			cout << "Total ZB Events =  " << N1 << endl;
			cout << "Total ZB Events For Run =  " << ORPass[0][l] << endl;
			cout << "NEvents Passing MB OR Trigger (Real) = " << ORPass3[i][l] << endl;
			cout << "MB OR Efficiency = " << OReff3[i][l] << endl;

			ofstream out2(Form("datFiles%s/RunbyRun/Info%dANDRun%dReal.dat",colsyst.Data(),i,Run3List[l]));
			cout << "RUNID = " << Run3List[l] << endl;
			cout << "ADC AND Thrshold  =  " << Threshold[i] << endl;
			cout << "Total ZB Events =  " << N1 << endl;
			cout << "Total ZB Events For Run =  " << ANDPass[0][l] << endl;
			cout << "NEvents Passing MB AND Trigger (Real) = " << ANDPass3[i][l] << endl;
			cout << "MB AND Efficiency = " << ANDeff3[i][l] << endl;	

			hORReal[l]->SetBinContent(i+1,OReff3[i][l]);
			hANDReal[l]->SetBinContent(i+1,ANDeff3[i][l]);
		}
	}




	TCanvas *c3 = new TCanvas("c3","c3",600,600);
	c3->cd();

	for(int i = 0; i < NRun1; i++){
		TLegend * lOR = new TLegend(0.35,0.52,0.85,0.73);
		lOR->AddEntry(hOR[i],Form("MB OR Efficiency vs ADC Thresholds %s Data (Reemulated)",colsyst.Data()),"lp");

		hOR[i]->SetMarkerStyle(21);
		hOR[i]->SetMarkerSize(1.5);
		hOR[i]->SetMarkerColor(kBlue);
		hOR[i]->Draw("p");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s ZB Run = %d",colsyst.Data(),Run1List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		lOR->Draw("SAME");
		c3->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBOREfficiencyRun%d.png",colsyst.Data(),Run1List[i]));
	}

	for(int i = 0; i < NRun1; i++){
		TLegend * lAND = new TLegend(0.35,0.52,0.85,0.73);
		lAND->AddEntry(hAND[i],Form("MB AND Efficiency vs ADC Thresholds %s Data (Reemulated)",colsyst.Data()),"lp");
		hAND[i]->SetMarkerStyle(22);
		hAND[i]->SetMarkerColor(kRed);
		hAND[i]->SetMarkerSize(1.5);
		hAND[i]->Draw("p");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s ZB Run = %d (Reemulated)",colsyst.Data(),Run1List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");


		lAND->Draw("SAME");
		c3->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBANDEfficiencyRun%d.png",colsyst.Data(),Run1List[i]));
	}

	for(int i = 0; i < NRun1; i++){
		TLegend * lBoth = new TLegend(0.35,0.52,0.85,0.73);
		hOR[i]->Draw("p");
		hAND[i]->Draw("pSAME");


		TLatex* tex = new TLatex(0.30,0.78,Form("%s ZB Run = %d",colsyst.Data(),Run1List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");


		lBoth->AddEntry(hOR[i],Form("MB OR Efficiency vs ADC Thresholds %s Data (Reemulated)",colsyst.Data()),"lp");
		lBoth->AddEntry(hAND[i],Form("MB AND Efficiency vs ADC Thresholds %s Data (Reemulated)",colsyst.Data()),"lp");
		lBoth->Draw("SAME");

		c3->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBEfficiencyBothRun%d.png",colsyst.Data(),Run1List[i]));
	}




	TCanvas *c3MB = new TCanvas("c3MB","c3MB",600,600);
	c3MB->cd();

	for(int i = 0; i < NRun3; i++){
		TLegend * lORMB = new TLegend(0.35,0.52,0.85,0.73);
		lORMB->AddEntry(hORReal[i],Form("MB OR Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");

		hORReal[i]->SetMarkerStyle(21);
		hORReal[i]->SetMarkerSize(1.5);
		hORReal[i]->SetMarkerColor(kBlue);
		hORReal[i]->Draw("p");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s MB Run = %d",colsyst.Data(),Run3List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");

		lORMB->Draw("SAME");
		c3MB->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBRealOREfficiencyRun%d.png",colsyst.Data(),Run3List[i]));
	}

	for(int i = 0; i < NRun3; i++){
		TLegend * lANDMB = new TLegend(0.35,0.52,0.85,0.73);
		lANDMB->AddEntry(hANDReal[i],Form("MB AND Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
		hANDReal[i]->SetMarkerStyle(22);
		hANDReal[i]->SetMarkerColor(kRed);
		hANDReal[i]->SetMarkerSize(1.5);
		hANDReal[i]->Draw("p");

		TLatex* tex = new TLatex(0.30,0.78,Form("%s MB Run = %d",colsyst.Data(),Run3List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");


		lANDMB->Draw("SAME");
		c3MB->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBRealANDEfficiencyRun%d.png",colsyst.Data(),Run3List[i]));
	}

	for(int i = 0; i < NRun3; i++){
		TLegend * lBoth = new TLegend(0.35,0.52,0.85,0.73);
		hORReal[i]->Draw("p");
		hANDReal[i]->Draw("pSAME");


		TLatex* tex = new TLatex(0.30,0.78,Form("%s MB Run = %d",colsyst.Data(),Run3List[i]));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");


		lBoth->AddEntry(hORReal[i],Form("MB OR Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
		lBoth->AddEntry(hANDReal[i],Form("MB AND Efficiency vs ADC Thresholds %s Data",colsyst.Data()),"lp");
		lBoth->Draw("SAME");

		c3MB->SaveAs(Form("plots%s/RunbyRun/Day1Task/MBRealEfficiencyBothRun%d.png",colsyst.Data(),Run3List[i]));
	}



	TFile * fout = new TFile(Form("Day1Task%sData.root",colsyst.Data()),"RECREATE");
	fout->cd();
	h->Write();
	h2->Write();
	h3->Write();

	for(int i = 0; i < NRun1; i++){
		ADXMax1[i]->Write();
		hOR[i]->Write();
		hAND[i]->Write();
	}

	for(int i = 0; i < NRun2; i++){
		ADXMax2[i]->Write();
	}

	for(int i = 0; i < NRun3; i++){
		ADXMax3[i]->Write();
		hORReal[i]->Write();
		hANDReal[i]->Write();
	}


	fout->Close();

}






