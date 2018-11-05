#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TCut.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>

const int nBin = 35;
Float_t bins[nBin+1]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,45,50,55,60,70,80};


// Take a tree, a variable and calculate the efficiency
TGraphAsymmErrors* getEfficiency(TTree *t, char *variable, TCut weights, TCut preselection, TCut cut, int nBin, Float_t *bins)
{
	static int count = 0;
	count++;
	TH1D *hPass = new TH1D (Form("hPass%d",count),"",nBin,bins);
	TH1D *hAll = new TH1D (Form("hAll%d",count),"",nBin,bins);
	t->Draw(Form("%s>>hAll%d",variable,count),weights*preselection);
	t->Draw(Form("%s>>hPass%d",variable,count),weights*(preselection&&cut));
	cout << "hPass = " << hPass->Integral() << endl;
	cout << "hAll = " << hAll->Integral() << endl;
	hAll->Draw();
	cout<<"ici"<<endl;

	TGraphAsymmErrors *g = new TGraphAsymmErrors;
	g->BayesDivide(hPass,hAll);
	return g;
}

TGraphAsymmErrors* getEfficiencySum(TTree *t,TTree *t2, char *variable, TCut preselection,TCut preselection2, TCut cut, int nBin, Float_t *bins)
{
	static int count = 0;
	count++;
	TH1D *hPass = new TH1D (Form("hPass%d",count),"",nBin,bins);
	TH1D *hAll = new TH1D (Form("hAll%d",count),"",nBin,bins);
	t->Draw(Form("%s>>hAll%d",variable,count),preselection);
	t->Draw(Form("%s>>hPass%d",variable,count),preselection&&cut);


	TH1D *hPass2 = new TH1D (Form("hPass2%d",count),"",nBin,bins);
	TH1D *hAll2 = new TH1D (Form("hAll2%d",count),"",nBin,bins);
	t2->Draw(Form("%s>>hAll2%d",variable,count),preselection2);
	t2->Draw(Form("%s>>hPass2%d",variable,count),preselection2&&cut);

	TH1D *hPassTot = (TH1D*)hPass->Clone("hPassTot");
	TH1D *hAllTot = (TH1D*)hAll->Clone("hAllTot");

	hPassTot->Add(hPass2);
	hAllTot->Add(hAll2);

	TFile*f=new TFile(Form("test%d.root",count),"recreate");
	f->cd();
	hPass->Write();
	hAll->Write();
	hPass2->Write();
	hAll2->Write();
	hAllTot->Write();
	hPassTot->Write();
	f->Close();

	TGraphAsymmErrors *g = new TGraphAsymmErrors;
	g->BayesDivide(hPassTot,hAllTot);
	return g;
}


