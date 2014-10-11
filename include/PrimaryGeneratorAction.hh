#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class DetectorConstruction;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*,int nuclear,int mass,float energy);    
   ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

  private:
	int atom;
	int massa;
	float energy_nuc;
    G4ParticleGun* particleGun;
    DetectorConstruction* myDetector;
};

#endif


