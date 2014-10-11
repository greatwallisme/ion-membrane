#include <iostream>
using namespace std;
#include <stdlib.h>
#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "TFile.h"
#include "TSystem.h"
#include "TNtuple.h"
#include "G4SystemOfUnits.hh"

TrackerSD::TrackerSD(G4String name, G4double Adensity, G4double Amolar, G4double Awidth, char *nameofion)
:G4VSensitiveDetector(name)
{
  /*char * ion;
  ion = (char*)malloc(2*sizeof(char));
  ion = strcopy(nameofion);*/ 
  char *way1, *way2;
  way1 = (char*)malloc(8*sizeof(char));
  way2 = (char*)malloc(36*sizeof(char));
  way1 = strcat(way1, nameofion);
  way1 = strcat(way1, ".root");
  way2 = strcat(way2, nameofion);
  way2 = strcat(way2, "Electron_energy_distribution.root");
  f = new TFile(way1,"RECREATE");
  ntuple1 = new TNtuple(nameofion,"Demo ntuple","z:r:launch");//energy:de/dx
  ntuple2 = new TNtuple("Electron","Demo ntuple","z:r");
  ntuple3 = new TNtuple("ElectronEnd","Demo ntuple","z:r");
  ntuple4 = new TNtuple("Info","Demo ntuple","launchAmount:density:molar:width");
  f1 = new TFile(way2,"RECREATE");
  f1->cd();
  ntuple5 = new TNtuple("Second_electron_energy","Demo ntuple","energy");  
  ntuple6 = new TNtuple("All_electron_energy","Demo ntuple","energy");
  ntuple7 = new TNtuple("First_electron_energy","Demo ntuple","energy"); 
  entireZ = 0;
  entireX = 0;
  entireY = 0;
  entireR = 0;
  first = 0;
  second = 0;
  endZ = 0;
  endR = 0;
  number = 0;
  trackId = 0;
  written = false;
  launchAmount = 0;
  wasElectron = false;
  wasfirst = false;
  density = Adensity;
  molar = Amolar;
  width = Awidth;
}

TrackerSD::~TrackerSD()
{ 
	f->cd();
	ntuple1 -> Write();
	ntuple2 -> Write();
	ntuple3 -> Write();
	ntuple4 -> Fill (launchAmount, density, molar, width/um);
	ntuple4 -> Write();
    f -> Close();
    f1->cd();
    ntuple5 -> Write();
    ntuple6 -> Write();
    ntuple7 -> Write();
    f1 ->Close(); 
    cout << "first_e = " << first << " second_e = " << second << "   "<< (second * 1.0)/(first + second) << endl;
	delete f;	
}

void TrackerSD::Initialize(G4HCofThisEvent*)
{
}

G4bool TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
  
  G4int currentTrackId = (aStep->GetTrack())->GetTrackID();
  G4ThreeVector position =  (aStep->GetTrack()) -> GetPosition();
  
  if ((aStep->GetTrack())->GetDefinition()->GetPDGMass() > 56 ) // криво - подправить
	   nickel=true;
  else 
	   nickel=false;
	   
   	   
  if (nickel) {
	  	if (currentTrackId == trackId) {
			//kinenergy = aStep->GetTrack()->GetKineticEnergy() / MeV;  //for de/dx
			wasElectron = false;
			G4double z = position.z() - start_pos.z();	
			G4double r = sqrt ((position.x() - start_pos.x()) * (position.x() - start_pos.x()) + (position.y() - start_pos.y()) * (position.y() - start_pos.y()));
			/*if ((kinenergy/58 <=1) || (z/cm>=5)){                     //for de/dx
				cout << kinenergy/58 << endl;
				aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			}*/            
			array[number][0] = z / nm;
			array[number][1] = r / nm;
			/*array[number][2] = aStep->GetDeltaEnergy()/eV;            //for de/dx
			array[number][3] = aStep->GetStepLength()/nm;
			array[number][4] = kinenergy;*/
			number ++;
			/*if (array[number-1][0] - array[0][0] >= 1000){            //for de/dx //v .hh vernut' razmer massiva
				G4double  de = 0;
				G4double  dx = 0;
				G4double  e = 0;
				for (int i =0 ; i<number; i++){
					de += array[i][2];
					dx += array[i][3];
					e += array[i][4];
				}
				ntuple1 -> Fill (e/number, de/dx);
				number = 0;	
			}*/
			ntuple1 -> Fill (z/um, r/nm, launchAmount);
		}
		else {
			//cout << charge << endl;
			launchAmount ++;
			trackId = currentTrackId;
			number = 1; 
			array[0][0] = 0;
			array[0][1] = 0;
			start_pos.setX(position.x());
			start_pos.setY(position.y());
			start_pos.setZ(position.z());
		}
  }
  else {
	  					
	  	if (currentTrackId != trackId){
			if ((wasElectron) && (written)){
				ntuple3 -> Fill (endZ / um, endR, charge);
				wasElectron = false;
				ntuple6 -> Fill(kinenergy);
				if (wasfirst){
					first ++;
					ntuple7 -> Fill(kinenergy);
				}else{
					second++;
					ntuple5 -> Fill(kinenergy);
				}
			}
			if (aStep->GetTrack()->GetDefinition()->GetParticleName()== "e-"){
				kinenergy = aStep->GetTrack()->GetKineticEnergy() / eV;
				if ((aStep->GetTrack())->GetParentID() == 1)
					wasfirst = true;
				else
					wasfirst = false;
				entireZ = position.z() - start_pos.z();
				entireY = position.y() - start_pos.y();
				entireX = position.x() - start_pos.x();
				entireR = getCorrectRadius(position);
				trackId = currentTrackId;
				written = false;
				wasElectron = true;
			} 
		}
		else {
			if ((aStep->GetTrack()->GetDefinition()->GetParticleName()== "e-") && (!written)){
				G4double z = position.z() - start_pos.z()- entireZ;
				G4double y = position.y() - start_pos.y()- entireY;
				G4double x = position.x() - start_pos.x()- entireX;
				if (sqrt (z*z+y*y+x*x) > lengthThreshold){
					ntuple2 -> Fill (entireZ / um, entireR, charge);
					written = true;
				}
			}
			if ((aStep->GetTrack()->GetDefinition()->GetParticleName()== "e-") && (written)){
				endZ = position.z() - start_pos.z();
				endR = getCorrectRadius(position);
			}
		}
		
		if ((aStep->GetTrack()) -> GetDefinition() ->GetParticleName()!= "e-"){
			wasElectron = false;
		}
  }
  return true;
}

float TrackerSD::getCorrectRadius (G4ThreeVector position){
	G4double z = position.z() - start_pos.z();
	G4double r = sqrt ((position.x() - start_pos.x()) * (position.x() - start_pos.x()) + (position.y() - start_pos.y()) * (position.y() - start_pos.y()));
	int min = 0;
	int max = number-1;
	int num = max/2;
	float eps = 10;
	float z1 = array [num][0];
	float closest = z1;
	int closest_num = num;
	z = z/nm;
	while (abs(z1 - z) > eps){
		if (z1 > z){
			max = num;
			num = int ((max - min) / 2) + min;
		}
		else{
			min = num;
			num += int ((max - min) / 2);
		}
		z1 = array[num][0];
		if (abs(z1 - z) < abs(closest - z)){
			closest = z1;
			closest_num = num;
		}
		if (max - min <= 1)
			break;
	}
	return abs (r/nm - array[closest_num][1]);
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
}

