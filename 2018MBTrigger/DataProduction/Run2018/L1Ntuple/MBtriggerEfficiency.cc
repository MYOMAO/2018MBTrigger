
// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"


#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile2D.h"
#include "TMath.h"
#include "TTree.h"
//add L1 stuff
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTProducer.h"
#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//
// #include "DataFormats/L1Trigger/interface/CaloTower.h"
//

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtObject.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtFdlWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtLogicParser.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtObject.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTrigger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"



//Include New



// class declaration
//

class MBtriggerEfficiency : public edm::EDAnalyzer {
	public:
		explicit MBtriggerEfficiency(const edm::ParameterSet&);
		~MBtriggerEfficiency();

		int doCharge = 0;

		int ADCSize;
		int ADCBoth[10000];
		int ADCBack[10000];
		int ADCFront[10000];
		int ieta[10000];
		int iphi[10000];
		int depth[10000];

		int chargeSize; 
		double charge[10000];
		double chargeped[10000];
		double energy[10000];
		double energyped[10000];


		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

		std::string outfname;
		// TFile* outputFile;
		edm::EDGetTokenT<QIE10DigiCollection> hfDigiTag;
		edm::EDGetTokenT<CaloTowerCollection> caloTowerTag;
		// edm::EDGetTokenT<l1t::CaloTowerBxCollection> caloTowerTag;
		edm::EDGetTokenT<edm::View<reco::Track>> trackTag;
		edm::EDGetTokenT<reco::VertexCollection> vertexTag;

		int evtsTot;

		TH1F* hEvtnum;
		TH1F* accPerEvt;
		TH1F* accPerEvtAnd;
		TH1F* accPerEvtPlus;
		TH1F* accPerEvtMinus;

		TH1F* accPerEvtThrFileSumEtDeno;

		TH2F* accPerEvtThrFileSumEt2D;
		TH2F* accPerEvtThrFileSumEtAnd2D;
		TTree * ADC = new TTree("ADC","ADC");

		// edm::InputTag digis_;

		// edm::InputTag m_l1stage2globalAlgBlk;
		// edm::EDGetTokenT<GlobalAlgBlkBxCollection> l1tStage2uGtSource_;

	private:
		// L1GtUtils m_l1GtUtils;

		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;
		// edm::Service<TFileService> fs;
		virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
		//virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
		//virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
		//virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
//		edm::ESHandle<HcalDbService> conditions;
		const HcalDDDRecConstants *hcons;
		const HcalTopology *htopology;
		// ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MBtriggerEfficiency::MBtriggerEfficiency(const edm::ParameterSet& iConfig)
// digis_(iConfig.getParameter<edm::InputTag>("triggerPrimitives"))
// m_l1GtUtils(iConfig, consumesCollector(), true)//this is important for 80x to compile

{
	outfname = iConfig.getParameter<std::string>("outputFileName");
	hfDigiTag = consumes<QIE10DigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("hfDigiTag"));
	caloTowerTag = consumes<CaloTowerCollection>(iConfig.getParameter<edm::InputTag>("caloTowerTag"));
	// caloTowerTag = consumes<l1t::CaloTowerBxCollection>(iConfig.getParameter<edm::InputTag>("caloTowerTag"));
	trackTag = consumes<edm::View<reco::Track>>(iConfig.getParameter<edm::InputTag>("trackTag"));
	vertexTag = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertexTag"));

	accPerEvt = new TH1F("accPerEvt",";Threshold;Efficiency Or",40,0,40);
	accPerEvt->TH1F::Sumw2();
	accPerEvtPlus = new TH1F("accPerEvtPlus",";Threshold;Efficiency Plus",40,0,40);
	accPerEvtPlus->TH1F::Sumw2();
	accPerEvtMinus = new TH1F("accPerEvtMinus",";Threshold;Efficiency Minus",40,0,40);
	accPerEvtMinus->TH1F::Sumw2();
	accPerEvtAnd = new TH1F("accPerEvtAnd",";Threshold;Efficiency And;",40,0,40);
	accPerEvtAnd->TH1F::Sumw2();
	hEvtnum = new TH1F("hEvtnum", "", 1, 0, 1);

	accPerEvtThrFileSumEtDeno = new TH1F("accPerEvtThrFileSumEtDeno", "accPerEvtThrFileSumEtDeno", 5000, 0, 5000);
	accPerEvtThrFileSumEt2D = new TH2F("accPerEvtThrFileSumEt2D", "accPerEvtThrFileSumEt2D", 40, 0, 40, 5000, 0, 5000);
	accPerEvtThrFileSumEtAnd2D = new TH2F("accPerEvtThrFileSumEtAnd2D","accPerEvtThrFileSumEtAnd2D", 40, 0, 40, 5000, 0, 5000);

