

#include "GeomService.hh"
#include "GeomHandle.hh"

#include "DRFPIcalorimeter.hh"
#include "DRCaloIO.hh"
//#include "g4root.hh"

#include <fstream>
#include <iostream>
#include <cstddef>

namespace drc {

static DRCaloIO* instance = 0;

DRCaloIO::DRCaloIO(std::string outFold)
  :  _outFold(outFold),
//	 cont(0),
	 Fibers(0.),                 //vector of Fibers
	 FiberIDs(0),                //vector of Fibers IDs
	 Fiber_Hits{0.},
	 Tracking_Hits{0.},
	 Energyem(0.),
	 EnergyScin(0.),
	 EnergyCher(0.),
	 NofCherenkovDetected(0),
	 //NofScintillationDetected(0),
	 EnergyTot(0.),
	 PrimaryParticleName(""),
	 PrimaryParticleEnergy(0.),
	 neutrinoleakage(0.),
	 leakage(0.),
	 VectorR_loop(0.),
	 VectorL_loop(0.),
	 VectorSignalsR(0.),
	 VectorSignalsL(0.),
	 VectorSignalsCherR(0.),
	 VectorSignalsCherL(0.),
	 VectorR(0.),
	 VectorL(0.),
	 fNbOfBarrel(0),
	 fNbOfEndcap(0),
	 fNbOfZRot(0)
{
}

DRCaloIO::~DRCaloIO(){
	if (instance !=0 ) {
		delete instance;
		instance=0;
	}
}

DRCaloIO* DRCaloIO::GetInstance(std::string outFold){

  if (instance == 0 ){

    instance = new DRCaloIO(outFold);
  }
  
  return instance;

}

void DRCaloIO::newEvent(G4int evId, bool writeASCII) {

	if (writeASCII) {
		//Time_distribution event
		std::ofstream TimeFile;
		std::string fname=_outFold+"/"+"Time.txt";
		TimeFile.open(fname.c_str(), std::ios_base::app);
		TimeFile<<"Event "<<evId<<" % % %"<<G4endl;
		TimeFile.close();
	}

	// initialisation per event
	Energyem = 0.;
	EnergyScin = 0.;
	EnergyCher = 0.;
	NofCherenkovDetected = 0;
	//NofScintillationDetected = 0;
	EnergyTot = 0.;
	neutrinoleakage = 0.;
	leakage = 0.;

	if (fNbOfBarrel==0) {
		GeomHandle<DRFPIcalorimeter> drfpic;

		fNbOfBarrel = drfpic->NbOfBarrel();//40;
		fNbOfEndcap = drfpic->NbOfEndcap();//35;
		fNbOfZRot = drfpic->NbOfZRot();//36;
	}

//	int fNbOfBarrel = drfpic->NbOfBarrel();//40;
//	int fNbOfEndcap = drfpic->NbOfEndcap();//35;
//	int fNbOfZRot = drfpic->NbOfZRot();//36;
	/*for(int i=0;i<64;i++){
   Signalfibre[i]=0;
  }*///only if you want to use SignalFibre[64]

	Fibers.clear();

	FiberIDs.clear();

	for (int i=0;i<1000000;i++)
		Fiber_Hits[i]={0.};

	for (int i=0;i<200;i++)
		Tracking_Hits[i]={0.};

	for (int i=0;i<VectorR.size();i++){
		VectorR.at(i)=0.;
	}
	for (int i=0;i<VectorL.size();i++){
		VectorL.at(i)=0.;
	}

	for (int i=0;i<VectorR_loop.size();i++){
		VectorR_loop.at(i)=0.;
	}
	for (int i=0;i<VectorL_loop.size();i++){
		VectorL_loop.at(i)=0.;
	}

	for (int i=0;i<VectorSignalsR.size();i++){
		VectorSignalsR.at(i)=0.;
	}
	for (int i=0;i<VectorSignalsL.size();i++){
		VectorSignalsL.at(i)=0.;
	}
	for (int i=0;i<VectorSignalsCherR.size();i++){
		VectorSignalsCherR.at(i)=0.;
	}
	for (int i=0;i<VectorSignalsCherL.size();i++){
		VectorSignalsCherL.at(i)=0.;
	}
	PrimaryParticleEnergy = 0;

	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorR.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorR.push_back(0.);}}
	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorL.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorL.push_back(0.);}}

	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorR_loop.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorR_loop.push_back(0.);}}
	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorL_loop.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorL_loop.push_back(0.);}}

	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorSignalsR.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorSignalsR.push_back(0.);}}
	for(int i=0;i<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);i++){
		if(VectorSignalsL.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorSignalsL.push_back(0.);}}
	//VectorSignals.at(i)=0;}
	for(int k=0;k<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);k++){
		if(VectorSignalsCherR.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorSignalsCherR.push_back(0.);}}
	for(int k=0;k<=fNbOfZRot*(fNbOfBarrel+fNbOfEndcap);k++){
		if(VectorSignalsCherL.size() <= fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)){
			VectorSignalsCherL.push_back(0.);}}
}

