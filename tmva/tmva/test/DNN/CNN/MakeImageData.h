#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"

void makeImages(int n = 10000) {

   auto h1 = new TH2D("h1","h1",8,0,10,8,0,10);
   auto h2 = new TH2D("h2","h2",8,0,10,8,0,10);

   auto f1 = new TF2("f1","xygaus"); 
   auto f2 = new TF2("f2","xygaus");
   float x1[64];
   float x2[64]; 
   TTree sgn("sgn","sgn");
   TTree bkg("bkg","bkg");
   TFile f("imagesData.root","RECREATE");
  
   for(auto i=0;i<64;i++)
   {
        bkg.Branch(Form("var%d",i),&x1[i]);
        sgn.Branch(Form("var%d",i),&x2[i]);
   }
   
   sgn.SetDirectory(&f);
   bkg.SetDirectory(&f);
   
   f1->SetParameters(1,5,3,5,3);
   f2->SetParameters(1,5,3.3,5,2.7 );
   gRandom->SetSeed(0); 
   for (int i = 0; i < n; ++i) {
      h1->Reset();
      h2->Reset();
      //generate random means in range [3,7] to be not too much on the border
      f1->SetParameter(1,gRandom->Uniform(3,7));
      f1->SetParameter(3,gRandom->Uniform(3,7));
      f2->SetParameter(1,gRandom->Uniform(3,7));
      f2->SetParameter(3,gRandom->Uniform(3,7));

      h1->FillRandom("f1",500);
      h2->FillRandom("f2",500);


      for (int k = 0; k < 8 ; ++k) {
         for (int l = 0; l < 8 ; ++l)  {
            int m = k * 8 + l; 
            x1[m] = h1->GetBinContent(k+1,l+1) + gRandom->Gaus(0,3); 
            x2[m] = h2->GetBinContent(k+1,l+1) + gRandom->Gaus(0,3); 
         }
      }
      sgn.Fill();
      bkg.Fill();
      
//       if (n==1) { 
//          h1->Draw("COLZ");
//          new TCanvas();
//          h2->Draw("COLZ");
//       }
   }
   sgn.Write();
   bkg.Write();
   f.Close();
}