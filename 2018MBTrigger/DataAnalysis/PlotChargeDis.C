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



void PlotChargeDis()
{
	gStyle->SetOptStat(0); 
	int PedSub = 0;
	int useMatched = 1;
	int ietaminL = -41;
	int ietamaxL = -29;
	int ietaminR = 30;
	int ietamaxR = 42;
	int iphimin = 1;
	int iphimax = 73;
	double Scaling = 1;

	double xmin = 0;
	double xmax = 100;
	double ymin = -1.5;
	double ymax = 1.5;


	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";


	int NPlots2 = (ietamaxL-ietaminL + ietamaxR-ietaminR)*(iphimax-iphimin);
	const int NPlots =NPlots2;

	const int NPlotBinsX = 25;
	const int NPlotBinsY = 40;

	TH2D * ChargePlotsShort[NPlots];
	TString CPNameShort;
	TH2D * ChargePlotsLong[NPlots];
	TString CPNameLong;

	for( int i = 0; i< NPlots; i++){
		CPNameShort = Form("CPS%d",i);
		CPNameLong = Form("CPL%d",i);
		ChargePlotsShort[i] = new TH2D(CPNameShort.Data(),CPNameShort.Data(),NPlotBinsX,xmin,xmax,NPlotBinsY,ymin,ymax);
		ChargePlotsLong[i] = new TH2D(CPNameLong.Data(),CPNameLong.Data(),NPlotBinsX,xmin,xmax,NPlotBinsY,ymin,ymax);
		ChargePlotsShort[i]->GetXaxis()->SetTitle("Q1+Q2 [fC]");
		ChargePlotsShort[i]->GetYaxis()->SetTitle("(Q1-Q2)/(Q1+Q2)");
		ChargePlotsLong[i]->GetXaxis()->SetTitle("Q1+Q2 [fC]");
		ChargePlotsLong[i]->GetYaxis()->SetTitle("(Q1-Q2)/(Q1+Q2)");


	}

	double LongQ1;
	double LongQ2;
	double ShortQ1;
	double ShortQ2;

	double XShort;
	double YShort;
	double XLong;
	double YLong;

	int ADCBothMatched[10000];
	int ADCBackMatched[10000];
	int ADCFrontMatched[10000];

	int nTowersMatched;
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

	TString infile;
	TTree * t;

	if(useMatched == 0 ){
		if(doPbPb == 2) 
		{
			infile ="EventMatchedXeXe.root";

		}
		if(doPbPb == 1) infile ="L1NtuplePbPbMCNew2.root";
		if(doPbPb == 0) infile ="EventMatchedpp.root";

		TFile * fin = new TFile(infile.Data());

		t = (TTree*) fin->Get("ADC");
		//t->SetBranchStatus("*", 1);
		t->SetBranchAddress("ADCSize",&ADCSizeMatched);
		t->SetBranchAddress("charge",ADCMatchedcharge);
		t->SetBranchAddress("chargeped",ADCMatchedchargeped);

	}


	if(useMatched ==1 ){
		if(doPbPb == 2) 
		{
			infile ="EventMatchedXeXe.root";

		}
		if(doPbPb == 1) infile ="EventMatchedPbPb.root";
		if(doPbPb == 0) infile ="EventMatchedpp.root";

		TFile * fin = new TFile(infile.Data());


		t = (TTree*) fin->Get("EVTMatchTree");
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
	}






	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();

	TFile * fout = new TFile("ChargeDis.root","RECREATE"); 
	int NEvents = t->GetEntries();
	//NEvents = 1;
	for(int i = 0; i < NEvents; i++){
		if(i%1000==0) 	cout << "Now Working on Event = " << i << endl;
		int index = 0;
		t->GetEntry(i);
		for(int j = iphimin; j < iphimax; j++){
			for(int k = ietaminL; k<ietamaxL;k++)
			{		
				XShort = -999;
				YShort = -999;
				XLong = -999;
				YLong = -999;
				//cout << "Before:  " << "index = " << index << "   iphi =  " << j << "   ieta = " << k << endl;

				for(int l = 0; l < ADCSizeMatched; l++)
				{
					if(ADCMatchedieta[l] == k && ADCMatchediphi[l]  == j){
						if(PedSub == 1){
							if(ADCMatcheddepth[l] == 1) ShortQ1 = ADCMatchedcharge[l];
							if(ADCMatcheddepth[l] == 2) ShortQ2 = ADCMatchedcharge[l];
							if(ADCMatcheddepth[l] == 3) LongQ1 = ADCMatchedcharge[l];
							if(ADCMatcheddepth[l] == 4) LongQ2 = ADCMatchedcharge[l];
						}

						if(PedSub == 0){
							if(ADCMatcheddepth[l] == 1) ShortQ1 = ADCMatchedchargeped[l];
							if(ADCMatcheddepth[l] == 2) ShortQ2 = ADCMatchedchargeped[l];
							if(ADCMatcheddepth[l] == 3) LongQ1 = ADCMatchedchargeped[l];
							if(ADCMatcheddepth[l] == 4) LongQ2 = ADCMatchedchargeped[l];
						}


						XShort = (ShortQ1 + ShortQ2)*Scaling;
						YShort = (ShortQ1 - ShortQ2)/(ShortQ1 + ShortQ2);
						XLong = (LongQ1 + LongQ2)*Scaling;
						YLong = (LongQ1 - LongQ2)/(LongQ1 + LongQ2);
					}

				}

				if(XShort > -1) ChargePlotsShort[index]->Fill(XShort,YShort);
				if(YShort > -1) 	ChargePlotsLong[index]->Fill(XLong,YLong);
				index = index + 1;

			}

			for(int k = ietaminR; k<ietamaxR;k++)
			{		
				XShort = -999;
				YShort = -999;
				XLong = -999;
				YLong = -999;
				for(int l = 0; l < ADCSizeMatched; l++)
				{
					if(ADCMatchedieta[l] == j && ADCMatchediphi[l]  == k){
						if(ADCMatcheddepth[l] == 1) ShortQ1 = ADCMatchedcharge[l];
						if(ADCMatcheddepth[l] == 2) ShortQ2 = ADCMatchedcharge[l];
						if(ADCMatcheddepth[l] == 3) LongQ1 = ADCMatchedcharge[l];
						if(ADCMatcheddepth[l] == 4) LongQ2 = ADCMatchedcharge[l];

						XShort = ShortQ1 + ShortQ2;
						YShort = (ShortQ1 - ShortQ2)/(ShortQ1 + ShortQ2);
						XLong = LongQ1 + LongQ2;
						YLong = (LongQ1 - LongQ2)/(LongQ1 + LongQ2);
					}

				}

				if(XShort > -1) ChargePlotsShort[index]->Fill(XShort,YShort);
				if(YShort > -1) 	ChargePlotsLong[index]->Fill(XLong,YLong);
				index = index + 1;
			}

		}


	}

	int	index = 0;
	TLatex * tex;
	TFile *fout = new TFile("ChargePlot%s.root",colsyst.Data());

	for( int i = iphimin; i< iphimax; i++){

		for(int j = ietaminL; j< ietamaxL; j++){
			//		cout << "After:  " << "index = " << index << "   iphi =  " << i << "   ieta = " << j << endl;

			ChargePlotsShort[index]->Draw("colz");

			tex = new TLatex(0.40,0.83,Form("ieta = %d, iphi = %d, Short Fibers",j,i));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.04);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			c->SetLogz();
			if(ChargePlotsShort[index]->Integral() > 0) c->SaveAs(Form("plots%s/Chargeplots/ShortPlot%d.png",colsyst.Data(),index));

			ChargePlotsLong[index]->Draw("colz");

			tex = new TLatex(0.40,0.83,Form("ieta = %d, iphi = %d, Long Fibers",j,i));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.04);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			c->SetLogz();
			if(	ChargePlotsLong[index]->Integral() > 0) c->SaveAs(Form("plots%s/Chargeplots/LongPlot%d.png",colsyst.Data(),index));
			index = index + 1;

		}

		for(int j = ietaminR;j < ietamaxR; j++){
			ChargePlotsShort[index]->Draw("colz");

			tex = new TLatex(0.40,0.83,Form("ieta = %d, iphi = %d, Short Fibers",j,i));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.04);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			c->SetLogz();
			if(ChargePlotsShort[index]->Integral() > 0) c->SaveAs(Form("plots%s/Chargeplots/ShortPlot%d.png",colsyst.Data(),index));

			ChargePlotsLong[index]->Draw("colz");

			tex = new TLatex(0.40,0.83,Form("ieta = %d, iphi = %d, Long Fibers",j,i));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.04);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			c->SetLogz();
			if(	ChargePlotsLong[index]->Integral() > 0) c->SaveAs(Form("plots%s/Chargeplots/LongPlot%d.png",colsyst.Data(),index));
			index = index + 1;
		}


	}




}
