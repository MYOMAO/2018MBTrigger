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


void PlotRAWADC()
{

	TFile * fin = new TFile("L1NtupleEMBXAll.root");
	TTree * t = (TTree *) fin->Get("ADC");

	TH1D * ADCHis = new TH1D("ADCHis","ADCHis",100,0,100);

	t->Project("ADCHis","ADCBoth");
	ADCHis->SetMinimum(1);
	ADCHis->GetXaxis()->SetTitle("ADC");
	ADCHis->GetYaxis()->SetTitle("Counts");
	ADCHis->SetTitle("ADC Distribution Empty Bunches");

	TCanvas *c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	CentHis->Draw("hist e");
	c1->SetLogy();
	c1->SaveAs("ADCplots/ADCDistribution.png");

	TFile *fout = new TFile("ADCEMBXDis.root","RECREATE");
	fout->cd();
	ADCHis->Write();
	fout->Close();

}

