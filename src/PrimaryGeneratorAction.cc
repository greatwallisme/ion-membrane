#include <iostream>
using namespace std;
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(
                                               DetectorConstruction* myDC,int nuclear,int mass,float energy)
:myDetector(myDC)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  atom = nuclear;
  massa = mass;
  energy_nuc = energy;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  G4double position = -0.5*(myDetector->GetWorldFullLength());
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,position));
  
  G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->FindIon(atom, massa, 0);//28 58 // 36 84 // 54 129 // 82 208
  particleGun->SetParticleDefinition(ion);
  particleGun->SetParticleEnergy(energy_nuc * massa* MeV); //6.7 Mev * 58 nuclo // 6.2 * 84 // 3.5 * 129 // 11.1 * 208
  particleGun->SetParticleCharge(2.*eplus);
  
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  
  particleGun->GeneratePrimaryVertex(anEvent);
}
