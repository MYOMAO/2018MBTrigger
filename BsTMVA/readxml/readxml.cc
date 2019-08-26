#include "readxml.h"
#include "Math/SpecFunc.h"
#include "TSystem.h"
#include "Math/QuantFuncMathCore.h"

using namespace std;
void readxml(TString inputSname, TString inputBname, TString outputname,
		TString weightfile, TString collisionsyst,
		TString mycuts, TString mycutb, TString mycutg, TString weightfunctiongen, TString weightfunctionreco,
		TString mvatype, Float_t ptmin, Float_t ptmax, 
		Float_t lumi, Float_t raa=1.)
{
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	gStyle->SetMarkerStyle(20);
	gStyle->SetTextSize(0.05);
	gStyle->SetTextFont(42);
	gStyle->SetPadRightMargin(0.043);
	gStyle->SetPadLeftMargin(0.18);
	gStyle->SetPadTopMargin(0.1);
	gStyle->SetPadBottomMargin(0.145);
	gStyle->SetTitleX(.0f);
	Bool_t isPbPb = collisionsyst=="PbPb"?true:false;
	collisionsyst = "PbPb";
	std::ofstream ofresult(Form("%s_%s.txt",outputresult.Data(),outputname.Data()),std::ofstream::out);
	cfout cfresult(ofresult, std::cout);

	TString cuts = isPbPb?Form("(%s)&&Bpt>%f&&Bpt<%f&&hiBin>=0&&hiBin<=200",mycuts.Data(),ptmin,ptmax):Form("(%s)&&Bpt>%f&&Bpt<%f",mycuts.Data(),ptmin,ptmax);
	TString cutb = isPbPb?Form("(%s)&&Bpt>%f&&Bpt<%f&&hiBin>=0&&hiBin<=200",mycutb.Data(),ptmin,ptmax):Form("(%s)&&Bpt>%f&&Bpt<%f",mycutb.Data(),ptmin,ptmax);
	TString cutg = isPbPb?Form("(%s)&&Gpt>%f&&Gpt<%f&&hiBin>=0&&hiBin<=200",mycutg.Data(),ptmin,ptmax):Form("(%s)&&Gpt>%f&&Gpt<%f",mycutg.Data(),ptmin,ptmax);

	// read weight file
	const char* filename = weightfile;
	void *doc = TMVA::gTools().xmlengine().ParseFile(filename,TMVA::gTools().xmlenginebuffersize());
	void* rootnode = TMVA::gTools().xmlengine().DocGetRootElement(doc); //node "MethodSetup"
	TString fullMethodName("");
	TMVA::gTools().ReadAttr(rootnode, "Method", fullMethodName);

	cout << "pass  -1 "<< endl;
	cfresult<<std::endl;
	cfresult<<"  ===================================================="<<std::endl;
	cfresult<<" |                Cut Opt Configuration               |"<<std::endl;
	cfresult<<"  ----------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(10)<<"Method"<<" | "<<std::setw(37)<<fullMethodName<<" |"<<std::endl;

	cout << "pass  0 "<< endl;

	void* opts = TMVA::gTools().GetChild(rootnode,"Options");
	void* opt = TMVA::gTools().GetChild(opts,"Option");

	TString varProp("");
	while(opt)
	{
		TString optname("");
		TMVA::gTools().ReadAttr(opt, "name", optname);
		if(optname=="VarProp") varProp = TMVA::gTools().GetContent(opt);
		opt = TMVA::gTools().GetNextChild(opt);
	}

	void* variables = TMVA::gTools().GetChild(rootnode,"Variables");
	UInt_t nVar=0;
	std::vector<TString> varnames;
	TMVA::gTools().ReadAttr(variables, "NVar", nVar);
	void* var = TMVA::gTools().GetChild(variables,"Variable");
	for(unsigned int k=0;k<nVar;k++)
	{
		TString varname("");
		TMVA::gTools().ReadAttr(var, "Expression", varname);
		TString tem = Form("Variable%i",k);
		cfresult<<"  ----------------------------------------------------"<<std::endl;
		cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(10)<<tem<<" | "<<std::setw(37)<<varname<<" |"<<std::endl;
		var = TMVA::gTools().GetNextChild(var);
		varnames.push_back(varname);
	}
	cfresult<<"  ===================================================="<<std::endl;
	std::cout<<std::endl;

	TFile* inputS = new TFile(inputSname.Data());
	TFile* inputB = new TFile(inputBname.Data());

	TTree* background = (TTree*)inputB->Get("Bfinder/ntphi");

	//	background->AddFriend("ntHlt");
	//	background->AddFriend("ntHi");
	//	background->AddFriend("ntSkim");
	background->AddFriend(Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax));
	//background->AddFriend("BDT");
	background->AddFriend("hiEvtAnalyzer/HiTree");
	background->AddFriend("skimanalysis/HltTree");
	background->AddFriend("hltanalysis/HltTree");

	cout << "PASS = 0.1" << background->GetEntries() << endl;
	
	TTree* signal = (TTree*)inputS->Get("Bfinder/ntphi");
	signal->AddFriend("hiEvtAnalyzer/HiTree");

	
	cout << "PASS =  0.2" << background->GetEntries() << endl;

	signal->AddFriend("hltanalysis/HltTree");
	signal->AddFriend("skimanalysis/HltTree");

	cout << "PASS =  0.3" << background->GetEntries() << endl;

	//signal->AddFriend("ntHlt");
	//	signal->AddFriend("ntHi");
	//	signal->AddFriend("ntSkim");
	signal->AddFriend(Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax));
	//	signal->AddFriend("mvaTree");
	//signal->AddFriend("BDT");



	signal->AddFriend("CentWeightTree");
	TTree* generated = (TTree*)inputS->Get("Bfinder/ntGen");
	generated->AddFriend("hiEvtAnalyzer/HiTree");

