#ifndef TrackerSD_h
#define TrackerSD_h 1 

#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
class G4Step;
class G4HCofThisEvent;
class TFile;
class TH1F;
class TNtuple;
const int GLOBAL = 5000000;
const int lengthThreshold = 1*nm;

class TrackerSD : public G4VSensitiveDetector
{
  public:
      TrackerSD(G4String, G4double Adensity, G4double Amolar, G4double Awidth, char *nameofion);
     ~TrackerSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);
      float getCorrectRadius (G4ThreeVector position);

  private:
	  int charge;
	  int first;
	  int second;
	  bool wasfirst;
      float array[GLOBAL][2];//5
      int number;
      TFile *f;
      TFile *f1;
      TNtuple *ntuple1;
      TNtuple *ntuple2;
      TNtuple *ntuple3;
      TNtuple *ntuple4;
      TNtuple *ntuple5;
      TNtuple *ntuple6;
      TNtuple *ntuple7;
      bool nickel;
	  G4int trackId;
	  G4ThreeVector start_pos; 
	  G4double entireZ;
	  G4double entireY;  
	  G4double entireX;
	  G4double entireR;
	  G4double endZ;
	  G4double endR; 
	  bool written;
	  G4int launchAmount;
	  bool wasElectron;
	  G4double density;
	  G4double molar;
	  G4double width;
	  G4double kinenergy;
	  //char *nameofion;
};

#endif