	evtsTot=0;
	ADC->Branch("ADCSize",&ADCSize);
	ADC->Branch("ADCBoth",ADCBoth,"ADCBoth[ADCSize]/I");
	ADC->Branch("ieta",ieta,"ieta[ADCSize]/I");
	ADC->Branch("iphi",iphi,"iphi[ADCSize]/I");
	ADC->Branch("depth",depth,"depth[ADCSize]/I");
	/*
	   ADC->Branch("ADCFornt",ADCFront,"ADCFront[ADCSize]/I");
	   ADC->Branch("ADCBack",ADCBack,"ADCBack[ADCSize]/I");

	   ADC->Branch("chargeSize",&chargeSize);
	   ADC->Branch("charge",charge,"charge[chargeSize]/D");
	   ADC->Branch("chargeped",chargeped,"chargeped[chargeSize]/D");

	   ADC->Branch("energy",energy,"energy[chargeSize]/D");
	   ADC->Branch("energyped",energyped,"energyped[chargeSize]/D");
	   */
	//  outputFile=new TFile(outfname.c_str(),"RECREATE");
}


MBtriggerEfficiency::~MBtriggerEfficiency()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void MBtriggerEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	using namespace edm;
	//select particular lumi, bunch, etc...
	edm::Handle<QIE10DigiCollection> digi;
	//edm::Handle<HFDigiCollection> digi;
	iEvent.getByToken(hfDigiTag,digi);
	if(!iEvent.getByToken(hfDigiTag,digi)) std::cout << "invalid hfDigiTag" << std::endl; 
	else std::cout << "good hfDigiTag" << std::endl;
	edm::Handle<CaloTowerCollection> towers;
	// edm::Handle<l1t::CaloTowerBxCollection> towers;
	iEvent.getByToken(caloTowerTag, towers);
	if(!iEvent.getByToken(caloTowerTag, towers)) std::cout << "invalid caloTowerTag (not necessary)" << std::endl;
	else std::cout << "good caloTowerTag" << std::endl;

	//	ESHandle<HcalDbService> conditions;
