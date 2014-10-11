#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;
#include "TFile.h"
#include "TSystem.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

void electron(char *element, float radius){
	char *way;
	way = malloc(10*sizeof(char));
	way = strcat("./", element);
	way = strcat(way, ".root");
	TFile* f = TFile :: Open(way);
	
	TNtuple *tuple, *ntuple;
	float *info, *content;
	tuple = (TNtuple*) f->Get("Info");
	tuple -> GetEntry(0);
	info = tuple->GetArgs();
	int num = info[0];
	float density = info[1];
	float molar = info[2];
	float minz = 0;
	int maxz = info[3];
	int minr = 10, maxr = int(1.1*radius);
	ntuple = (TNtuple*) f->Get("Electron");
		
	const int binz = 100, binr = 75;
	int j;
	
	
	TH1F *h2a = new TH1F("Section", "Average_section", binr, minr, maxr); // delim na nol'
	//TH2F *h3b = new TH2F("Electron_amount","Ionisation", binz, minz, maxz, binr, minr, maxr);
		
	for (j = 0; j < ntuple->GetEntries(); j++){
		ntuple->GetEntry(j);
        content = ntuple->GetArgs();
		//h3b->Fill(content[0], content[1], 1.0/num);
		h2a->Fill(content[1], 1.0/6.28/num/(maxz-minz)/content[1]); // 2*pi krivo
	}
    cout << "ThresholdConcentration=" << h2a->GetBinContent(h2a->FindBin(radius)) << " +/- " << h2a->GetBinError(h2a->FindBin(radius)) << " amount/(um*nm)" << endl;
    
    /*TH2F *h3c = new TH2F("Electron_end","Ionisation_end", binz, minz, maxz, binr, minr, maxr); 
	ntuple = (TNtuple*) f->Get("ElectronEnd");
	
	for (j = 0; j < ntuple->GetEntries(); j++){
		ntuple->GetEntry(j);
        content = ntuple->GetArgs();
		h3c->Fill(content[0], content[1], 1.0/num);
	}
	TCanvas *c4 = new TCanvas("Average_section","Average_section");
	TCanvas *c3 = new TCanvas("EndIonisation","EndIonisation");
	TCanvas *c2 = new TCanvas("Ionisation","Ionisation");
	TCanvas *c1 = new TCanvas("Difference","Substraction");
	
	TH2F *hnew = (TH2F*)h3c->Clone("hnew");
	c4->cd();*/
	h2a->Draw();
	/*c3->cd();
	h3c->Draw("lego2");
	c2->cd();
    h3b->Draw("lego2");
    c1->cd();
    hnew->Add(h3b, -1);
    hnew->Draw("lego2");*/
}