void SingleTrackL1Efficiency()
{
	gStyle->SetOptStat(0);

	bool sideband = false;
	bool cent = false;   
	TString outf,infname;
	//infname="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/Trigger/openHLTAll.root";
	//infname="/export/d00/scratch/jwang/HLTppRef/crab_HLT_V14_pp_20171019_prompt_D0_Pthat100_5020GeV_pythia8_20171005_fixGT.root";
	//infname="/export/d00/scratch/jwang/HLTppRef/crab_HLT_V14_pp_20171023_prompt_D0_5020GeV_pythia8_20171005_pthatweight.root";
	infname="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/HFTrigger/Forest/HiForestAOD_D0_Pthat10.root";
	outf = "result";

	cout << " Pass 1" << endl; 

	// ============== Open file and basic settings ===============   
	// Open Dntuple file

	// L1 trigger thresholds
	TCut l1CutMBSeed1 = "L1_SingleJet24_BptxAND";
	TCut l1CutMBSeed2 = "L1_SingleJet28_BptxAND";
	TCut l1CutMBSeed3 = "L1_SingleJet32_BptxAND";
	TCut l1CutMBSeed4 = "L1_SingleJet40_BptxAND";
	TCut l1CutMBSeed5 = "L1_SingleJet44_BptxAND";
	TCut l1CutMBSeed6 = "L1_SingleJet50_BptxAND";
	TCut l1CutMBSeed7 = "L1_SingleJet56_BptxAND";
	TCut l1CutMBSeed8 = "L1_SingleJet60_BptxAND";


	TFile *inf = new TFile(infname);
	TTree *ntDkpi = (TTree*)inf->Get("Dfinder/ntDkpi");
	TTree *ntHlt = (TTree*)inf->Get("hltanalysis/HltTree");
	TTree *ntHi = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");

	ntDkpi->AddFriend(ntHlt);
	ntDkpi->AddFriend(ntHi);
	//  ntHlt->AddFriend(ntHi);
	
	TFile *infMB = new TFile(infname);
	TTree* ntDkpiMB = (TTree*)infMB->Get("Dfinder/ntDkpi");
	TTree* ntHltMB= (TTree*)infMB->Get("hltbitanalysis/HltTree");
	TTree* ntHiMB= (TTree*)infMB->Get("hiEvtAnalyzer/HiTree");

	cout<<"step1"<<endl;
	ntDkpiMB->SetName("ntDkpiMB");
	ntHltMB->SetName("ntHltMB");
	ntDkpiMB->AddFriend(ntHltMB);
	ntDkpiMB->AddFriend(ntHltMB);
	ntDkpiMB->AddFriend(ntHiMB);

	// Define bin size and bin width for trigger turnon curve histograms

	// Templates for plotting  
	TH1D *hTmp = new TH1D ("hTmp","",nBin,bins);
	TH1D *hTmp2 = new TH1D ("hTmp2","",nBin,bins);

	// D meson selection
	TCut DmassCut             = "(abs(Dmass-1.8696)<0.1)";
	TCut DmesonCut            = "(DsvpvDistance/DsvpvDisErr)>2.5&&Dchi2cl>0.10&&Dalpha<0.12";
	TCut DmesonDaughterTrkCut="Dtrk1highPurity&&Dtrk1Pt>1&&abs(Dtrk1Eta)<2.0&&Dtrk2highPurity&&Dtrk2Pt>1&&abs(Dtrk2Eta)<2.0";
	if (sideband) DmassCut = "(abs(Dmass-1.8696)>0.06 && abs(Dmass-1.8696)>0.12)";

	// Final selection for D candidates for trigger turnon studies
	// TCut DAnaCut = DmassCut && DmesonCut && DmesonDaughterTrkCut;
	//HLT Tresholds//
	TCut DAnaCut = "1";

	TString weight= "pthatweight"; 
//	TCut HLTCut5 = "HLT_PADmesonPPTrackingGlobal_Dpt5_v1";
//	TCut HLTCut8 = "HLT_DmesonPPTrackingGlobal_Dpt8_v1";
//	TCut HLTCut15 = "HLT_DmesonPPTrackingGlobal_Dpt15_v1";
	TCut HLTCut20 = "HLT_cPPTrackingGlobal_Dpt20_v1";
	TCut HLTCut30 = "HLT_cPPTrackingGlobal_Dpt30_v1";
	TCut HLTCut40 = "HLT_cPPTrackingGlobal_Dpt40_v1";
	TCut HLTCut50 = "HLT_cPPTrackingGlobal_Dpt50_v1";
	TCut HLTCut60 = "HLT_cPPTrackingGlobal_Dpt60_v1";

//	TCut HLTCut50 = "HLT_PADmesonPPTrackingGlobal_Dpt50_v1";
	//TCut HLTCut8 = "1";
	//TCut HLTCut15 = "1";
	//TCut HLTCut30 = "1";
	// ============== L1 trigger efficiency study ===============
	cout<<"step3"<<endl;

	TCanvas *c = new TCanvas("c","",600,600);

	//   TGraphAsymmErrors* g5;
//	TGraphAsymmErrors* g8;
//	TGraphAsymmErrors* g15;
//	TGraphAsymmErrors* g30;
	//   TGraphAsymmErrors* g50;

	TGraphAsymmErrors* g20;
	TGraphAsymmErrors* g30;
	TGraphAsymmErrors* g40;
	TGraphAsymmErrors* g50;
	TGraphAsymmErrors* g60;


	//  g5  = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(DAnaCut&&l1CutMBSeed1), HLTCut5, nBin, bins);

	//   g8  = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(DAnaCut&&l1CutMBSeed1), HLTCut8, nBin, bins);
	//   g15 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(DAnaCut&&l1CutMBSeed2), HLTCut15, nBin, bins);
	//   g30 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(DAnaCut&&l1CutMBSeed3), HLTCut30, nBin, bins);
	// g50  = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(DAnaCut&&l1CutMBSeed1), HLTCut50, nBin, bins);
/*
	g8  = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()),TCut(weight),TCut(DAnaCut), l1CutMBSeed1, nBin, bins);
	g15 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()),TCut(weight),TCut(DAnaCut) , l1CutMBSeed2, nBin, bins);
	g30 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed3, nBin, bins);
	g12 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed4, nBin, bins);
	g82 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed5, nBin, bins);
	g4 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed6, nBin, bins);
*/
	g20  = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()),TCut(weight),TCut(DAnaCut), l1CutMBSeed1, nBin, bins);
	g30 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()),TCut(weight),TCut(DAnaCut) , l1CutMBSeed2, nBin, bins);
	g40 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed3, nBin, bins);
	g50 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed4, nBin, bins);
	g60 = getEfficiency(ntDkpiMB,Form("Max$(Dpt*(%s))",DAnaCut.GetTitle()), TCut(weight),TCut(DAnaCut), l1CutMBSeed5, nBin, bins);


	cout << " Pass 2" << endl; 

	hTmp->Draw();
	hTmp->SetXTitle("D Meson p_{T} (GeV/c)");
	hTmp->SetYTitle("D Meson L1 Efficiency");

	//  hTmp->SetYTitle("D Meson HLT Efficiency");
	/*   
		 g5->SetLineColor(1);
		 g5->SetMarkerColor(1);
		 g5->Draw("pl same");
		 */
	g8->SetLineColor(4);
	g8->SetMarkerColor(4);
	g8->Draw("pl same");

	g15->SetLineColor(2);
	g15->SetMarkerColor(2);
	g15->Draw("pl same");

	g30->SetLineColor(kGreen+2);
	g30->SetMarkerColor(kGreen+2);
	g30->Draw("pl same");

	g12->SetLineColor(6);
	g12->SetMarkerColor(6);
	g12->Draw("pl same");

	g82->SetLineColor(kYellow+2);
	g82->SetMarkerColor(kYellow+2);
	g82->Draw("pl same");

	g4->SetLineColor(1);
	g4->SetMarkerColor(1);
	g4->Draw("pl same");


	/*
	   g50->SetLineColor(6);
	   g50->SetMarkerColor(6);
	   g50->Draw("pl same");
	   */
	TLegend *leg = new TLegend(0.53,0.2,0.88,0.6);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	//   leg->AddEntry(g5,"HLT D meson 5","pl");
	/*
	   leg->AddEntry(g8,"HLT D meson 8","pl");
	   leg->AddEntry(g15,"HLT D meson 15","pl");
	   leg->AddEntry(g30,"HLT D meson 30","pl");
	   */
	/*
	leg->AddEntry(g8,"L1_SingleJet16","pl");
	leg->AddEntry(g15,"L1_SingleJet24","pl");
	leg->AddEntry(g30,"L1_SingleJet40","pl");
	leg->AddEntry(g12,"L1_SingleJet12","pl");
	leg->AddEntry(g82,"L1_SingleJet8","pl");
	leg->AddEntry(g4,"L1_SingleJet4","pl");
	*/
	leg->AddEntry(g20,"L1_SingleJet16","pl");
	leg->AddEntry(g30,"L1_SingleJet24","pl");
	leg->AddEntry(g40,"L1_SingleJet40","pl");
	leg->AddEntry(g50,"L1_SingleJet12","pl");
	leg->AddEntry(g60,"L1_SingleJet8","pl");


	// leg->AddEntry(g50,"HLT D meson 50","pl");
	leg->Draw();
	cout<<"step4"<<endl;

	c->SaveAs(outf+"/Dmeson-HIHLTriggerEfficiency.pdf");
	c->SaveAs(outf+"/Dmeson-HIHLTriggerEfficiency.png");
	//  c->SaveAs(outf+"/Dmeson-HIHLTriggerEfficiency.C");
	TFile *fouput=new TFile(outf+"/fefficiency.root","recreate");
	fouput->cd();
	//   g5->SetName("g5");
	g8->SetName("g8");
	g15->SetName("g15");
	g30->SetName("g30");
	g12->SetName("g12");
	g82->SetName("g82");
	g4->SetName("g4");

	//  g50->SetName("g50");
	//   g5->Write();
	g8->Write();
	g15->Write();
	g30->Write();
	g12->Write();
	g82->Write();
	g4->Write();
	//   g50->Write();
	fouput->Close();
}