//	iSetup.get<HcalDbRecord>().get(conditions);
	// double etHFtowerSumPlus = 0.0;
	// double etHFtowerSumMinus = 0.0;
	double etHFtowerSum = 0.0;

	edm::Handle<reco::VertexCollection> vertices;
	iEvent.getByToken(vertexTag, vertices);
	if(iEvent.getByToken(vertexTag, vertices)) std::cout << "good vertexTag" << std::endl;
	else std::cout << "invalid vertexTag" << std::endl;

	// double bestvz=-999.9, bestvx=-999.9, bestvy=-999.9;
	// double bestvzError=-999.9, bestvxError=-999.9, bestvyError=-999.9;
	// const reco::Vertex & vtx = (*vertices)[0];
	// bestvz = vtx.z();
	// bestvx = vtx.x();
	// bestvy = vtx.y();
	// bestvzError = vtx.zError();
	// bestvxError = vtx.xError();
	// bestvyError = vtx.yError();

	edm::Handle<edm::View<reco::Track>> tracks;
	iEvent.getByToken(trackTag, tracks);
	if(iEvent.getByToken(trackTag, tracks)) std::cout << "good trackTag" << std::endl;
	else std::cout << "invlid trackTag" << std::endl;

	// int nMult_ass_good = 0;
	// for(unsigned it = 0; it < tracks->size(); it++)
	//   {
	//     const reco::Track & trk = (*tracks)[it];
	//     math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

	//     double dzvtx = trk.dz(bestvtx);
	//     double dxyvtx = trk.dxy(bestvtx);
	//     double dzerror = sqrt(trk.dzError()*trk.dzError()+bestvzError*bestvzError);
	//     double dxyerror = sqrt(trk.d0Error()*trk.d0Error()+bestvxError*bestvyError);

	//     if( trk.algo() != 4 ) continue;

	//     if(!trk.quality(reco::TrackBase::highPurity)) continue;
	//     if(fabs(trk.ptError())/trk.pt() > 0.1 ) continue;
	//     if(fabs(dzvtx/dzerror) > 3.0 ) continue;
	//     if(fabs(dxyvtx/dxyerror) > 3.0 ) continue;
	//     if(fabs(trk.eta()) < 2.4 && trk.pt() > 0.4){ nMult_ass_good++; }// NtrkOffline
	//   }

	// if(nMult_ass_good == 0) return;

	cout << "event = " << evtsTot << endl;
	evtsTot++;

	bool fire[40];
	bool firePlus[40];
	bool fireMinus[40];
	for (int p=0; p<40; p++)
	{
		fire[p] = false;
		firePlus[p] = false;
		fireMinus[p] = false;
	}

	for(uint32_t i = 0; i < digi->size(); i++)
	{
		QIE10DataFrame frame = static_cast<QIE10DataFrame>((*digi)[i]);
		HcalDetId cell = frame.detid();
		//	if(cell.subdet() != HcalForward) continue; // is HF?
		ieta[i] = cell.ieta();
		depth[i] = cell.depth();
		iphi[i] = cell.iphi();
		ADCSize = digi->size();
		int ampl=0;
		int amplFront=0;
		int amplBack=0;
		int digiSize=frame.samples();

		for (int k=0; k<digiSize; k++)
		{
			QIE10DataFrame::Sample sam = frame[k];
			int adc=frame[k].adc();
			if(sam.soi()) ampl+=adc;
			if (k==1) amplFront+=adc;
			if (k==3) amplBack+=adc;

		}
		for (int p=0; p<40; p++)
		{
			if (ampl>p) {fire[p]=true;}
			if (ampl>p&&ieta>0) firePlus[p]=true;
			if (ampl>p&&ieta<0) fireMinus[p]=true;
		}
		ADCBoth[i] = ampl;
		ADCBack[i] = amplBack;
		ADCFront[i] = amplFront;
			// Reconstruct the charge, energy, etc
/*
		if(doCharge == 1){

			const HcalCalibrations& calibrations(conditions->getHcalCalibrations(cell));
			const HcalQIECoder* channelCoder = conditions->getHcalCoder(cell);
			const HcalQIEShape* shape = conditions->getHcalShape(channelCoder);
			const HcalCoderDb coder(*channelCoder, *shape);
			CaloSamples tool;
			coder.adc2fC(frame, tool);

			int soi = tool.presamples();

			charge[i] = 0;
			energy[i] = 0;
			chargeped[i] = 0;
			energyped[i] = 0;
			chargeSize = digi->size();

			for (int ii = 0; ii < tool.size(); ii++) {
				//	QIE10DataFrame::Sample sam = frame[ii];
				int capid = frame[ii].capid();
				if( ii == soi) {
					charge[i] = (tool[ii] - calibrations.pedestal(capid));
					energy[i] = energy[i] +  charge[i] * calibrations.respcorrgain(capid);
					chargeped[i] = tool[ii];
					energyped[i] = energyped[i] + chargeped[i] * calibrations.respcorrgain(capid);
				}

			}
		}
*/
	}

	for (int p=0; p<40; p++)
	{
		if (fire[p]) { accPerEvt->Fill(p,1); accPerEvtThrFileSumEt2D->Fill(p, etHFtowerSum,1); }
		if (firePlus[p]) accPerEvtPlus->Fill(p,1);
		if (fireMinus[p]) accPerEvtMinus->Fill(p,1);
		if (firePlus[p] && fireMinus[p]) { accPerEvtAnd->Fill(p,1); accPerEvtThrFileSumEtAnd2D->Fill(p, etHFtowerSum,1); }
	}
	ADC->Fill();

}
// ------------ method called once each job just before starting event loop  ------------
	void 
MBtriggerEfficiency::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
	void 
MBtriggerEfficiency::endJob() 
{
	//outputFile->cd();

	accPerEvt->Scale(pow(evtsTot,-1));
	accPerEvtPlus->Scale(pow(evtsTot,-1));
	accPerEvtMinus->Scale(pow(evtsTot,-1));
	accPerEvtAnd->Scale(pow(evtsTot,-1));

	accPerEvtThrFileSumEt2D->Scale(pow(evtsTot,-1));
	accPerEvtThrFileSumEtAnd2D->Scale(pow(evtsTot,-1));
	accPerEvtThrFileSumEtDeno->Scale(pow(evtsTot,-1));

	hEvtnum->SetBinContent(1, evtsTot);

	accPerEvt->Write();
	accPerEvtPlus->Write();
	accPerEvtMinus->Write();
	accPerEvtAnd->Write();

	accPerEvtThrFileSumEt2D->Write();
	accPerEvtThrFileSumEtAnd2D->Write();
	accPerEvtThrFileSumEtDeno->Write();
	hEvtnum->Write();
	//  ADC->Write();

	//  outputFile->Close();
	// std::cout<<"wrote stuff"<<std::endl;
}

// ------------ method called when starting to processes a run  ------------

	void 
MBtriggerEfficiency::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
	/*
	   bool useL1EventSetup = true;
	   bool useL1GtTriggerMenuLite = false;

	// could be added in beginRun(...) - if not added, the caching will be done in analyze/produce/filter method
	m_l1GtUtils.getL1GtRunCache(iRun, iSetup, useL1EventSetup, useL1GtTriggerMenuLite);
	*/
}

// ------------ method called when ending the processing of a run  ------------
/*
   void 
   MBtriggerEfficiency::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void 
   MBtriggerEfficiency::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void 
   MBtriggerEfficiency::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MBtriggerEfficiency::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MBtriggerEfficiency);