//	TString BptWeight="0.474599*TMath::Exp(-0.001406*Bpt)+38.036016/(Bpt*Bpt+0.000330*0.000330)";
	TString BptWeight="0.603534*TMath::Exp(-0.006505*Bpt)+13.177674/(Bpt*Bpt -4.418950 * Bpt + 0.009566*0.009566)";

	TString	CentWeight = "CentWeight";
//	TString	PVzWeight="(0.164847 * TMath::Exp(- 0.021378 * (PVz - 0.342927)*(PVz - 0.342927)))/(0.159507 * TMath::Exp(- 0.019986 * (PVz - 0.601387)*(PVz - 0.601387)))";
	TString	PVzWeight="0.163562 * TMath::Exp(- 0.021039 * (PVz - 0.426587)*(PVz - 0.426587))/(0.159619 * TMath::Exp(- 0.020011 * (PVz - 0.587652)*(PVz - 0.587652)))";

	//generated->AddFriend("ntHlt");
	//	generated->AddFriend("ntHi");

	cout << "pass  1 "<< endl;

	for(int i=0;i<=nMVA;i++) gmvaBins[i] = minMVA+i*widMVA;
	cout <<  "pass  1.1 " << endl;
	TH1F* hcountEffS = new TH1F("hcountEffS",Form(";%s;Signal counts",mvatype.Data()),nMVA,minMVA,maxMVA);
	TH1F* hcountEffB = new TH1F("hcountEffB",Form(";%s;Background counts",mvatype.Data()),nMVA,minMVA,maxMVA);