void plotturnon(){


	const int nfiles=3;

	TFile *fouput=new TFile("result/fefficiency.root");
	TGraphAsymmErrors *g[nfiles];
	//   g[0]=(TGraphAsymmErrors*)fouput->Get("g5");
	g[0]=(TGraphAsymmErrors*)fouput->Get("g8");

	g[1]=(TGraphAsymmErrors*)fouput->Get("g15");
	g[2]=(TGraphAsymmErrors*)fouput->Get("g30");

	TLegendEntry *entry[nfiles];
	TString label[nfiles] = {"D^{0} trigger p_{T}> 20", "D^{0} trigger p_{T}> 40", "D^{0} trigger p_{T}> 60"};
	const int nBin = 17;
	int colors[nfiles]={1,2,4};

	for (int m=0;m<nBin;m++){
		Double_t x,y;
		g[0]->GetPoint(m,x,y);  
		if (x>40){
			g[0]->SetPoint(m,0.,0.);
			g[0]->SetPointEYlow(m,0);
			g[0]->SetPointEYhigh(m,0);
		}
	}

	int whichthreshold[nBin]={0,0,0,0,0,0,1,1,1,2,2,2,2};

	double yvalue[nBin];
	double yvaluehigh[nBin];
	double yvaluelow[nBin];
	double xvalue[nBin];
	double xvaluehigh[nBin];
	double xvaluelow[nBin];


	for (int m=0;m<nBin;m++){
		int mybin=whichthreshold[m];

		Double_t x,y;
		g[mybin]->GetPoint(m,x,y);  

		yvalue[m]=y;
		yvaluehigh[m]=g[mybin]->GetErrorYhigh(m);
		yvaluelow[m]=g[mybin]->GetErrorYlow(m);

		xvalue[m]=x;
		xvaluehigh[m]=g[mybin]->GetErrorXhigh(m);
		xvaluelow[m]=g[mybin]->GetErrorXlow(m);

		cout<<xvalue[m]<<","<<xvaluelow[m]<<","<<xvaluehigh[m]<<","<<yvalue[m]<<","<<yvaluelow[m]<<","<<yvaluehigh[m]<<endl;

	}

	TGraphAsymmErrors* gaeTrigger = new TGraphAsymmErrors(nBin,xvalue,yvalue,xvaluelow,xvaluehigh,yvaluelow,yvaluehigh);
	gaeTrigger->SetName("gaeTrigger");
	gaeTrigger->SetMarkerStyle(20);
	gaeTrigger->SetMarkerSize(0.8);

	TH2F* hemptyRatio=new TH2F("hemptyRatio","",50,10,100.,10.,0.0,1.5);
	hemptyRatio->GetXaxis()->CenterTitle();
	hemptyRatio->GetYaxis()->CenterTitle();
	hemptyRatio->GetYaxis()->SetTitle("HLT D meson trigger efficiency");
	hemptyRatio->GetXaxis()->SetTitle("D^{0} p_{T} (GeV/c)");
	hemptyRatio->GetXaxis()->SetTitleOffset(1.20);
	hemptyRatio->GetYaxis()->SetTitleOffset(1.14);//1.3
	hemptyRatio->GetXaxis()->SetTitleSize(0.055);
	hemptyRatio->GetYaxis()->SetTitleSize(0.055);
	hemptyRatio->GetXaxis()->SetTitleFont(42);
	hemptyRatio->GetYaxis()->SetTitleFont(42);
	hemptyRatio->GetXaxis()->SetLabelFont(42);
	hemptyRatio->GetYaxis()->SetLabelFont(42);
	hemptyRatio->GetXaxis()->SetLabelSize(0.045);
	hemptyRatio->GetYaxis()->SetLabelSize(0.045);  
	hemptyRatio->Draw();

	//TLegend *legendSigma=new TLegend(0.4714765,0.3496503,0.8003356,0.5244755,"");//0.5100806,0.5868644,0.8084677,0.7605932
	TLegend *legendSigma=new TLegend(0.50,0.30,0.80,0.52,"");//0.5100806,0.5868644,0.8084677,0.7605932
	legendSigma->SetBorderSize(0);
	legendSigma->SetLineColor(0);
	legendSigma->SetFillColor(0);
	legendSigma->SetFillStyle(1001);
	legendSigma->SetTextFont(42);
	//legendSigma->SetTextSize(0.045);
	legendSigma->SetTextSize(0.060);

	TCanvas *cFit = new TCanvas("c","",1200,600);
	hemptyRatio->Draw();
	gPad->SetLogx();
	TF1 *f= new TF1("f","[0]+[1]*x",20,100);
	gaeTrigger->Fit(f,"","",20,100);
	gStyle->SetOptFit(0);

	TF1 *fup= new TF1("f","[0]+[1]*x",20,100);
	TF1 *flow= new TF1("f","[0]+[1]*x",20,100);
	fup->SetParameter(1,f->GetParameter(1));
	flow->SetParameter(1,f->GetParameter(1));
	fup->SetParameter(0,f->GetParameter(0)+f->GetParError(0));
	flow->SetParameter(0,f->GetParameter(0)-f->GetParError(0));
	fup->Draw("same");
	flow->Draw("same");
	gaeTrigger->SetLineColor(1);
	gaeTrigger->SetMarkerColor(1);
	gaeTrigger->SetLineWidth(2);
	gaeTrigger->SetFillStyle(0); 
	gaeTrigger->Draw("epsame"); 
	cFit->SaveAs("result_PbPbHPPlusMB/Dmeson-HLTfinaltriggerefficiency.pdf");
	cFit->SaveAs("result_PbPbHPPlusMB/Dmeson-HLTfinaltriggerefficiency.png");


	TCanvas *c = new TCanvas("c","",600,600);
	hemptyRatio->Draw();

	cout << "Pass 3" << endl;

	for (int ifile=0;ifile<nfiles;ifile++){
		g[ifile]->SetLineColor(colors[ifile]);
		g[ifile]->SetMarkerColor(colors[ifile]);
		g[ifile]->SetLineWidth(2);
		g[ifile]->SetFillStyle(0); 
		g[ifile]->Draw("epsame");  
		entry[ifile]=legendSigma->AddEntry(g[ifile],label[ifile].Data(),"f");
		entry[ifile]->SetTextFont(42);
		entry[ifile]->SetTextSize(0.045);
		entry[ifile]->SetLineColor(colors[ifile]);
		entry[ifile]->SetMarkerColor(colors[ifile]);

	}

	cout << "Pass 4" << endl;

	legendSigma->Draw("same");
	TLatex* texlumi = new TLatex(0.42,0.865,"404 #mub^{-1} (5.02 TeV PbPb)");
	texlumi->SetNDC();
	texlumi->SetTextFont(42);
	texlumi->SetTextSize(0.044);
	texlumi->SetLineWidth(2);
	texlumi->Draw();
	TLatex* texcms = new TLatex(0.22,0.90,"CMS");
	texcms->SetNDC();
	texcms->SetTextAlign(13);
	texcms->SetTextFont(62);//61
	texcms->SetTextSize(0.06);
	texcms->SetLineWidth(2);
	texcms->Draw();
	TLatex* texpre = new TLatex(0.22,0.84,"Performance");
	texpre->SetNDC();
	texpre->SetTextAlign(13);
	texpre->SetTextFont(52);
	texpre->SetTextSize(0.04);
	texpre->SetLineWidth(2);
	texpre->Draw();
	c->SaveAs("HLTDmesonEfficiency.pdf");


}

