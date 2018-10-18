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


void PlotADCEnergy2D()
{
	int debug = 0;
	double ZBRate = 1;
	double CentWidth = 0.5;
	int doZoom = 0;

//	int doefficiency=0;
    //int doOR = 0;
	//int doAND = 1;
	int doEMBX = 0;
	TString label;
	//if(doOR == 1) label = "OR";
	//if(doAND == 1) label = "AND";

	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";



	double pmin = 0.0;
	double pmax = 1.6;
	double Centmin = 0;
	double Centmax = 100;
	int CentBins = (Centmax - Centmin)/CentWidth;
	double CentminAll = 0;
	double CentmaxAll = 100;
	int CentBinsAll = (CentmaxAll - CentminAll)/CentWidth;

	double ADCtoGeV = 0.3;

	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
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
	double Cent;

	double legb = 0.64; 
	double legt = 0.89; 
	double legl = 0.35; 
	double legr = 0.75; 

	int nTowersMatched;
	int Pass;
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
	t->SetBranchAddress("Cent",&Cent);



	int NEvents = t->GetEntries();
	//NEvents = 20000;




	TH1D * CentHis = new TH1D("CentHis","CentHis",100,0,100);

	t->Project("CentHis","Cent");
	CentHis->SetMinimum(0);
	CentHis->GetXaxis()->SetTitle("Cent");
	CentHis->GetYaxis()->SetTitle("Counts");
	CentHis->SetTitle("Centrality Distribution");

	TCanvas *c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	CentHis->Draw("hist e");
	c1->SaveAs(Form("plots%s/effCent/Cent.png",colsyst.Data()));

	int HFietaCut = 28;



	cout << "START" << endl;
	TCanvas *c2 = new TCanvas("c2","c2",1800,1200);
	c2->cd();


	const int NFG = 10;


	//	Double_t FGThres[NFG] = {0,10,12,15,17};
	//		int ADCLabel[NFG] = {0,10,12,15,17};
	Double_t FGThres[NFG] = {0,10,12,15,17,20,24,30,34,40};
	int ADCLabel[NFG] = {0,10,12,15,17,20,24,30,34,40};

	/*
	   const int NFG = 1;
	//	Double_t FGThres[NFG] = {0,10,12,15,17};
	//	int ADCLabel[NFG] = {0,10,12,15,17};
	Double_t FGThres[NFG] = {0};
	int ADCLabel[NFG] = {0};

*/


	double TotalShort[NFG];
	double TotalLong[NFG];
	double TotalOr[NFG];
	double TotalSt[NFG];

	double TotalShortAND[NFG];
	double TotalLongAND[NFG];
	double TotalOrAND[NFG];
	double TotalStAND[NFG];

	TH1D * ShortHis[NFG];
	TH1D * LongHis[NFG];
	TH1D * OrHis[NFG];
	TH1D * AllOrHis[NFG];

	TH1D * CentShortHis[NFG];
	TH1D * CentLongHis[NFG];
	TH1D * CentOrHis[NFG];
	TH1D * CentAllOrHis[NFG];


	TH1D * ShortHisAND[NFG];
	TH1D * LongHisAND[NFG];
	TH1D * OrHisAND[NFG];
	TH1D * AllOrHisAND[NFG];

	TH1D * CentShortHisAND[NFG];
	TH1D * CentLongHisAND[NFG];
	TH1D * CentOrHisAND[NFG];
	TH1D * CentAllOrHisAND[NFG];




	TH1D * ADCHis = new TH1D("ADCHis","ADCHis",101,0,100);
	TH1D * ADCShortHis = new TH1D("ADCShortHis","ADCShortHis",101,0,100);
	TH1D * ADCLongHis = new TH1D("ADCLongHis","ADCLongHis",101,0,100);

	TH1D * ADCMaxHis = new TH1D("ADCMaxHis","ADCMaxHis",15,10,25);
	TH1D * ADCShortMaxHis = new TH1D("ADCShortMaxHis","ADCShortMaxHis",15,10,25);
	TH1D * ADCLongMaxHis = new TH1D("ADCLongMaxHis","ADCLongMaxHis",15,10,25);

	TH2D * ietaHis1 = new TH2D("ietaHis1","ietaHis1",83,-41,41,72,1,72);
	TH2D * ietaHis2 = new TH2D("ietaHis2","ietaHis2",83,-41,41,72,1,72);
	TH2D * ietaHis3 = new TH2D("ietaHis3","ietaHis3",83,-41,41,72,1,72);
	TH2D * ietaHis4 = new TH2D("ietaHis4","ietaHis4",83,-41,41,72,1,72);

	ietaHis1->GetXaxis()->SetTitle("ieta");	
	ietaHis1->GetYaxis()->SetTitle("iphi");
	ietaHis1->SetTitle("eta phi distribution for ADCMax = 15");

	ietaHis2->GetXaxis()->SetTitle("ieta");	
	ietaHis2->GetYaxis()->SetTitle("iphi");
	ietaHis2->SetTitle("eta phi distribution for ADCMax = 16");

	ietaHis3->GetXaxis()->SetTitle("ieta");	
	ietaHis3->GetYaxis()->SetTitle("iphi");
	ietaHis3->SetTitle("eta phi distribution for ADCMax = 17");

	ietaHis4->GetXaxis()->SetTitle("ieta");	
	ietaHis4->GetYaxis()->SetTitle("iphi");
	ietaHis4->SetTitle("eta phi distribution for ADCMax = 18");


	double maxHF;
	int kmax =0;
	TH1D * hShort =  new TH1D("hShort","hShort",NFG-1,FGThres);
	hShort->GetXaxis()->SetTitle("Short ADC Threshold OR");
	hShort->GetYaxis()->SetTitle("Efficiency");
	hShort->SetTitle("Trigger Efficiency for Short ADC OR Threshold");

	TH1D * hLong =  new TH1D("hLong","hLong",NFG-1,FGThres);
	hLong->GetXaxis()->SetTitle("Long ADC Threshold OR");
	hLong->GetYaxis()->SetTitle("Efficiency");
	hLong->SetTitle("Trigger Efficiency for Long ADC OR Threshold");

	TH1D * hOr =  new TH1D("hOr","hOr",NFG-1,FGThres);
	hOr->GetXaxis()->SetTitle("Short or Long ADC Threshold");
	hOr->GetYaxis()->SetTitle("Efficiency");
	hOr->SetTitle("Trigger Efficiency for Short OR Long ADC OR Threshold");

	TH1D * hSt =  new TH1D("hSt","hSt",NFG-1,FGThres);
	hSt->GetXaxis()->SetTitle("Standard ADC OR Threshold");
	hSt->GetYaxis()->SetTitle("Efficiency");
	hSt->SetTitle("Standard Trigger OR Efficiency");

	//Decalre AND//
	
	TH1D * hShortAND =  new TH1D("hShort","hShort",NFG-1,FGThres);
	hShortAND->GetXaxis()->SetTitle("Short ADC Threshold");
	hShortAND->GetYaxis()->SetTitle("Efficiency");
	hShortAND->SetTitle("Trigger Efficiency for Short ADC AND Threshold");

	TH1D * hLongAND =  new TH1D("hLong","hLong",NFG-1,FGThres);
	hLongAND->GetXaxis()->SetTitle("Long ADC Threshold");
	hLongAND->GetYaxis()->SetTitle("Efficiency");
	hLongAND->SetTitle("Trigger Efficiency for Long ADC AND Threshold");

	TH1D * hOrAND =  new TH1D("hOr","hOr",NFG-1,FGThres);
	hOrAND->GetXaxis()->SetTitle("Short or Long ADC Threshold");
	hOrAND->GetYaxis()->SetTitle("Efficiency");
	hOrAND->SetTitle("Trigger Efficiency for Short OR Long ADC AND Threshold");

	TH1D * hStAND =  new TH1D("hSt","hSt",NFG-1,FGThres);
	hStAND->GetXaxis()->SetTitle("Standard ADC AND Threshold");
	hStAND->GetYaxis()->SetTitle("Efficiency");
	hStAND->SetTitle("Standard AND Trigger Efficiency");



	for(int i = 0; i < NFG; i++)
	{
		TotalShort[i] = 0;
		TotalLong[i] = 0;
		TotalOr[i] = 0;
		TotalSt[i] = 0;
		TotalShortAND[i] = 0;
		TotalLongAND[i] = 0;
		TotalOrAND[i] = 0;
		TotalStAND[i] = 0;


		//OR Histograms//
		ShortHis[i] = new TH1D("ShortHis","ShortHis",50,0,50);
		ShortHis[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		ShortHis[i]->GetYaxis()->SetTitle("Number of Events");
		ShortHis[i]->SetTitle(Form("HF Energy (GeV) Distribution with Shortcolsyst.Data(),-ADC OR > %d", ADCLabel[i]));

		LongHis[i] = new TH1D("LongHis","LongHis",50,0,50);
		LongHis[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		LongHis[i]->GetYaxis()->SetTitle("Number of Events");
		LongHis[i]->SetTitle(Form("HF Energy (GeV) Distribution with Long-ADC OR > %d", ADCLabel[i]));

		OrHis[i] = new TH1D("OrHis","OrHis",50,0,50);
		OrHis[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		OrHis[i]->GetYaxis()->SetTitle("Number of Events");
		OrHis[i]->SetTitle(Form("HF Energy (GeV) Distribution with (Short OR Long) ADC OR > %d", ADCLabel[i]));

		AllOrHis[i] = new TH1D("AllOrHis","AllOrHis",50,0,50);
		AllOrHis[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		AllOrHis[i]->GetYaxis()->SetTitle("Number of Events");
		AllOrHis[i]->SetTitle(Form("HF Energy (GeV) Distribution with Any ADC OR > %d", ADCLabel[i]));


		CentShortHis[i] = new TH1D("CentShortHis","CentShortHis",CentBins, Centmin, Centmax);
		CentShortHis[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentShortHis[i]->GetYaxis()->SetTitle("Efficiency");
		CentShortHis[i]->SetTitle(Form("Efficiency vs Centrality with Short ADC OR > %d PbPb MC", ADCLabel[i]));

		CentLongHis[i] = new TH1D("CentLongHis","CentLongHis",CentBins,Centmin,Centmax);
		CentLongHis[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentLongHis[i]->GetYaxis()->SetTitle("Efficiency");
		CentLongHis[i]->SetTitle(Form("Efficiency vs Centrality with Long ADC OR > %d PbPb MC", ADCLabel[i]));

		CentOrHis[i] = new TH1D("CentOrHis","CentOrHis",CentBins,Centmin,Centmax);
		CentOrHis[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentOrHis[i]->GetYaxis()->SetTitle("Efficiency");
		CentOrHis[i]->SetTitle(Form("Efficiency vs Centrality with (Short OR Long) ADC OR > %d PbPb MC", ADCLabel[i]));

		CentAllOrHis[i] = new TH1D("CentAllOrHis","CentAllOrHis",CentBinsAll,CentminAll,CentmaxAll);
		CentAllOrHis[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentAllOrHis[i]->GetYaxis()->SetTitle("Efficiency");
		CentAllOrHis[i]->SetTitle(Form("Efficiency vs Centrality with Any ADC OR > %d PbPb MC", ADCLabel[i]));

		//AND Histograms//
	
		ShortHisAND[i] = new TH1D("ShortHis","ShortHis",50,0,50);
		ShortHisAND[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		ShortHisAND[i]->GetYaxis()->SetTitle("Number of Events");
		ShortHisAND[i]->SetTitle(Form("HF Energy (GeV) Distribution with Short-ADC AND > %d", ADCLabel[i]));

		LongHisAND[i] = new TH1D("LongHis","LongHis",50,0,50);
		LongHisAND[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		LongHisAND[i]->GetYaxis()->SetTitle("Number of Events");
		LongHisAND[i]->SetTitle(Form("HF Energy (GeV) Distribution with Long-ADC AND > %d", ADCLabel[i]));

		OrHisAND[i] = new TH1D("OrHis","OrHis",50,0,50);
		OrHisAND[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		OrHisAND[i]->GetYaxis()->SetTitle("Number of Events");
		OrHisAND[i]->SetTitle(Form("HF Energy (GeV) Distribution with (Short OR Long) ADC AND > %d", ADCLabel[i]));

		AllOrHisAND[i] = new TH1D("AllOrHis","AllOrHis",50,0,50);
		AllOrHisAND[i]->GetXaxis()->SetTitle("HF Energy (GeV)");
		AllOrHisAND[i]->GetYaxis()->SetTitle("Number of Events");
		AllOrHisAND[i]->SetTitle(Form("HF Energy (GeV) Distribution with Any ADC AND > %d", ADCLabel[i]));


		CentShortHisAND[i] = new TH1D("CentShortHis","CentShortHis",CentBins, Centmin, Centmax);
		CentShortHisAND[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentShortHisAND[i]->GetYaxis()->SetTitle("Efficiency");
		CentShortHisAND[i]->SetTitle(Form("Efficiency vs Centrality with Short ADC AND > %d PbPb MC", ADCLabel[i]));

		CentLongHisAND[i] = new TH1D("CentLongHis","CentLongHis",CentBins,Centmin,Centmax);
		CentLongHisAND[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentLongHisAND[i]->GetYaxis()->SetTitle("Efficiency");
		CentLongHisAND[i]->SetTitle(Form("Efficiency vs Centrality with Long ADC AND > %d PbPb MC", ADCLabel[i]));

		CentOrHisAND[i] = new TH1D("CentOrHis","CentOrHis",CentBins,Centmin,Centmax);
		CentOrHisAND[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentOrHisAND[i]->GetYaxis()->SetTitle("Efficiency");
		CentOrHisAND[i]->SetTitle(Form("Efficiency vs Centrality with (Short OR Long) ADC AND > %d PbPb MC", ADCLabel[i]));

		CentAllOrHisAND[i] = new TH1D("CentAllOrHis","CentAllOrHis",CentBinsAll,CentminAll,CentmaxAll);
		CentAllOrHisAND[i]->GetXaxis()->SetTitle("Centrality (Percent)");
		CentAllOrHisAND[i]->GetYaxis()->SetTitle("Efficiency");
		CentAllOrHisAND[i]->SetTitle(Form("Efficiency vs Centrality with Any ADC AND > %d PbPb MC", ADCLabel[i]));

	}

	//NEvents = 10;
	//	cout << "i = " << i << endl;
	for(int j = 0; j< NEvents; j++){
		if(j%10000==0) cout << "Now Working on Event =  " << j << endl;
		float etmax = 0;
		int ADCmax = 0;
		int AllShortMax=0;
		int AllLongMax=0;
		int FrontShortMax=0;
		int FrontLongMax=0;
		int BackShortMax=0;
		int BackLongMax=0;
		int ADCmaxieta=0;
		int ADCmaxiphi=0;
		ADCCellMax[0] = 0;
		ADCCellMax[1] = 0;
		ADCCellMax[2] = 0;
		ADCCellMax[3] = 0;
		FrontADCCellMax[0] = 0;
		FrontADCCellMax[1] = 0;
		FrontADCCellMax[2] = 0;
		FrontADCCellMax[3] = 0;

		BackADCCellMax[0] = 0;
		BackADCCellMax[1] = 0;
		BackADCCellMax[2] = 0;
		BackADCCellMax[3] = 0;
		int HFMatchedietaMaxE;
		int HFMatchediphiMaxE;
		t->GetEntry(j);

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

		for(int k = 0; k < nTowersMatched; k++){
			int AllShort=0;
			int AllLong=0;
			int FrontShort=0;
			int FrontLong=0;
			int BackShort=0;
			int BackLong=0;
			int p = 0;
			ADCCell[0] = 0;
			ADCCell[1] = 0;
			ADCCell[2] = 0;
			ADCCell[3] = 0;
			FrontADCCell[0] = 0;
			FrontADCCell[1] = 0;
			FrontADCCell[2] = 0;
			FrontADCCell[3] = 0;
			BackADCCell[0] = 0;
			BackADCCell[1] = 0;
			BackADCCell[2] = 0;
			BackADCCell[3] = 0;

			//cout << "k = " << k << endl;
			if(abs(HFMatchedieta[k]) > HFietaCut){

				for(int l = 0; l < ADCSizeMatched; l++)
				{
					if(abs(ADCMatchedieta[l]) > HFietaCut){

						if(ADCMatchedieta[l] ==  HFMatchedieta[k] && ADCMatchediphi[l]  ==  HFMatchediphi[k]){
							//	cout << "Pass 2" << endl;
							//				cout << "l = " << l << "   ADCMatcheddepth [l] = " << ADCMatcheddepth[l] << endl;

							if(ADCMatcheddepth[l] == 1 || ADCMatcheddepth[l] == 3)
							{
								AllShort = 	AllShort + ADCBothMatched[l];
								if(ADCMatchedieta[l]  > 0)	FrontShort  = FrontShort + ADCBothMatched[l];
								if(ADCMatchedieta[l]  < 0)	BackShort  = BackShort + ADCBothMatched[l];

								//		cout << "AllShort = " << AllShort << endl;

							}
							if(ADCMatcheddepth[l] == 2 || ADCMatcheddepth[l] == 4)
							{
								AllLong = 	AllLong + ADCBothMatched[l];
								if(ADCMatchedieta[l]  > 0)		FrontLong  = FrontLong + ADCBothMatched[l];
								if(ADCMatchedieta[l]  < 0)  	BackLong  = BackLong + ADCBothMatched[l];
							}
							ADCCell[p] = ADCBothMatched[l];
							//cout << "p = " << p << "  ADCCell[p] = " << ADCCell[p] << "ADCBothMatched[l] = " << ADCBothMatched[l] << endl;
							if(ADCMatchedieta[l]  > 0)	FrontADCCell[p] = ADCBothMatched[l];
							if(ADCMatchedieta[l]  < 0)		BackADCCell[p] = ADCBothMatched[l];
							p = p + 1;
							ADCHis->Fill(ADCBothMatched[l]);
							if(ADCBothMatched[l] > ADCmax) 
							{
								ADCmax = ADCBothMatched[l];
								ADCmaxieta = ADCMatchedieta[l];
								ADCmaxiphi = ADCMatchediphi[l];
							}
						}
					}

				}
				if(AllLong > 0)	ADCLongHis->Fill(AllLong);
				if(AllShort > 0)		ADCShortHis->Fill(AllShort);


			}

			if(AllShort > AllShortMax) AllShortMax = AllShort;
			if(AllLong > AllLongMax) AllLongMax = AllLong;
			if( ADCCell[0] > ADCCellMax[0]) ADCCellMax[0] = ADCCell[0];
			if( ADCCell[1] > ADCCellMax[1]) ADCCellMax[1] = ADCCell[1];
			if( ADCCell[2] > ADCCellMax[2]) ADCCellMax[2] = ADCCell[2];
			if( ADCCell[3] > ADCCellMax[3]) ADCCellMax[3] = ADCCell[3];

			if(FrontShort > FrontShortMax) FrontShortMax = FrontShort;
			if(FrontLong > FrontLongMax) FrontLongMax = FrontLong;
			if( FrontADCCell[0] > FrontADCCellMax[0]) FrontADCCellMax[0] = FrontADCCell[0];
			if( FrontADCCell[1] > FrontADCCellMax[1]) FrontADCCellMax[1] = FrontADCCell[1];
			if( FrontADCCell[2] > FrontADCCellMax[2]) FrontADCCellMax[2] = FrontADCCell[2];
			if( FrontADCCell[3] > FrontADCCellMax[3]) FrontADCCellMax[3] = FrontADCCell[3];

			if(BackShort > BackShortMax) BackShortMax = BackShort;
			if(BackLong > BackLongMax) BackLongMax = BackLong;
			if( BackADCCell[0] > BackADCCellMax[0]) BackADCCellMax[0] = BackADCCell[0];
			if( BackADCCell[1] > BackADCCellMax[1]) BackADCCellMax[1] = BackADCCell[1];
			if( BackADCCell[2] > BackADCCellMax[2]) BackADCCellMax[2] = BackADCCell[2];
			if( BackADCCell[3] > BackADCCellMax[3]) BackADCCellMax[3] = BackADCCell[3];


		}
		if(ADCCellMax[0] > AllShortMax && ADCCellMax[0] > AllLongMax) cout << "BUG!!!" << "  ADCCellMax = " << ADCCellMax[0] << "  AllShortMax = " << AllShortMax << "  AllLongMax = " << AllLongMax << endl;
		if(ADCCellMax[1] > AllShortMax && ADCCellMax[1] > AllLongMax) cout << "BUG!!!" << "  ADCCellMax = " << ADCCellMax[1] << "  AllShortMax = " << AllShortMax << "  AllLongMax = " << AllLongMax << endl;
		if(ADCCellMax[2] > AllShortMax && ADCCellMax[2] > AllLongMax) cout << "BUG!!!" << "  ADCCellMax = " << ADCCellMax[2] << "  AllShortMax = " << AllShortMax << "  AllLongMax = " << AllLongMax << endl;
		if(ADCCellMax[3] > AllShortMax && ADCCellMax[3] > AllLongMax) cout << "BUG!!!" << "  ADCCellMax = " << ADCCellMax[3] << "  AllShortMax = " << AllShortMax << "  AllLongMax = " << AllLongMax << endl;


		ADCMaxHis->Fill(ADCmax);
		ADCLongMaxHis->Fill(AllLongMax);
		ADCShortMaxHis->Fill(AllShortMax);

		if(ADCmax == 15) ietaHis1->Fill(ADCmaxieta,ADCmaxiphi);
		if(ADCmax == 16) ietaHis2->Fill(ADCmaxieta,ADCmaxiphi);
		if(ADCmax == 17) ietaHis3->Fill(ADCmaxieta,ADCmaxiphi);
		if(ADCmax == 18) ietaHis4->Fill(ADCmaxieta,ADCmaxiphi);

		for(int i = 0; i < NFG; i++){ 
				
				//OR Logic//
				if(AllShortMax > FGThres[i] - 1){
					ShortHis[i]->Fill(etmax);
					CentShortHis[i]->Fill(Cent);
					TotalShort[i] = TotalShort[i] + 1;
				}

				if(AllLongMax > FGThres[i] - 1){
					LongHis[i]->Fill(etmax);
					CentLongHis[i]->Fill(Cent);
					TotalLong[i] = TotalLong[i] + 1;


				}

				if((AllLongMax > FGThres[i] - 1) || (AllShortMax > FGThres[i] - 1)){
					OrHis[i]->Fill(etmax);
					CentOrHis[i]->Fill(Cent);
					TotalOr[i] = TotalOr[i] + 1;
				}

				if(ADCCellMax[0] > FGThres[i] - 1 || ADCCellMax[1] > FGThres[i] - 1 || ADCCellMax[2] > FGThres[i] - 1 || ADCCellMax[3] > FGThres[i] - 1 ){
					AllOrHis[i]->Fill(etmax);
					CentAllOrHis[i]->Fill(Cent);
					TotalSt[i] = TotalSt[i] + 1;

				}		

				
			
			
				//AND Logic//
				
				if(FrontShortMax > FGThres[i] - 1 && BackShortMax > FGThres[i] - 1){
					ShortHisAND[i]->Fill(etmax);
					CentShortHisAND[i]->Fill(Cent);
					TotalShortAND[i] = TotalShortAND[i] + 1;

				}

				if(FrontLongMax > FGThres[i] - 1 && BackLongMax > FGThres[i] - 1){
					LongHisAND[i]->Fill(etmax);
					CentLongHisAND[i]->Fill(Cent);
					TotalLongAND[i] = TotalLongAND[i] + 1;

				}

				if((FrontShortMax > FGThres[i] - 1 && BackShortMax > FGThres[i] - 1) || (FrontLongMax > FGThres[i] - 1 && BackLongMax > FGThres[i] - 1)){
					OrHisAND[i]->Fill(etmax);
					CentOrHisAND[i]->Fill(Cent);
					TotalOrAND[i] = TotalOrAND[i] + 1;

				}

				if( (FrontADCCellMax[0] > FGThres[i] - 1 && BackADCCellMax[0] > FGThres[i] - 1)  || (FrontADCCellMax[1] > FGThres[i] - 1 && BackADCCellMax[1] > FGThres[i] - 1) || (FrontADCCellMax[2] > FGThres[i] - 1 && BackADCCellMax[2] > FGThres[i] - 1) || (FrontADCCellMax[3] > FGThres[i] - 1 && BackADCCellMax[3] > FGThres[i] - 1) ){
					AllOrHisAND[i]->Fill(etmax);
					CentAllOrHisAND[i]->Fill(Cent);
					TotalStAND[i] = TotalStAND[i] + 1;

				}		

			if(debug == 1){
				cout << "----- New Event START ---- " << endl;

				cout << "AllLongMax = " << AllLongMax  << endl;
				cout << "AllShortMax = " << AllShortMax  << endl;
				cout << "ADCCell[0]Max = " << ADCCellMax[0]  << endl;
				cout << "ADCCell[1]Max = " << ADCCellMax[1]  << endl;
				cout << "ADCCell[2]Max = " << ADCCellMax[2]  << endl;
				cout << "ADCCell[3]Max = " << ADCCellMax[3]  << endl;
				cout << "i = " << i << "   Total Short  = "	<< TotalShort[i] << endl;	
				cout << "i = " << i << "   Total Long  = "	<< TotalLong[i] << endl;	
				cout << "i = " << i << "   Total Long OR Short  = "	<< TotalOr[i] << endl;	
				cout << "i = " << i << "   Total Standard  = "	<< TotalSt[i] << endl;	
				cout << "----- New Event DONE ---- " << endl;
			}
		}


	}

	ietaHis1->Draw("colz");
	c2->SaveAs(Form("plot%s/ADCDis/ietaHis1_%d.png",colsyst.Data(),doEMBX));
	ietaHis2->Draw("colz");
	c2->SaveAs(Form("plot%s/ADCDis/ietaHis2_%d.png",colsyst.Data(),doEMBX));
	ietaHis3->Draw("colz");
	c2->SaveAs(Form("plot%s/ADCDis/ietaHis3_%d.png",colsyst.Data(),doEMBX));
	ietaHis4->Draw("colz");
	c2->SaveAs(Form("plot%s/ADCDis/ietaHis4_%d.png",colsyst.Data(),doEMBX));

	for(int i = 0; i <NFG; i++){
		ShortHis[i]->Draw();
		c2->SaveAs(Form("plots%s/new/ShortOR%d.png",colsyst.Data(),ADCLabel[i]));

		LongHis[i]->Draw();
		c2->SaveAs(Form("plots%s/new/LongOR%d.png",colsyst.Data(),ADCLabel[i]));

		OrHis[i]->Draw();
		c2->SaveAs(Form("plots%s/new/OrOR%d.png",colsyst.Data(),ADCLabel[i]));

		ShortHisAND[i]->Draw();
		c2->SaveAs(Form("plots%s/new/ShortAND%d.png",colsyst.Data(),ADCLabel[i]));
		LongHisAND[i]->Draw();
		c2->SaveAs(Form("plots%s/new/LongAND%d.png",colsyst.Data(),ADCLabel[i]));
		OrHisAND[i]->Draw();
		c2->SaveAs(Form("plots%s/new/OrAND%d.png",colsyst.Data(),ADCLabel[i]));



		double	EfficiencyShort = TotalShort[i]/TotalShort[0];
		double	EfficiencyLong = TotalLong[i]/TotalLong[0];
		double	EfficiencyOr = TotalOr[i]/TotalOr[0];
		double	EfficiencySt = TotalSt[i]/TotalSt[0];



		double	EfficiencyShortAND = TotalShortAND[i]/TotalShortAND[0];
		double	EfficiencyLongAND = TotalLongAND[i]/TotalLongAND[0];
		double	EfficiencyOrAND = TotalOrAND[i]/TotalOrAND[0];
		double	EfficiencyStAND = TotalStAND[i]/TotalStAND[0];



		hShort->SetBinContent(i,EfficiencyShort);
		hLong->SetBinContent(i,EfficiencyLong);
		hOr->SetBinContent(i,EfficiencyOr);
		hSt->SetBinContent(i,EfficiencySt);

		hShort->Scale(ZBRate);
		hLong->Scale(ZBRate);
		hOr->Scale(ZBRate);
		hSt->Scale(ZBRate);
	


		hShortAND->SetBinContent(i,EfficiencyShortAND);
		hLongAND->SetBinContent(i,EfficiencyLongAND);
		hOrAND->SetBinContent(i,EfficiencyOrAND);
		hStAND->SetBinContent(i,EfficiencyStAND);

		hShortAND->Scale(ZBRate);
		hLongAND->Scale(ZBRate);
		hOrAND->Scale(ZBRate);
		hStAND->Scale(ZBRate);
	


		cout << "ADC Thrshold OR =  " << FGThres[i] << endl;
		cout << "ZB Events =  " << NEvents << endl;
		cout << "Long Or Short Events =  " <<  TotalOr[i] << endl;
		cout << "Rate Long Or Short Trigger =  " << EfficiencyOr * ZBRate << " (kHz) " << endl;
		cout << "Standard Trigger Events =  " <<  TotalSt[i] << endl;
		cout << "Rate Standard Trigger =  "<< EfficiencySt * ZBRate << " (kHz) " << endl;
		cout << "Relative Rates "<<  TotalOr[i]/TotalSt[i]  << endl;

		ofstream out1(Form("%sRate%dOR.dat",colsyst.Data(),i));

		out1 << "ADC Thrshold OR =  " << FGThres[i] << endl;
		out1 << "ZB Events =  " << NEvents << endl;
		out1 << "Long Or Short Events =  " <<  TotalOr[i] << endl;
		out1 << "Rate Long Or Short Trigger =  " << EfficiencyOr * ZBRate << " (kHz) " << endl;
		out1 << "Standard Trigger Events =  " <<  TotalSt[i] << endl;
		out1 << "Rate Standard Trigger =  "<< EfficiencySt * ZBRate << " (kHz) " << endl;
		out1 << "Relative Rates "<< TotalOr[i]/TotalSt[i]  << endl;



		cout << "ADC Thrshold AND =  " << FGThres[i] << endl;
		cout << "ZB Events =  " << NEvents << endl;
		cout << "Long Or Short Events =  " <<  TotalOrAND[i] << endl;
		cout << "Rate Long Or Short Trigger =  " << EfficiencyOrAND * ZBRate << " (kHz) " << endl;
		cout << "Standard Trigger Events =  " <<  TotalStAND[i] << endl;
		cout << "Rate Standard Trigger =  "<< EfficiencyStAND * ZBRate << " (kHz) " << endl;
		cout << "Relative Rates "<<  TotalOrAND[i]/TotalStAND[i]  << endl;

		ofstream out2(Form("%sRate%dAND.dat",colsyst.Data(),i));

		out1 << "ADC Thrshold AND =  " << FGThres[i] << endl;
		out1 << "ZB Events =  " << NEvents << endl;
		out1 << "Long Or Short Events =  " <<  TotalOrAND[i] << endl;
		out1 << "Rate Long Or Short Trigger =  " << EfficiencyOrAND * ZBRate << " (kHz) " << endl;
		out1 << "Standard Trigger Events =  " <<  TotalStAND[i] << endl;
		out1 << "Rate Standard Trigger =  "<< EfficiencyStAND * ZBRate << " (kHz) " << endl;
		out1 << "Relative Rates "<< TotalOrAND[i]/TotalStAND[i]  << endl;




	}
	c2 = new TCanvas("c2","c2",600,600);
	ADCShortMaxHis->SetLineColor(2);
	ADCLongMaxHis->SetLineColor(3);
	ADCMaxHis->SetLineColor(4);
	ADCShortMaxHis->SetLineWidth(2);
	ADCLongMaxHis->SetLineWidth(2);
	ADCMaxHis->SetLineWidth(2);


	TLegend * lADCMax = new TLegend(legl,legb,legr,legt);
	lADCMax->AddEntry(ADCShortMaxHis,"ADCShort Sum");
	lADCMax->AddEntry(ADCLongMaxHis,"ADCLong Sum");
	lADCMax->AddEntry(ADCMaxHis,"ADC All No Sum");

	ADCMaxHis->GetXaxis()->SetTitle("Max ADC");
	ADCMaxHis->GetYaxis()->SetTitle("Counts");
	ADCMaxHis->SetTitle("Max ADC Distribution");

	ADCMaxHis->SetMaximum(100000);		
	ADCMaxHis->SetMinimum(0);
	ADCMaxHis->Draw();
	ADCLongMaxHis->Draw("SAME");
	ADCShortMaxHis->Draw("SAME");
	lADCMax->Draw("SAME");
	c2->SaveAs(Form("plots%s/ADCDis/ADCMaxDistribution%d.png",colsyst.Data(),doEMBX));

	ADCShortHis->SetLineColor(2);
	ADCLongHis->SetLineColor(3);
	ADCHis->SetLineColor(4);
	ADCShortHis->SetLineWidth(2);
	ADCLongHis->SetLineWidth(2);
	ADCHis->SetLineWidth(2);

	TLegend * lADC = new TLegend(legl,legb,legr,legt);
	lADC->AddEntry(ADCShortHis,"ADCShort Sum");
	lADC->AddEntry(ADCLongHis,"ADCLong Sum");
	lADC->AddEntry(ADCHis,"ADC All No Sum");

	ADCHis->GetXaxis()->SetTitle("ADC");
	ADCHis->GetYaxis()->SetTitle("Counts");
	ADCHis->SetTitle("ADC Distribution");


	ADCHis->Draw();
	ADCLongHis->Draw("SAME");
	ADCShortHis->Draw("SAME");
	lADC->Draw("SAME");
	c2->SaveAs(Form("plots%s/ADCDis/ADCDistribution%d.png",colsyst.Data(),doEMBX));



	TFile * fADC = new TFile(Form("ADC%d.root",doEMBX),"RECREATE");
	fADC->cd();
	ADCMaxHis->Write();
	ADCLongMaxHis->Write();
	ADCShortMaxHis->Write();
	ADCHis->Write();
	ADCLongHis->Write();
	ADCShortHis->Write();
	ietaHis1->Write();
	ietaHis2->Write();
	ietaHis3->Write();
	ietaHis4->Write();
	fADC->Close();

	TLegend * l = new TLegend(legl,legb,legr,legt);
	TLegend * l2 = new TLegend(legl,legb,legr,legt);
	TLegend * l4 = new TLegend(legl,legb,legr,legt);
	TLegend * l5 = new TLegend(legl,legb,legr,legt);
	TLegend * l6 = new TLegend(legl,legb,legr,legt);
	TLegend * l7 = new TLegend(legl,legb,legr,legt);
	TLegend * lOR = new TLegend(0.35,0.72,0.75,0.98);
	TLegend * l15 = new TLegend(legl,legb,legr,legt);
	TLegend * l16 = new TLegend(legl,legb,legr,legt);

	TLegend * l8[NFG]; 

	TLegend * lAND  = new TLegend(legl,legb,legr,legt);
	TLegend * l2AND  = new TLegend(legl,legb,legr,legt);
	TLegend * l4AND  = new TLegend(legl,legb,legr,legt);
	TLegend * l5AND = new TLegend(legl,legb,legr,legt);
	TLegend * l6AND = new TLegend(legl,legb,legr,legt);

	TLegend * lORAND = new TLegend(0.35,0.72,0.75,0.98);
	TLegend * l15AND = new TLegend(legl,legb,legr,legt);
	TLegend * l16AND = new TLegend(legl,legb,legr,legt);
	TLegend * l8AND[NFG]; 


	l->SetTextFont(42);
	l2->SetTextFont(42);
	l4->SetTextFont(42);
	l5->SetTextFont(42);
	l6->SetTextFont(42);
	l7->SetTextFont(42);
	lOR->SetTextFont(42);

	c2->cd();
	for(int i = 1; i < NFG; i++)
	{
		CentOrHis[i]->SetFillColor(i+2);

		if(i == 1)
		{	CentOrHis[i]->Sumw2();
			CentOrHis[0]->Sumw2();
			CentOrHis[i]->Divide(CentOrHis[0]);
			CentOrHis[i]->SetMinimum(pmin);
			CentOrHis[i]->SetMaximum(pmax);		
			CentOrHis[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds");
			CentOrHis[i]->Draw("hist e");

		}


		if(i > 1) 
		{	CentOrHis[i]->Sumw2();
			CentOrHis[0]->Sumw2();
			CentOrHis[i]->Divide(CentOrHis[0]);
			CentOrHis[i]->Draw("hist eSAME");

		}
		l5->AddEntry(CentOrHis[i],Form("Long OR Short - ADC OR > %d",ADCLabel[i]));
		l5->Draw("SAME");

	}
	c2->SaveAs(Form("plots%s/effCent/OrEffCentOR.png",colsyst.Data()));

	for(int i = 1; i < NFG; i++)
	{
		CentOrHisAND[i]->SetFillColor(i+2);

		if(i == 1)
		{	CentOrHisAND[i]->Sumw2();
			CentOrHisAND[0]->Sumw2();
			CentOrHisAND[i]->Divide(CentOrHis[0]);
			CentOrHisAND[i]->SetMinimum(pmin);
			CentOrHisAND[i]->SetMaximum(pmax);		
			CentOrHisAND[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds");
			CentOrHisAND[i]->Draw("hist e");

		}


		if(i > 1) 
		{	CentOrHisAND[i]->Sumw2();
			CentOrHisAND[0]->Sumw2();
			CentOrHisAND[i]->Divide(CentOrHis[0]);
			CentOrHisAND[i]->Draw("hist eSAME");

		}
		l5AND->AddEntry(CentOrHisAND[i],Form("Long OR Short - ADC AND > %d",ADCLabel[i]));
		l5AND->Draw("SAME");

	}
	c2->SaveAs(Form("plots%s/effCent/OrEffCentAND.png",colsyst.Data()));



	for(int i = 1; i < NFG; i++)
	{
		CentAllOrHis[i]->SetFillColor(i+2);

		if(i == 1)
		{
			CentAllOrHis[i]->Sumw2();
			CentAllOrHis[0]->Sumw2();
			CentAllOrHis[i]->Divide(CentAllOrHis[0]);
			CentAllOrHis[i]->SetMinimum(pmin);
			CentAllOrHis[i]->SetMaximum(pmax);		
			CentAllOrHis[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds for Any ADC Trigger");
			CentAllOrHis[i]->Draw("hist e");

		}


		if(i > 1) 
		{
			CentAllOrHis[0]->Sumw2();
			CentAllOrHis[i]->Sumw2();
			CentAllOrHis[i]->Divide(CentAllOrHis[0]);
			CentAllOrHis[i]->Draw("hist eSAME");

		}
		l6->AddEntry(CentAllOrHis[i],Form("Any ADC OR > %d",ADCLabel[i]));

		l6->Draw("SAME");
	}
	c2->SaveAs(Form("plots%s/effCent/AllOrEffCentOR.png",colsyst.Data()));
	
		for(int i = 1; i < NFG; i++)
	{
		CentAllOrHisAND[i]->SetFillColor(i+2);

		if(i == 1)
		{
			CentAllOrHisAND[i]->Sumw2();
			CentAllOrHisAND[0]->Sumw2();
			CentAllOrHisAND[i]->Divide(CentAllOrHis[0]);
			CentAllOrHisAND[i]->SetMinimum(pmin);
			CentAllOrHisAND[i]->SetMaximum(pmax);		
			CentAllOrHisAND[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds for Any ADC Trigger");
			CentAllOrHisAND[i]->Draw("hist e");

		}


		if(i > 1) 
		{
			CentAllOrHisAND[0]->Sumw2();
			CentAllOrHisAND[i]->Sumw2();
			CentAllOrHisAND[i]->Divide(CentAllOrHis[0]);
			CentAllOrHisAND[i]->Draw("hist eSAME");

		}
		l6AND->AddEntry(CentAllOrHisAND[i],Form("Any ADC AND > %d",ADCLabel[i]));

		l6AND->Draw("SAME");
	}
	c2->SaveAs(Form("plots%s/effCent/AllOrEffCentAND.png",colsyst.Data()));

	
	
	if(doZoom == 1){
		for(int i = 1; i < NFG; i++)
		{
			CentOrHis[i]->SetFillColor(i+2);

			if(i == 1)
			{	CentOrHis[i]->Sumw2();
				CentOrHis[0]->Sumw2();
				CentOrHis[i]->Divide(CentOrHis[0]);
				CentOrHis[i]->SetMinimum(pmin);
				CentOrHis[i]->SetMaximum(pmax);		
				CentOrHis[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds");
				CentOrHis[i]->GetXaxis()->SetRangeUser(75,100);
				CentOrHis[i]->Draw("hist e");

			}


			if(i > 1) 
			{	CentOrHis[i]->Sumw2();
				CentOrHis[0]->Sumw2();
				CentOrHis[i]->Divide(CentOrHis[0]);
				CentOrHis[i]->Draw("hist eSAME");

			}
			l15->AddEntry(CentOrHis[i],Form("Long OR Short - ADCOR > %d",ADCLabel[i]));
			l15->Draw("SAME");

		}
		c2->SaveAs(Form("plots%s/effCent/OrEffCentZoomOR.png",colsyst.Data()));


			for(int i = 1; i < NFG; i++)
		{
			CentOrHisAND[i]->SetFillColor(i+2);

			if(i == 1)
			{	CentOrHisAND[i]->Sumw2();
				CentOrHisAND[0]->Sumw2();
				CentOrHisAND[i]->Divide(CentOrHis[0]);
				CentOrHisAND[i]->SetMinimum(pmin);
				CentOrHisAND[i]->SetMaximum(pmax);		
				CentOrHisAND[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds");
				CentOrHisAND[i]->GetXaxis()->SetRangeUser(75,100);
				CentOrHisAND[i]->Draw("hist e");

			}


			if(i > 1) 
			{	CentOrHisAND[i]->Sumw2();
				CentOrHisAND[0]->Sumw2();
				CentOrHisAND[i]->Divide(CentOrHis[0]);
				CentOrHisAND[i]->Draw("hist eSAME");

			}
			l15AND->AddEntry(CentOrHisAND[i],Form("Long OR Short - ADC AND > %d",ADCLabel[i]));
			l15AND->Draw("SAME");

		}
		c2->SaveAs(Form("plots%s/effCent/OrEffCentZoomAND.png",colsyst.Data()));



		for(int i = 1; i < NFG; i++)
		{
			CentAllOrHis[i]->SetFillColor(i+2);

			if(i == 1)
			{
				CentAllOrHis[i]->Sumw2();
				CentAllOrHis[0]->Sumw2();
				CentAllOrHis[i]->Divide(CentAllOrHis[0]);
				CentAllOrHis[i]->SetMinimum(pmin);
				CentAllOrHis[i]->SetMaximum(pmax);		
				CentAllOrHis[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds for Any ADC Trigger");
				CentAllOrHis[i]->GetXaxis()->SetRangeUser(75,100);
				CentAllOrHis[i]->Draw("hist e");

			}


			if(i > 1) 
			{
				CentAllOrHis[0]->Sumw2();
				CentAllOrHis[i]->Sumw2();
				CentAllOrHis[i]->Divide(CentAllOrHis[0]);
				CentAllOrHis[i]->Draw("hist eSAME");

			}
			l16->AddEntry(CentAllOrHis[i],Form("Any ADC OR > %d",ADCLabel[i]));

			l16->Draw("SAME");

		}
		c2->SaveAs(Form("plots%s/effCent/AllOrEffCentZoomOR.png",colsyst.Data()));


		for(int i = 1; i < NFG; i++)
		{
			CentAllOrHisAND[i]->SetFillColor(i+2);

			if(i == 1)
			{
				CentAllOrHisAND[i]->Sumw2();
				CentAllOrHisAND[0]->Sumw2();
				CentAllOrHisAND[i]->Divide(CentAllOrHis[0]);
				CentAllOrHisAND[i]->SetMinimum(pmin);
				CentAllOrHisAND[i]->SetMaximum(pmax);		
				CentAllOrHisAND[i]->SetTitle("Efficiency vs Centrality for Different ADC Thresholds for Any ADC Trigger");
				CentAllOrHisAND[i]->GetXaxis()->SetRangeUser(75,100);
				CentAllOrHisAND[i]->Draw("hist e");

			}


			if(i > 1) 
			{
				CentAllOrHisAND[0]->Sumw2();
				CentAllOrHisAND[i]->Sumw2();
				CentAllOrHisAND[i]->Divide(CentAllOrHis[0]);
				CentAllOrHisAND[i]->Draw("hist eSAME");

			}
			l16->AddEntry(CentAllOrHisAND[i],Form("Any ADC AND > %d",ADCLabel[i]));

			l16->Draw("SAME");

		}
		c2->SaveAs(Form("plots%s/effCent/AllOrEffCentZoomAND.png",colsyst.Data()));


	}

	



	for(int i = 1; i < NFG; i++)
	{
		l8[i] = new TLegend(legl,legb,legr,legt);
		CentAllOrHis[i]->SetTitle(Form("Efficiency vs Centrality with ADC OR > %d Thresold",ADCLabel[i]));
		CentAllOrHis[i]->SetFillColor(3);
		CentOrHis[i]->SetFillColor(4);

		CentOrHis[i]->SetMaximum(1.6);
		CentOrHis[i]->Draw("hist e");
		CentAllOrHis[i]->Draw("hist eSAME");
		l8[i]->AddEntry(CentOrHis[i],Form("ADC OR > %d Long OR Short Sum",ADCLabel[i]));
		l8[i]->AddEntry(CentAllOrHis[i],Form("Any ADC OR > %d (Standard Trigger)",ADCLabel[i]));
		l8[i]->Draw("SAME");
		c2->SaveAs(Form("plots%s/effCent/BothTrigEffOR%d.png",colsyst.Data(),i));

	}

	for(int i = 1; i < NFG; i++)
	{
		l8AND[i] = new TLegend(legl,legb,legr,legt);
		CentAllOrHisAND[i]->SetTitle(Form("Efficiency vs Centrality with ADC AND > %d Thresold",ADCLabel[i]));
		CentAllOrHisAND[i]->SetFillColor(3);
		CentOrHisAND[i]->SetFillColor(4);

		CentOrHisAND[i]->SetMaximum(1.6);
		CentOrHisAND[i]->Draw("hist e");
		CentAllOrHisAND[i]->Draw("hist eSAME");
		l8AND[i]->AddEntry(CentOrHisAND[i],Form("ADC AND > %d Long OR Short Sum",ADCLabel[i]));
		l8AND[i]->AddEntry(CentAllOrHisAND[i],Form("Any ADC AND > %d (Standard Trigger)",ADCLabel[i]));
		l8AND[i]->Draw("SAME");
		c2->SaveAs(Form("plots%s/effCent/BothTrigEffAND%d.png",colsyst.Data(),i));

	}


	for(int i = 1; i < NFG; i++)
	{
		ShortHis[i]->SetFillColor(i+2);
		if(i == 1){
			ShortHis[i]->SetTitle("HF Energy (GeV) Distribution with Short Thresholds Cuts ADC OR");
			ShortHis[i]->Draw("hist e");

		}

		if(i > 1) 	ShortHis[i]->Draw("SAME");

		l->AddEntry(ShortHis[i],Form("Short - ADC OR > %d",ADCLabel[i]));

		l->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/ShortOR.png",colsyst.Data()));

	for(int i = 1; i < NFG; i++)
	{
		ShortHisAND[i]->SetFillColor(i+2);
		if(i == 1){
			ShortHisAND[i]->SetTitle("HF Energy (GeV) Distribution with Short Thresholds Cuts ADC AND");
			ShortHisAND[i]->Draw("hist e");

		}

		if(i > 1) 	ShortHisAND[i]->Draw("SAME");

		lAND->AddEntry(ShortHisAND[i],Form("Short - ADC AND > %d",ADCLabel[i]));

		lAND->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/ShortAND.png",colsyst.Data()));




	for(int i = 1; i < NFG; i++)
	{
		LongHis[i]->SetFillColor(i+2);

		if(i == 1) {
			LongHis[i]->SetTitle("HF Energy (GeV) Distribution with Long Thresholds Cuts ADC OR");
			LongHis[i]->Draw("hist e");
		}

		if(i  > 1) 	LongHis[i]->Draw("SAME");
		l2->AddEntry(LongHis[i],Form("Long - ADC OR > %d",ADCLabel[i]));
		l2->Draw("hist eSAME");


	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/LongOR.png",colsyst.Data()));


	for(int i = 1; i < NFG; i++)
	{
		LongHisAND[i]->SetFillColor(i+2);

		if(i == 1) {
			LongHisAND[i]->SetTitle("HF Energy (GeV) Distribution with Long Thresholds Cuts ADC AND");
			LongHisAND[i]->Draw("hist e");

		}

		if(i  > 1) 	LongHisAND[i]->Draw("SAME");
		l2AND->AddEntry(LongHisAND[i],Form("Long - ADC AND > %d",ADCLabel[i]));
		l2AND->Draw("hist eSAME");


	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/LongAND.png",colsyst.Data()));




	for(int i = 1; i < NFG; i++)
	{
		OrHis[i]->SetFillColor(i+2);

		if(i == 1) {
			OrHis[i]->SetTitle("HF Energy (GeV) Distribution with Long OR Short Thresholds Cuts ADC OR");
			OrHis[i]->Draw("hist e");

		}



		if(i > 1) 	OrHis[i]->Draw("SAME");

		l4->AddEntry(OrHis[i],Form("Long OR Short - ADC OR > %d",ADCLabel[i]));
		l4->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/LongOrShortOR.png",colsyst.Data()));




	for(int i = 1; i < NFG; i++)
	{
		OrHisAND[i]->SetFillColor(i+2);

		if(i == 1) {
			OrHisAND[i]->SetTitle("HF Energy (GeV) Distribution with Long OR Short Thresholds Cuts ADC AND");
			OrHisAND[i]->Draw("hist e");

		}



		if(i > 1) 	OrHis[i]->Draw("SAME");

		l4AND->AddEntry(OrHis[i],Form("Long OR Short - ADC AND > %d",ADCLabel[i]));
		l4AND->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/LongOrShortAND.png",colsyst.Data()));






	for(int i = 1; i < NFG; i++)
	{
		AllOrHis[i]->SetFillColor(i+2);
		if(i == 1){ 	
			AllOrHis[i]->Draw();
			LongHis[i]->SetTitle("HF Energy (GeV) Distribution with Standard Trigger Cuts ADC OR");

		}

		if(i > 1) 	AllOrHis[i]->Draw("hist eSAME");

		lOR->AddEntry(AllOrHis[i],Form("Long OR Short - ADC OR > %d",ADCLabel[i]));

		lOR->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/AnyOR.png",colsyst.Data()));


	for(int i = 1; i < NFG; i++)
	{
		AllOrHisAND[i]->SetFillColor(i+2);
		if(i == 1){ 	
			AllOrHisAND[i]->Draw();
			AllOrHisAND[i]->SetTitle("HF Energy (GeV) Distribution with Standard Trigger Cuts ADC AND");

		}

		if(i > 1) 	AllOrHisAND[i]->Draw("hist eSAME");

		lORAND->AddEntry(AllOrHisAND[i],Form("Long OR Short - ADC AND > %d",ADCLabel[i]));

		lORAND->Draw("hist eSAME");

	}
	c2->SetLogy();
	c2->SaveAs(Form("plots%s/new/AnyAND.png",colsyst.Data()));



		TCanvas *c5 = new TCanvas("c5","c5",600,600);
		c5->cd();
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/ShortEfficiencyOR.png",colsyst.Data()));
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/LongEfficiencyOR.png",colsyst.Data()));
		hOr->Draw();
		c5->SaveAs(Form("plots%s/efficiency/OrEfficiencyOR.png",colsyst.Data()));
		TLegend * l10; 
		if(doPbPb == 2)	 l10 = new TLegend(legl,legb,legr,legt);
		if(doPbPb == 1)	 l10 = new TLegend(0.15,0.15,0.65,0.55);
		if(doPbPb == 0)	 l10 = new TLegend(legl,legb,legr,legt);

		l10->SetTextFont(42);

		hShort->SetMarkerColor(kRed);
		hLong->SetMarkerColor(kBlue);
		hOr->SetMarkerColor(kGreen);
		hSt->SetMarkerColor(kBlack);

		hShort->SetMarkerStyle(23);
		hLong->SetMarkerStyle(23);
		hOr->SetMarkerStyle(23);
		hSt->SetMarkerStyle(23);

		hShort->SetMarkerSize(1);
		hLong->SetMarkerSize(1);
		hOr->SetMarkerSize(1);
		hSt->SetMarkerSize(1);
		hShort->GetXaxis()->SetTitle("ADC OR Threshold");
		hShort->SetTitle("Trigger Efficiencies for ADC OR Threshold");

		hShort->Draw("p");
		hLong->Draw("pSAME");
		hOr->Draw("pSAME");
		hSt->Draw("pSAME");

		l10->AddEntry(hShort,"Short ADC Efficiency","pl");
		l10->AddEntry(hLong,"Long ADC Efficiency","pl");
		l10->AddEntry(hOr,"Short OR Long ADC Efficiency","pl");
		l10->AddEntry(hSt,"Standard Trigger","pl");

		l10->Draw("SAME");

		c5->SaveAs(Form("plots%s/efficiency/AllEfficiencyOR.png",colsyst.Data()));

		hShortAND->Draw();
		c5->SaveAs(Form("plots%s/efficiency/ShortEfficiencyOR.png",colsyst.Data()));
		hShortAND->Draw();
		c5->SaveAs(Form("plots%s/efficiency/LongEfficiencyOR.png",colsyst.Data()));
		hOrAND->Draw();
		c5->SaveAs(Form("plots%s/efficiency/OrEfficiencyOR.png",colsyst.Data()));
		TLegend * l10AND; 
		if(doPbPb == 2)	 l10AND = new TLegend(legl,legb,legr,legt);
		if(doPbPb == 1)	 l10AND = new TLegend(0.15,0.15,0.65,0.55);
		if(doPbPb == 0)	 l10AND = new TLegend(legl,legb,legr,legt);

		l10AND->SetTextFont(42);

		hShortAND->SetMarkerColor(kRed);
		hLongAND->SetMarkerColor(kBlue);
		hOrAND->SetMarkerColor(kGreen);
		hStAND->SetMarkerColor(kBlack);

		hShortAND->SetMarkerStyle(23);
		hLongAND->SetMarkerStyle(23);
		hOrAND->SetMarkerStyle(23);
		hStAND->SetMarkerStyle(23);

		hShortAND->SetMarkerSize(1);
		hLongAND->SetMarkerSize(1);
		hOrAND->SetMarkerSize(1);
		hStAND->SetMarkerSize(1);
		hShortAND->GetXaxis()->SetTitle("ADC AND Threshold");
		hShortAND->SetTitle("Trigger Efficiencies for ADC AND Threshold");

		hShortAND->Draw("p");
		hLongAND->Draw("pSAME");
		hOrAND->Draw("pSAME");
		hStAND->Draw("pSAME");

		l10AND->AddEntry(hShort,"Short ADC Efficiency","pl");
		l10AND->AddEntry(hLong,"Long ADC Efficiency","pl");
		l10AND->AddEntry(hOr,"Short OR Long ADC Efficiency","pl");
		l10AND->AddEntry(hSt,"Standard Trigger","pl");

		l10AND->Draw("SAME");

		c5->SaveAs(Form("plots%s/efficiency/AllEfficiencyAND.png",colsyst.Data()));




	TFile * fout = new TFile(Form("MBTriggerPlots%s.root",colsyst.Data()),"RECREATE");
	fout->cd();
	for(int i = 1; i < NFG; i++)
	{
		LongHis[i]->Write();
		ShortHis[i]->Write();
		OrHis[i]->Write();
		AllOrHis[i]->Write();
		CentShortHis[i]->Write();
		CentLongHis[i]->Write();
		CentOrHis[i]->Write();
		CentAllOrHis[i]->Write();

		LongHisAND[i]->Write();
		ShortHisAND[i]->Write();
		OrHisAND[i]->Write();
		AllOrHisAND[i]->Write();
		CentShortHisAND[i]->Write();
		CentLongHisAND[i]->Write();
		CentOrHisAND[i]->Write();
		CentAllOrHisAND[i]->Write();

		hShort->Write();
		hLong->Write();
		hOr->Write();
		hSt->Write();

		hShortAND->Write();
		hLongAND->Write();
		hOrAND->Write();
		hStAND->Write();

	}
	fout->Close();
/*
	for(int i = 0; i < NFG; i++)
	{
		TLegend * l3 = new TLegend(0.55,0.52,0.75,0.73);
		ShortHis[i]->SetFillColor(2);
		LongHis[i]->SetFillColor(3);
		LongHis[i]->SetTitle("Long - Short ADC Comparisons");
		LongHis[i]->Draw();
		ShortHis[i]->Draw("hist eSAME");
		l3->AddEntry(ShortHis[i],Form("Short - ADC > %d",ADCLabel[i]));
		l3->AddEntry(LongHis[i],Form("Long - ADC > %d",ADCLabel[i]));
		l3->Draw("SAME");
		c2->SetLogy();
		c2->SaveAs(Form("plots%s/new/LongShort%d.png",colsyst.Data(),i));

	}
*/

/*


	if(doefficiency == 1){

		const int NFG = 11;
		int step = 3;
		double FGThres[NFG];
		double TotalShort[NFG];
		double TotalLong[NFG];
		double TotalOr[NFG];
		double TotalSt[NFG];

		int kmax =0;
		TH1D * hShort =  new TH1D("hShort","hShort",NFG,0,NFG*step);
		hShort->GetXaxis()->SetTitle("Short ADC Threshold");
		hShort->GetYaxis()->SetTitle("Efficiency");
		hShort->SetTitle("Trigger Efficiency for Short ADC Threshold");

		TH1D * hLong =  new TH1D("hLong","hLong",NFG,0,NFG*step);
		hLong->GetXaxis()->SetTitle("Long ADC Threshold");
		hLong->GetYaxis()->SetTitle("Efficiency");
		hLong->SetTitle("Trigger Efficiency for Long ADC Threshold");

		TH1D * hOr =  new TH1D("hOr","hOr",NFG,0,NFG*step);
		hOr->GetXaxis()->SetTitle("Short or Long ADC Threshold");
		hOr->GetYaxis()->SetTitle("Efficiency");
		hOr->SetTitle("Trigger Efficiency for Short OR Long ADC Threshold");

		TH1D * hSt =  new TH1D("hSt","hSt",NFG,0,NFG*step);
		hSt->GetXaxis()->SetTitle("Short or Long ADC Threshold");
		hSt->GetYaxis()->SetTitle("Efficiency");
		hSt->SetTitle("Standard Trigger Efficiency");



		for(int i = 0; i < NFG; i++)
		{
			FGThres[i] = i * step;
			TotalShort[i] = 0;
			TotalLong[i] = 0;
			TotalOr[i] = 0;
			TotalSt[i] = 0;

			for(int j = 0; j< NEvents; j++){
				float etmax = 0;
				int ADCmax = 0;
				int AllShortMax=0;
				int AllLongMax=0;
				int FrontShortMax=0;
				int FrontLongMax=0;
				int BackShortMax=0;
				int BackLongMax=0;


				ADCCellMax[0] = 0;
				ADCCellMax[1] = 0;
				ADCCellMax[2] = 0;
				ADCCellMax[3] = 0;
				FrontADCCellMax[0] = 0;
				FrontADCCellMax[1] = 0;
				FrontADCCellMax[2] = 0;
				FrontADCCellMax[3] = 0;

				BackADCCellMax[0] = 0;
				BackADCCellMax[1] = 0;
				BackADCCellMax[2] = 0;
				BackADCCellMax[3] = 0;

				int HFMatchedietaMaxE;
				int HFMatchediphiMaxE;
				t->GetEntry(j);
				for(int k = 0; k < nTowersMatched; k++){
					int AllShort=0;
					int AllLong=0;
					int FrontShort=0;
					int FrontLong=0;
					int BackShort=0;
					int BackLong=0;
					int p = 0;

					if(abs(HFMatchedieta[k]) > HFietaCut){

						for(int l = 0; l < ADCSizeMatched; l++)
						{
							if(abs(ADCMatchedieta[l]) > HFietaCut){
								//	cout << "Pass 1" << endl;
								if(ADCMatchedieta[l] ==  HFMatchedieta[k] && ADCMatchediphi[l]  ==  HFMatchediphi[k]){
									//	cout << "Pass 2" << endl;
									//		cout << "l = " << l << "   ADCMatcheddepth [l] = " << ADCMatcheddepth[l] << endl;

									if(ADCMatcheddepth[l] == 1 || ADCMatcheddepth[l] == 3)
									{
										AllShort = 	AllShort + ADCBothMatched[l];
										if(ADCMatchedieta[l]  > 0)	FrontShort  = FrontShort + ADCBothMatched[l];
										if(ADCMatchedieta[l]  < 0)	BackShort  = BackShort + ADCBothMatched[l];

										//	cout << "AllShort = " << AllShort << endl;

									}
									if(ADCMatcheddepth[l] == 2 || ADCMatcheddepth[l] == 4)
									{
										AllLong = 	AllLong + ADCBothMatched[l];
										if(ADCMatchedieta[l]  > 0)		FrontLong  = FrontLong + ADCBothMatched[l];
										if(ADCMatchedieta[l]  < 0)  	BackLong  = BackLong + ADCBothMatched[l];
									}
									ADCCell[p] = ADCBothMatched[l];
									//cout << "p = " << p << "  ADCCell[p] = " << ADCCell[p] << "ADCBothMatched[l] = " << ADCBothMatched[l] << endl;
									if(ADCMatchedieta[l]  > 0)	FrontADCCell[p] = ADCBothMatched[l];
									if(ADCMatchedieta[l]  < 0)		BackADCCell[p] = ADCBothMatched[l];
									p = p + 1;
								}
							}
		TCanvas *c5 = new TCanvas("c5","c5",600,600);
		c5->cd();
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/ShortEfficiency%s.png",colsyst.Data(),label.Data()));
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/LongEfficiency%s.png",colsyst.Data(),label.Data()));
		hOr->Draw();
		c5->SaveAs(Form("plots%s/efficiency/OrEfficiency%s.png",colsyst.Data(),label.Data()));
		TLegend * l10; 
		if(doPbPb == 2)	 l10 = new TLegend(legl,legb,legr,legt);
		if(doPbPb == 1)	 l10 = new TLegend(0.15,0.15,0.65,0.55);
		if(doPbPb == 0)	 l10 = new TLegend(legl,legb,legr,legt);

		l10->SetTextFont(42);

		hShort->SetMarkerColor(kRed);
		hLong->SetMarkerColor(kBlue);
		hOr->SetMarkerColor(kGreen);
		hSt->SetMarkerColor(kBlack);

		hShort->SetMarkerStyle(23);
		hLong->SetMarkerStyle(23);
		hOr->SetMarkerStyle(23);
		hSt->SetMarkerStyle(23);

		hShort->SetMarkerSize(1);
		hLong->SetMarkerSize(1);
		hOr->SetMarkerSize(1);
		hSt->SetMarkerSize(1);
		hShort->GetXaxis()->SetTitle(Form("ADC %s Threshold",label.Data()));
		hShort->SetTitle(Form("Trigger Efficiencies for ADC %s Threshold",label.Data()));

		hShort->Draw("ep");
		hLong->Draw("epSAME");
		hOr->Draw("epSAME");
		hSt->Draw("epSAME");

		l10->AddEntry(hShort,"Short ADC Efficiency","pl");
		l10->AddEntry(hLong,"Long ADC Efficiency","pl");
		l10->AddEntry(hOr,"Short OR Long ADC Efficiency","pl");
		l10->AddEntry(hSt,"Standard Trigger","pl");

		l10->Draw("SAME");

		c5->SaveAs(Form("plots%s/efficiency/AllEfficiency%s.png",colsyst.Data(),label.Data()));

		hShort->Scale(25);
		hLong->Scale(25);
		hOr->Scale(25);
		hSt->Scale(25);
						}

					}
					if(AllShort > AllShortMax) AllShortMax = AllShort;
					if(AllLong > AllLongMax) AllLongMax = AllLong;
					if( ADCCell[0] > ADCCellMax[0]) ADCCellMax[0] = ADCCell[0];
					if( ADCCell[1] > ADCCellMax[1]) ADCCellMax[1] = ADCCell[1];
					if( ADCCell[2] > ADCCellMax[2]) ADCCellMax[2] = ADCCell[2];
					if( ADCCell[3] > ADCCellMax[3]) ADCCellMax[3] = ADCCell[3];

					if(FrontShort > FrontShortMax) FrontShortMax = FrontShort;
					if(FrontLong > FrontLongMax) FrontLongMax = FrontLong;
					if( FrontADCCell[0] > FrontADCCellMax[0]) FrontADCCellMax[0] = FrontADCCell[0];
					if( FrontADCCell[1] > FrontADCCellMax[1]) FrontADCCellMax[1] = FrontADCCell[1];
					if( FrontADCCell[2] > FrontADCCellMax[2]) FrontADCCellMax[2] = FrontADCCell[2];
					if( FrontADCCell[3] > FrontADCCellMax[3]) FrontADCCellMax[3] = FrontADCCell[3];

					if(BackShort > BackShortMax) BackShortMax = BackShort;
					if(BackLong > BackLongMax) BackLongMax = BackLong;
					if( BackADCCell[0] > BackADCCellMax[0]) BackADCCellMax[0] = BackADCCell[0];
					if( BackADCCell[1] > BackADCCellMax[1]) BackADCCellMax[1] = BackADCCell[1];
					if( BackADCCell[2] > BackADCCellMax[2]) BackADCCellMax[2] = BackADCCell[2];
					if( BackADCCell[3] > BackADCCellMax[3]) BackADCCellMax[3] = BackADCCell[3];
				}
				if(doOR == 1){
					if(AllShortMax > FGThres[i] - 1){
						TotalShort[i] = TotalShort[i] + 1;
					}

					if(AllLongMax > FGThres[i] - 1){
						TotalLong[i] = TotalLong[i] + 1;

					}

					if(AllLongMax > FGThres[i] - 1 || AllShortMax > FGThres[i] - 1){
						TotalOr[i] = TotalOr[i] + 1;
					}

					if(ADCCellMax[0] > FGThres[i] - 1 || ADCCellMax[1] > FGThres[i] - 1 || ADCCellMax[2] > FGThres[i] - 1 || ADCCellMax[3] > FGThres[i] - 1 ){
						TotalSt[i] = TotalSt[i] + 1;

					}		

				}


				if(doAND == 1){
					if(FrontShortMax > FGThres[i] - 1 && BackShortMax > FGThres[i] - 1){
						TotalShort[i] = TotalShort[i] + 1;

					}

					if(FrontLongMax > FGThres[i] - 1 && BackLongMax > FGThres[i] - 1){
						TotalLong[i] = TotalLong[i] + 1;

					}

					if((FrontShortMax > FGThres[i] - 1 && BackShortMax > FGThres[i] - 1) || (FrontLongMax > FGThres[i] - 1 && BackLongMax > FGThres[i] - 1)){
						TotalOr[i] = TotalOr[i] + 1;
					}

					if( (FrontADCCellMax[0] > FGThres[i] - 1 && BackADCCellMax[0] > FGThres[i] - 1)  || (FrontADCCellMax[1] > FGThres[i] - 1 && BackADCCellMax[1] > FGThres[i] - 1) || (FrontADCCellMax[2] > FGThres[i] - 1 && BackADCCellMax[2] > FGThres[i] - 1) || (FrontADCCellMax[3] > FGThres[i] - 1 && BackADCCellMax[3] > FGThres[i] - 1) ){
						TotalSt[i] = TotalSt[i] + 1;
					}		

				}


			}



			double	EfficiencyShort = TotalShort[i]/TotalShort[0];
			double	EfficiencyLong = TotalLong[i]/TotalLong[0];
			double	EfficiencyOr = TotalOr[i]/TotalOr[0];
			double	EfficiencySt = TotalSt[i]/TotalSt[0];

			hShort->SetBinContent(i,EfficiencyShort);
			hLong->SetBinContent(i,EfficiencyLong);
			hOr->SetBinContent(i,EfficiencyOr);
			hSt->SetBinContent(i,EfficiencySt);


		}

		TCanvas *c5 = new TCanvas("c5","c5",600,600);
		c5->cd();
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/ShortEfficiency%s.png",colsyst.Data(),label.Data()));
		hShort->Draw();
		c5->SaveAs(Form("plots%s/efficiency/LongEfficiency%s.png",colsyst.Data(),label.Data()));
		hOr->Draw();
		c5->SaveAs(Form("plots%s/efficiency/OrEfficiency%s.png",colsyst.Data(),label.Data()));
		TLegend * l10; 
		if(doPbPb == 2)	 l10 = new TLegend(legl,legb,legr,legt);
		if(doPbPb == 1)	 l10 = new TLegend(0.15,0.15,0.65,0.55);
		if(doPbPb == 0)	 l10 = new TLegend(legl,legb,legr,legt);

		l10->SetTextFont(42);

		hShort->SetMarkerColor(kRed);
		hLong->SetMarkerColor(kBlue);
		hOr->SetMarkerColor(kGreen);
		hSt->SetMarkerColor(kBlack);

		hShort->SetMarkerStyle(23);
		hLong->SetMarkerStyle(23);
		hOr->SetMarkerStyle(23);
		hSt->SetMarkerStyle(23);

		hShort->SetMarkerSize(1);
		hLong->SetMarkerSize(1);
		hOr->SetMarkerSize(1);
		hSt->SetMarkerSize(1);
		hShort->GetXaxis()->SetTitle(Form("ADC %s Threshold",label.Data()));
		hShort->SetTitle(Form("Trigger Efficiencies for ADC %s Threshold",label.Data()));

		hShort->Draw("ep");
		hLong->Draw("epSAME");
		hOr->Draw("epSAME");
		hSt->Draw("epSAME");

		l10->AddEntry(hShort,"Short ADC Efficiency","pl");
		l10->AddEntry(hLong,"Long ADC Efficiency","pl");
		l10->AddEntry(hOr,"Short OR Long ADC Efficiency","pl");
		l10->AddEntry(hSt,"Standard Trigger","pl");

		l10->Draw("SAME");

		c5->SaveAs(Form("plots%s/efficiency/AllEfficiency%s.png",colsyst.Data(),label.Data()));

		hShort->Scale(25);
		hLong->Scale(25);
		hOr->Scale(25);
		hSt->Scale(25);



	}
*/
}

