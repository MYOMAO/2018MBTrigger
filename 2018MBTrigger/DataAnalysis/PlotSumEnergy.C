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
#include "TBranchElement.h"
#include "TApplication.h"
#include "TInterpreter.h"
#include "TSystem.h"

//cpp dependencies
#include <iostream>
#include <fstream>
#include <map>

//local dependencies
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

void PlotSumEnergy(){


	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
	TString colsyst;
	if(doPbPb == 2 ) colsyst="XeXe";
	if(doPbPb == 1 ) colsyst="PbPb";
	if(doPbPb == 0 ) colsyst="pp";


	int dorecent = 2;
	int doZB = 0;
	int doEMBX = 1;
	int donew = 1;
	int nTowers;
	float hiHFEnergy[10000];
	int HFieta[10000];
	int ieta[10000];
	short ADCnTower;
	int iet;

	double scale = 1.5;
	TFile * fin1;
	TFile * fin2;

	if(donew == 1){
		fin1 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/L1Ntuple_XeXe_EMBX_All_WithET.root", "READ");
		fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/finder_XeXe_EMBX_All_AllE.root", "READ");
	}

	if(dorecent == 1){
		fin1 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/LPUPPEMBX/L1NtupleEMBXLPUppSmaller.root", "READ");
		fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/finder_XeXe_EMBX_All_AllE.root", "READ");
	}
	if(dorecent == 2){
		fin1 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/PbPbMCnew/L1NtuplePbPbMCNew.root", "READ");
		fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/MBTrigger/XeXeEMBXnew/finder_XeXe_EMBX_All_AllE.root", "READ");
	}

	TString systag;

	if(dorecent == 1) systag = "LPUPP Jul 2018";
	if(dorecent == 0) systag = "XeXe 2017";
	if(dorecent == 2) systag = "PbPb MB MC";


	fin1->cd();
	TTree * t1 = (TTree*) fin1->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
	L1Analysis::L1AnalysisL1CaloTowerDataFormat *towers_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();

	t1->SetBranchStatus("*", 1);
	t1->SetBranchAddress("L1CaloTower", &towers_);
	t1->SetBranchAddress("nTower", &ADCnTower);

	fin2->cd();
	TTree * t2 = (TTree*) fin2->Get("hiEvtAnalyzer/HiTree");

	t2->SetBranchStatus("*", 0);
	t2->SetBranchStatus("nTowers", 1);
	t2->SetBranchStatus("hiHFEnergy", 1);
	t2->SetBranchStatus("ieta", 1);
	t2->SetBranchAddress("nTowers", &nTowers);
	t2->SetBranchAddress("hiHFEnergy", hiHFEnergy);
	t2->SetBranchAddress("ieta", HFieta);	
	//	const int NEvents1 = 5;
	const int NEvents1 = t1->GetEntries();
	const int NEvents2 = t2->GetEntries();
	const int NCut = 2;
	int ietaCut[NCut] = {25, 28};

	int iETSum[NCut];
	float EnergySum[NCut];

	TH1D * iETSumHis[NCut]; 
	TH1D * EnergySumHis[NCut]; 



	int step = 1;
	int ietmin = 0;
	int ietmax;
	if(dorecent == 1)
	{
		ietmax = 200;

	}

	if(dorecent == 2)
	{
		ietmax = 400;

	}


	int Niet = (ietmax - ietmin)/step;

	for(int i = 0; i < NCut; i++){
		iETSumHis[i] = new TH1D(Form("iETSumHis%d",i),Form("iETSumHis%d",i),Niet,ietmin,ietmax);
		iETSumHis[i]->GetXaxis()->SetTitle("iet Sum");
		iETSumHis[i]->GetYaxis()->SetTitle("Counts");
		iETSumHis[i]->SetTitle(Form("iet Sum for |ieta| < %d",ietaCut[i]));

		EnergySumHis[i] = new TH1D(Form("EnergySumHis%d",i),Form("EnergySumHis%d",i),100,0,40);
		EnergySumHis[i]->GetXaxis()->SetTitle("Offline Energy Sum (GeV)");
		EnergySumHis[i]->GetYaxis()->SetTitle("Counts");
		EnergySumHis[i]->SetTitle(Form("Offline Energy Sum for |ieta| < %d",ietaCut[i]));
	}


	for(int j = 0; j <NEvents1; j++){
		if(j%10000==0) cout << "Now Working on Event =  " << j << endl;
		t1->GetEntry(j);
		for(int k = 0; k < NCut; k++){
			iETSum[k] = 0;
		}

		int m = 0;
		for (std::vector<short>::const_iterator i = towers_->ieta.begin(); i != towers_->ieta.end(); i++)
		{
			ieta[m] = int(*i);
			m = m + 1;
		}

		int n = 0;
		for (std::vector<short>::const_iterator i = towers_->iet.begin(); i != towers_->iet.end(); i++)
		{
			iet = int(*i);


			for(int k = 0; k< NCut;k++){
				if(abs(ieta[n]) < ietaCut[k])
				{
					iETSum[k] = iETSum[k] + iet;
					//		cout << "ieta = " << ieta[n] << "  iet = " << iet << endl;
				}

			}
			n = n + 1;
		}
		
		if(m != n) cout << "m = " << m << " n = " << n << endl;

		for(int k = 0; k< NCut;k++){
			iETSumHis[k]->Fill(iETSum[k]);
		}
	}



	for(int j = 0; j <NEvents2; j++){
		if(j%10000==0) cout << "Now Working on Event =  " << j << endl;
		t2->GetEntry(j);
		for(int k = 0; k < NCut; k++){
			EnergySum[k] = 0.0;
		}

		for(int i = 0; i < nTowers; i++){

			for(int k = 0; k< NCut;k++){
				if(abs(HFieta[i]) < ietaCut[k])  EnergySum[k] = EnergySum[k] + hiHFEnergy[i];

			}

		}

		for(int k = 0; k< NCut;k++){
			EnergySumHis[k]->Fill(EnergySum[k]);

		}
	}

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	for(int k = 0; k< NCut;k++){
		int binmax1 = iETSumHis[k]->GetMaximumBin();
		double maxhis1 = 	iETSumHis[k]->GetBinContent(binmax1);
		double ymax1 = maxhis1 * scale;
		iETSumHis[k]->SetMaximum(ymax1);
		iETSumHis[k]->Draw();


		TLatex* tex = new TLatex(0.17,0.78,Form("%s EMBX Total Events = %d",systag.Data(),NEvents1));
		tex->SetNDC();
		tex->SetTextFont(42);
		tex->SetTextSize(0.04);
		tex->SetLineWidth(2);
		tex->Draw("same");
		/*
		TLatex* tex2 = new TLatex(0.41,0.70,Form("N Entries on Plot = %d",NEvents1));
		tex2->SetNDC();
		tex2->SetTextFont(42);
		tex2->SetTextSize(0.04);
		tex2->SetLineWidth(2);
		tex2->Draw("same");
		*/
		c->SaveAs(Form("plots%s/EnergySum/iETSumHisLT%d.png",colsyst.Data(),ietaCut[k]));

		int binmax2 = EnergySumHis[k]->GetMaximumBin();
		double maxhis2 = EnergySumHis[k]->GetBinContent(binmax2);
		double ymax2 = maxhis2 * scale;
		EnergySumHis[k]->SetMaximum(ymax2);
		EnergySumHis[k]->Draw();


		TLatex* tex3 = new TLatex(0.17,0.68,Form("%s EMBX Total Events = %d",systag.Data(),NEvents2));
		tex3->SetNDC();
		tex3->SetTextFont(42);
		tex3->SetTextSize(0.04);
		tex3->SetLineWidth(2);
		tex3->Draw("same");
		/*
		TLatex* tex4 = new TLatex(0.41,0.60,Form("N Entries on Plot = %d",NEvents2));
		tex4->SetNDC();
		tex4->SetTextFont(42);
		tex4->SetTextSize(0.04);
		tex4->SetLineWidth(2);
		tex4->Draw("same");
		*/
		c->SaveAs(Form("plots%s/EnergySum/EnergySumHis%d.png",colsyst.Data(),ietaCut[k]));

		cout << "ymax1 = " << ymax1 << "  ymax2 = " << ymax2 << endl;
	}


	TFile * fout = new TFile("EnergySum.root","RECREATE");
	fout->cd();

	for(int k = 0; k< NCut;k++){
		iETSumHis[k]->Write();
		EnergySumHis[k]->Write();
	}

	fout->Close();




}