//TH1F* hcountEffS = new TH1F("hcountEffS",Form(";%s;Signal counts",mvatype.Data()),1000,-1.0,1.0);
//TH1F* hcountEffB = new TH1F("hcountEffB",Form(";%s;Background counts",mvatype.Data()),1000,-1.0,1.0);
	cout << "Pass 1.2" << endl;
	signal->Project("hcountEffS",Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax),(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(weightfunctionreco.Data()))*(TCut(cuts.Data())));
	background->Project("hcountEffB",Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax),Form("%s",cutb.Data()));
	cout << "Pass 1.5" << endl;
	cout << "BDTG Branch = " << Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax) << endl;
	cout << "SIGHIS = " << hcountEffS->Integral() << endl;

	cout << "BKGDHIS = " << hcountEffB->Integral() << endl;

	cout << "Cut == " << cutb.Data() << endl;

	TH1F* hdisEffS = (TH1F*)hcountEffS->Clone("hdisEffS");
	hdisEffS->Scale(1./hcountEffS->Integral());
	hdisEffS->SetLineColor(kRed+1);
	hdisEffS->SetFillStyle(1001);
	hdisEffS->SetFillColor(kRed-9);
	hdisEffS->SetLineWidth(3);
	hdisEffS->SetStats(0);
	TH1F* hdisEffB = (TH1F*)hcountEffB->Clone("hdisEffB");
	hdisEffB->Scale(1./hcountEffB->Integral());
	hdisEffB->SetLineColor(kBlue+1);
	hdisEffB->SetFillStyle(1001);
	hdisEffB->SetFillColor(kBlue-9);
	hdisEffB->SetLineWidth(3);
	hdisEffB->SetStats(0);

	cout << "pass  2 "<< endl;


	//TFile *mvaout = new TFile(Form("mvadisfile_%.0f_%.0f.root",ptmin,ptmax),"RECREATE");
	//mvaout->cd();
	TH2F* hemptydis = new TH2F("hemptydis",Form(";%s;Probability",mvatype.Data()),50,-1.2,1.2,10,0.,hdisEffB->GetMaximum()*1.4);
	//TH2F* hemptydis = new TH2F("hemptydis",Form(";%s;Probability",mvatype.Data()),100,-0.8,1.0,10,0.,0.05);

//	cout << "hdisEffB->GetMaximum()*1.4 = " << hdisEffB->GetMaximum()*1.4 << endl;
	//TH2F* hemptydis = new TH2F("hemptydis",Form(";%s;Probability",mvatype.Data()),50,-1.2,1.2,10,0.,1.4);

	//TH2F* hemptydis = new TH2F("hemptydis",Form(";%s;Probability",mvatype.Data()),1000,-0.8,1.0,10,0.,hdisEffB->GetMaximum()*1.4);
	sethempty(hemptydis);
	TLatex* texPt = new TLatex(0.22,0.823, Form("%.0f < B_{s} p_{T} < %.0f GeV",ptmin,ptmax));
	texPt->SetNDC();
	texPt->SetTextAlign(12);
	texPt->SetTextSize(0.045);
	texPt->SetTextFont(42);
	TLatex* texY = new TLatex(0.22,0.773, "|y| < 2.4");
	texY->SetNDC();
	texY->SetTextAlign(12);
	texY->SetTextSize(0.045);
	texY->SetTextFont(42);

	TCanvas* cdisEffSB = new TCanvas("cdisEffSB","",600,600);
	hemptydis->Draw();
	hdisEffB->Draw("samehist");
	hdisEffS->Draw("samehist");
	
	hdisEffB->Write();
	hdisEffS->Write();