void DRCaloIO::writeASCIIEvent(G4int evId) {
	std::ofstream eventFile;
	std::string fname=_outFold+"/"+"Event.txt";
	eventFile.open(fname.c_str(), std::ios_base::app);
	/*G4cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
	G4cout<<"\t ID \t Energy(MeV) \t S/C \t Position \t slice \t tower"<<std::endl;
	G4cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;*/
	int v=0;
	G4double E=0.;
	if(evId==0)	eventFile<<"EvtID\tFiberID\tEt\tXt\tYt\tZt\tFlagt\tslicet\ttowert"<<std::endl;
	while(Fiber_Hits[v].F_ID!=0){
		//G4cout<<Fiber_Hits[v].F_ID<<"\t"<<Fiber_Hits[v].F_E<<"\t"<<Fiber_Hits[v].F_Type<<"\t"<<Fiber_Hits[v].F_X<<" "<<Fiber_Hits[v].F_Y<<" "<<Fiber_Hits[v].F_Z<<std::endl;
		E = E+Fiber_Hits[v].F_E;
		eventFile<<evId<<"\t"<<std::fixed << std::setprecision(3) <<Fiber_Hits[v].F_ID<<"\t"<<Fiber_Hits[v].F_E<<"\t"<<Fiber_Hits[v].F_X<<"\t"<<Fiber_Hits[v].F_Y<<"\t"<<Fiber_Hits[v].F_Z<<"\t"<<Fiber_Hits[v].F_Type<<"\t"<<Fiber_Hits[v].F_slice<<"\t"<<Fiber_Hits[v].F_tower<<std::endl;
		v++;
	}
	//eventFile<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
	eventFile.close();

	std::ofstream eventFile1;
	fname=_outFold+"/"+"Event_Track.txt";
	eventFile1.open(fname.c_str(), std::ios_base::app);
	if(evId==0)	eventFile1<<"EvtIDtrack\tTrackID\tXtrackt\tYtrackt\tZtrackt\tparticlesnamet"<<std::endl;
	for (v=0;v<200;v++){
		if(Tracking_Hits[v].T_ID!=0){
			eventFile1<<evId<<"\t"<<Tracking_Hits[v].T_ID<<"\t"<<Tracking_Hits[v].T_X<<"\t"<<Tracking_Hits[v].T_Y<<"\t"<<Tracking_Hits[v].T_Z<<"\t"<<Tracking_Hits[v].T_Name<<std::endl;
		}
	}
	eventFile1.close();
	//eventFile<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;

	/*G4cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
	G4cout<<"N fired: "<<v<<"\t tot E: "<<E<<"\t Lor: "<<NofCherenkovDetected<<std::endl;*/

}

//void DRCaloIO::fillAnlysis(){
//
//	  // Accumulate statistics
//	  //
//
//	  // get analysis manager
//	  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
//
//	  // fill histograms
//	  //analysisManager->FillH1(1, Energymodule);
//	  //analysisManager->FillH1(2, TrackLmodule);
//	  //analysisManager->FillH1(3, EnergyScin);
//
//	  // fill ntuple event by event
//	  analysisManager->FillNtupleDColumn(0, Energyem);
//	  analysisManager->FillNtupleDColumn(1, EnergyScin);
//	  analysisManager->FillNtupleDColumn(2, EnergyCher);
//	  analysisManager->FillNtupleDColumn(3, NofCherenkovDetected);
//	  analysisManager->FillNtupleDColumn(4, EnergyTot);
//	  analysisManager->FillNtupleDColumn(5, PrimaryParticleEnergy);
//	  analysisManager->FillNtupleSColumn(6, PrimaryParticleName);
//	  analysisManager->FillNtupleDColumn(7, neutrinoleakage);
//	  analysisManager->FillNtupleDColumn(8, leakage);
//	  analysisManager->AddNtupleRow();//columns with vector are automatically filled with this function
//
//	  //print here if you need event by event some information of the screen
//	  //G4cout<<EnergyTot<<G4endl;
//
//}

// inline functions

void DRCaloIO::Addneutrinoleakage(G4double de){
    neutrinoleakage += de;
}

 void DRCaloIO::Addleakage(G4double de){
    leakage += de;
}

/*inline*/ void DRCaloIO::AddVectorR(G4double de, G4int tower, G4int slice){
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorR.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorR element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//	VectorR.at(tower+(slice*75)) += de;
}

/*inline*/ void DRCaloIO::AddVectorL(G4double de, G4int tower, G4int slice){
	tower = -1*tower;
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorL.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorL element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//	VectorL.at(tower+(slice*75)) += de;
}

/*inline�*/ void DRCaloIO::AddVectorR_loop(G4double de, G4int tower, G4int slice){
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorR_loop.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorR_loop element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorR_loop.at(tower+(slice*75)) = de;
}

/*inline*/ void DRCaloIO::AddVectorL_loop(G4double de, G4int tower, G4int slice){
    tower = -1*tower;
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorL_loop.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorL_loop element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorL_loop.at(tower+(slice*75)) = de;
}

void DRCaloIO::appendfiber(int ID, int Type, int Slice, int Tower, int E, G4ThreeVector Pos, std::vector<double> phtimes){//function to search if fiber already exists
     Fiber f{ID, Type, Slice, Tower, E, Pos, phtimes};
     auto it = find(FiberIDs.begin(), FiberIDs.end(), f.ID); //return iterator to fiber ID if exhists or FiberIDs.end() if fiber ID is not found
     if ( it == FiberIDs.end() ){       //fiber not found
       FiberIDs.push_back(f.ID);       //push_back fiber ID
	Fibers.push_back(f);            //push_back fiber
     }
     else {                             //fiber found
       Fibers[distance(FiberIDs.begin(), it)].addfiber(f); //add fiber contribution with addfiber()
     }
   };

/*inline*/ void DRCaloIO::WriteFiber_Info(G4double FID, G4double FE, G4int FType, G4ThreeVector Fpos, G4int slice, G4int tower){
    int k=0;
    while (Fiber_Hits[k].F_ID!=0 && Fiber_Hits[k].F_ID!=FID){
        k++;}
    Fiber_Hits[k].F_ID = FID;
    Fiber_Hits[k].F_E += FE;
    Fiber_Hits[k].F_Type = FType;
    Fiber_Hits[k].F_X = Fpos[0];
    Fiber_Hits[k].F_Y = Fpos[1];
    Fiber_Hits[k].F_Z = Fpos[2];
    Fiber_Hits[k].F_slice = slice;
    Fiber_Hits[k].F_tower = tower;
}

/*inline*/ void DRCaloIO::WriteTracking_Info(G4double TID, G4ThreeVector Tpos, G4String Name, G4double Ek){
    int k=0;
    while (Tracking_Hits[k].T_ID!=0 && Tracking_Hits[k].T_ID!=TID){
        k++;}
    Tracking_Hits[k].T_ID = TID;
    Tracking_Hits[k].T_X = Tpos[0];
    Tracking_Hits[k].T_Y = Tpos[1];
    Tracking_Hits[k].T_Z = Tpos[2];
    Tracking_Hits[k].T_Name = Name;
    if (Tracking_Hits[k].T_Ek<=0.) {Tracking_Hits[k].T_Ek = Ek;}
}


/*inline*/ void DRCaloIO::SavePrimaryParticle(G4String name){
  PrimaryParticleName = name;
}

/*inline*/ void DRCaloIO::SavePrimaryEnergy(G4double primaryparticleenergy){
  PrimaryParticleEnergy = primaryparticleenergy;
}

/*inline*/ void DRCaloIO::AddVectorScinEnergyR(G4double de, G4int tower, G4int slice) {
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorSignalsR.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorScinEnergyR element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorSignalsR.at(tower+(slice*75)) += de;
}

/*inline*/ void DRCaloIO::AddVectorScinEnergyL(G4double de, G4int tower, G4int slice) {
    tower = -1*tower;
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorSignalsL.at(id) += de;
	} else {
		std::cerr<<"DRCaloIO::AddVectorScinEnergyL element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorSignalsL.at(tower+(slice*75)) += de;
}

/*inline*/ void DRCaloIO::AddVectorCherPEL(G4int c_signal, G4int tower, G4int slice) {
	tower = -1*tower;
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
		VectorSignalsCherL.at(id) = VectorSignalsCherL.at(id)+c_signal;
	} else {
		std::cerr<<"DRCaloIO::AddVectorCherPEL element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorSignalsCherL.at(tower+(slice*75)) = VectorSignalsCherL.at(tower+(slice*75))+c_signal;
}

/*inline*/ void DRCaloIO::AddVectorCherPER(G4int c_signal, G4int tower, G4int slice) {
	std::size_t id = tower+(slice*75);
	if (id<fNbOfZRot*(fNbOfBarrel+fNbOfEndcap)) {
	    VectorSignalsCherR.at(id) = VectorSignalsCherR.at(id)+c_signal;
	    G4cout<<"DRCaloIO step 1 done"<<std::endl;
	} else {
		std::cerr<<"DRCaloIO::AddVectorCherPER element out of range: tw "<<tower<<" slice "<<slice<<std::endl;
	}
//    VectorSignalsCherR.at(tower+(slice*75)) = VectorSignalsCherR.at(tower+(slice*75))+c_signal;
}

/*inline*/ void DRCaloIO::Addem(G4double de) {
  Energyem += de;
}

/*inline*/ void DRCaloIO::AddScin(G4double de){
  EnergyScin += de;
}

/*inline*/ void DRCaloIO::AddCher(G4double de){
  EnergyCher += de;
}

/*inline*/ void DRCaloIO::AddCherenkov(){
  NofCherenkovDetected = NofCherenkovDetected + 1;
}

/*inline void DRCaloIO::AddScintillation(){
  NofScintillationDetected = NofScintillationDetected +1;
}*/

/*inline*/ void DRCaloIO::Addenergy(G4double de){
  EnergyTot += de;
}
 


/*inline void DRCaloIO::AddEnergyfibre(G4double de, G4int number){
    Signalfibre[number] += de;
}*/

/*inline void DRCaloIO::AddSignalfibre(G4int number){
    Signalfibre[number] = Signalfibre[number] + 1;
}*/

}