//	mvaout->Close();

	TLegend* leg = new TLegend(0.62,0.75,0.97,0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(hdisEffB,"Background","f");
	leg->AddEntry(hdisEffS,"Signal","f");
	leg->Draw("same");
	drawCMS(collisionsyst);
	texPt->Draw();
	texY->Draw();
	cdisEffSB->SaveAs(Form("%s_%s.pdf",outputmvadis.Data(),outputname.Data()));

	Float_t effS[nMVA+1],effB[nMVA+1],effSig[nMVA+1];
	for(int i=0;i<(nMVA+1);i++)
	{
		effS[i] = 0;
		effB[i] = 0;
		effSig[i] = 0;
	}
	for(int i=0;i<nMVA;i++)
	{
		effS[i] = hcountEffS->Integral(i+1,nMVA)/hcountEffS->Integral();
		effB[i] = hcountEffB->Integral(i+1,nMVA)/hcountEffB->Integral();
	}

	cout << "pass  3 "<< endl;

	//TH2F* hemptyeff = new TH2F("hemptyeff",Form(";%s;Efficiency",mvatype.Data()),50,-1.2,1.2,10,0.,1.4);
	TH2F* hemptyeff = new TH2F("hemptyeff",Form(";%s;Efficiency",mvatype.Data()),50,-0.85,1.05,10,0.,1.4);
	sethempty(hemptyeff);
	TGraph* geffS = new TGraph(nMVA+1,gmvaBins,effS);
	geffS->SetMarkerSize(1.1);
	geffS->SetMarkerColor(kRed);
	geffS->SetLineWidth(2);
	geffS->SetLineColor(kRed);
	TGraph* geffB = new TGraph(nMVA+1,gmvaBins,effB);
	geffB->SetMarkerSize(1.1);
	geffB->SetMarkerColor(kBlue+1);
	geffB->SetLineWidth(2);
	geffB->SetLineColor(kBlue+1);
	TCanvas* ceffSB = new TCanvas("ceffSB","",600,600);
	hemptyeff->Draw();
	geffS->Draw("samepl");
	geffB->Draw("samepl");


	TLegend* legeff = new TLegend(0.62,0.75,0.97,0.86);
	legeff->SetBorderSize(0);
	legeff->SetFillStyle(0);
	legeff->AddEntry(geffB,"Background","pl");
	legeff->AddEntry(geffS,"Signal","pl");
	legeff->Draw("same");
	drawCMS(collisionsyst);
	texPt->Draw();
	texY->Draw();
	ceffSB->SaveAs(Form("%s_%s.pdf",outputefficiency.Data(),outputname.Data()));

	Float_t wSignal=0, wBackground=0;
	Float_t* weights = new Float_t[2];
	//
	//TString FONLLName = "fonlls/fo_pp_d0meson_5TeV_y1.dat";
	TString FONLLName = "fonlls/fo_pp_Bplus_5p03TeV_y2p4.dat";
	//calRatio(signal, background, generated, cuts, cutb, cutg, weight, ptmin, ptmax, FONLLName.Data(), lumi, raa, outputname, weights);
	calRatio(signal, background, generated, cuts, cutb, cutg, weightfunctiongen.Data(), weightfunctionreco.Data(), ptmin, ptmax, FONLLName.Data(), lumi, raa, outputname, weights);
	//

	wSignal = weights[1];
	wBackground = weights[0];

	std::cout<<std::endl;
	std::cout<<"Looking for max significance ..."<<std::endl;

	Double_t maxsig = 0;
	Int_t maxindex = 0;


	int doNew = 1;
	double alpha = 1-0.6827;
	/*
	double Non;
	double Noff;
	double Ntot;
	double tau = 0.16/0.20;
	*/
	for(int i=0;i<nMVA;i++)
	{
		//cout << "i = " << i  << "  effS = " << effS[i] << endl;
		
		if(doNew==0){

			cout << "Doing Old Stat Model" << endl;
			effSig[i] = wSignal*effS[i]/sqrt(wSignal*effS[i]+wBackground*effB[i]);

		}
		//New Significance Calculation//

		if(doNew == 1){

		cout << "Doing New Stat Model" << endl;
		/*
		Non = (wSignal*effS[i] + wBackground*effB[i]);
		Noff = (wBackground*effB[i])/tau;
		Ntot = Non + Noff;
		effSig[i] = TMath::Sqrt(2) * TMath::Sqrt( (Non * TMath::Log((Non*(1+tau))/Ntot) + Noff * TMath::Log((Noff*(1+tau))/(Ntot*tau))) );		
		*/
		cout << "S = " <<  wSignal*effS[i] << endl;
		cout << "B = " <<  wBackground*effB[i] << endl;

		if(wSignal*effS[i]+wBackground*effB[i] > 2)  effSig[i] = wSignal*effS[i]/TMath::Sqrt(ROOT::Math::gamma_quantile(alpha/2,wSignal*effS[i]+wBackground*effB[i],1.));

		}

		if(effSig[i]>maxsig)
		{
			maxsig=effSig[i];
			maxindex=i;
		}
	}

	// Print out opt results
	cfresult<<std::endl;
	cfresult<<"  =========================================================="<<std::endl;
	cfresult<<" |                         Opt Result                       |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"pT"<<" | "<<std::setw(38)<<Form("%.1f - %.1f GeV/c",ptmin,ptmax)<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"RAA"<<" | "<<std::setw(38)<<raa<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"Sig reg"<<" | "<<std::setw(9)<<Form("+- %.2f",dmassDsignal)<<" | "<<std::setw(13)<<"Sideband bkg"<<" | "<<std::setw(10)<<Form("%.0f",wBackground*(dmassDsidbandH-dmassDsidbandL)/dmassDsignal)<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"weight S"<<" | "<<std::setw(9)<<Form("%.0f",wSignal)<<" | "<<std::setw(13)<<"weight B"<<" | "<<std::setw(10)<<Form("%.0f",wBackground)<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"S"<<" | "<<std::setw(9)<<Form("%.0f",wSignal*effS[maxindex])<<" | "<<std::setw(13)<<"B"<<" | "<<std::setw(10)<<Form("%.0f",wBackground*effB[maxindex])<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(15)<<"effS"<<" | "<<std::setw(9)<<effS[maxindex]<<" | "<<std::setw(13)<<"sig"<<" | "<<std::setw(10)<<Form("%.2f",effSig[maxindex])<<" |"<<std::endl;
	cfresult<<"  ----------------------------------------------------------"<<std::endl;
	cfresult<<" | "<<std::setiosflags(std::ios::left)<<std::setw(35)<<mvatype<<" | "<<std::setw(18)<<gmvaBins[maxindex]<<" |"<<std::endl;
	cfresult<<"  =========================================================="<<std::endl;
	cfresult<<std::endl;

	ofresult.close();

	std::ofstream RAAZoo(Form("RAAScan_%d_%d.dat",int(ptmin),int(ptmax)), ios::app);
	RAAZoo << raa << "     " <<  effS[maxindex] << "     " << effB[maxindex] << "      " << gmvaBins[maxindex] << "     " << effSig[maxindex] << endl;

		TH2F* hempty = new TH2F("hempty",Form(";%s;S/sqrt(S+B)",mvatype.Data()),50,-1.2,1.2,10,0.,maxsig*1.4);
//	TH2F* hempty = new TH2F("hempty",Form(";%s;S/sqrt(S+B)",mvatype.Data()),50,0.85,1.05,10,0.,maxsig*1.4);
	sethempty(hempty);
	TGraph* gsig = new TGraph(nMVA+1,gmvaBins,effSig);
	TCanvas* csig = new TCanvas("csig","",600,600);
	hempty->Draw();
	gsig->Draw("same*");
	drawCMS(collisionsyst);  
	texPt->Draw();
	texY->Draw();
	csig->SaveAs(Form("%s_%s.pdf",outputsignificance.Data(),outputname.Data()));

}

int main(int argc, char* argv[])
{
	if(argc==16)
	{
		readxml(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], atof(argv[12]), atof(argv[13]), atof(argv[14]), atof(argv[15]));
		return 0;
	}
	if(argc==15)
	{
		readxml(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], atof(argv[12]), atof(argv[13]), atof(argv[14]));
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - readxml()"<<std::endl;
		return 1;
	}
}

