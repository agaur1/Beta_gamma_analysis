#include <RAT/DU/DSReader.hh>
#include <RAT/DU/Utility.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/Run.hh>
#include <RAT/DS/UniversalTime.hh>
#include <RAT/SunUtil.hh>
#include <RAT/DataCleaningUtility.hh>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>


void  Test_Modify( const string& fileName, std::string fitName = "" )
//void  PlotHitTimeResidualsFitPosition( const string& fileName, std::string fitName = "")
{
	//cout<<"debug 1 " <<endl;
  TFile *file = new TFile("myfiles_mc_1_mycode.root","RECREATE");
  TH1D* hHitTimeResiduals = new TH1D( "hHitTimeResidualsFit", "Hit time residuals using the Fit position", 1000, -500.0, 500.0 );
	//cout<<"debug 2 " <<endl;
  TCanvas *C = new TCanvas("c", "c",800,800);
  TH1D *h_nhits = new TH1D("nhits","nhits",200,0,1000);

  TH1D *h_nhitscleaned = new TH1D("nhits_cleaned","nhits_cleaned",200,0,1000);
 // TH1D *h_nhitscleaned_scint = new TH1D("nhitscleaned_scint","nhitscleaned_scint",1000,0,1000);
  TH1D *h_costheta = new TH1D("costheta","costheta",50,-1,1);
  TH1D *h_dotvec = new TH1D("dotvec","dotvec",25,-1,1);
  TH1D *h_costheta_new = new TH1D("costheta_new","costheta_new",25,-1,1);



  TH1D *h_nhits_early = new TH1D("nhits_early","nhits_early",200,0,200);
  TH2D *h_theta_phi = new TH2D("h_theta_phi","theta-phi",200,-1,1,628,-3.14,3.14);
  TH2D *h_theta_phi_mc = new TH2D("h_theta_phi_mc","theta-phi_mc",200,-1,1,628,-3.14,3.14);
  TH2D *h_theta_nhit = new TH2D("costheta_new","nhits_cleaned",25,-1,1,200,0,1000);

  TH2D *h_theta_phi_int = new TH2D("h_theta_phi_int","theta-phi_int",200,-1,1,628,-3.14,3.14);
  TH1D *h_theta_Ang_diif= new TH1D("costheta_Ang","costheta_Ang",50,0,3.14);
  TH1D *h_theta_Ang_diif_2= new TH1D("costheta_Ang_2","costheta_Ang_2",50,-1,1);


	//cout<<"debug 3 " <<endl;[]
 
  // If this is being done on data that does not require remote database connection
  // eg.: a simple simulation with default run number (0)
  // We can disable the remote connections:
  //
  // NOTE: Don't do this if you are using real data!!!
 // RAT::DB::Get()->SetAirplaneModeStatus(true);

	//cout<<"debug 4 " <<endl;
  RAT::DU::DSReader dsReader( fileName );

  // RAT::DU::Utility::Get()->GetLightPathCalculator() must be called *after* the RAT::DU::DSReader constructor.
	//cout<<"debug 5 " <<endl;
  RAT::DU::LightPathCalculator lightPath = RAT::DU::Utility::Get()->GetLightPathCalculator(); // To calculate the light's path
	//cout<<"debug 6 " <<endl;
  const RAT::DU::GroupVelocity& groupVelocity = RAT::DU::Utility::Get()->GetGroupVelocity(); // To get the group velocity
	//cout<<"debug 7 " <<endl;
  const RAT::DU::PMTInfo& pmtInfo = RAT::DU::Utility::Get()->GetPMTInfo(); // The PMT positions etc..
  cout<<"dsReader.GetEntryCount() = " << dsReader.GetEntryCount()<<endl;
  for( size_t iEntry = 0; iEntry < dsReader.GetEntryCount(); iEntry++ )

   // for( size_t iEntry = 0; iEntry < 1000; iEntry++ )
    {
    	//cout<<"Ankit"<<endl;

       int count_nhits=0;
  int count_nhits_early=0;

      const RAT::DS::Entry& rDS = dsReader.GetEntry( iEntry );

      const TVector3 eventPosition = rDS.GetMC().GetMCParticle(0).GetPosition();
      TVector3 eventPosition_2;

   //  cout<<eventPosition.x()<<eventPosition.y()<<eventPosition.z()<<endl;
       //h_nhits->Fill(rDS.evs.GetNhits());
      vector<int> pmtID;
      vector<int> GTID;
      for( size_t iEV = 0; iEV < rDS.GetEVCount(); iEV++ )
        {
           //h_nhits->Fill(rDS.evs.GetNhits());
         
          const RAT::DS::EV& rEV = rDS.GetEV( iEV );
          int count=0;
          GTID.push_back(rEV.GetGTID());
         // cout<<"GTID ="<<rEV.GetGTID()<<endl;

          // grab the fit information
         // if(fitName == "")
          //    {
                //fitName = rEV.GetDefaultFitName();
                //fitName = rEV.GetFitNames();
              //  fitName2 = rEV.GetFitNames()[0];

            // cout<<"11111111"<<fitName<<endl;
            // cout << "Ankit: " << rEV.GetFitNames()[0] << endl;
         //  }
             fitName= "partialFitter";
          //TVector3 eventPosition_2;
          double eventTime;

          try{
            //cout<<"Ankit"<<endl;
              const RAT::DS::FitVertex& rVertex = rEV.GetFitResult(fitName).GetVertex(0);
              if(!(rVertex.ValidPosition() && rVertex.ValidTime()))
                  continue; // fit invalid

              eventPosition_2 = rVertex.GetPosition();
              //cout<<eventPosition_2.x()<<eventPosition_2.y()<<eventPosition_2.z()<<endl;
              eventTime = rVertex.GetTime();
              //cout<<"Ankit"<<endl;
          // calculate time residuals
          const RAT::DS::CalPMTs& calibratedPMTs = rEV.GetCalPMTs();
         //if(rEV.GetNhitsCleaned()<90)
           h_nhits->Fill(rEV.GetNhits());

           h_nhitscleaned->Fill(rEV.GetNhitsCleaned());
          // if(rEV.GetNhitsCleaned()>300)
          // h_nhitscleaned_scint->Fill(rEV.GetNhitsCleaned());

            for( size_t iPMT = 0; iPMT < calibratedPMTs.GetCount(); iPMT++ )
            {
              const RAT::DS::PMTCal& pmtCal = calibratedPMTs.GetPMT( iPMT );
               
               //cout<<"Ankit"<<endl;
              lightPath.CalcByPosition( eventPosition_2, pmtInfo.GetPosition( pmtCal.GetID() ) );
              double distInInnerAV = lightPath.GetDistInInnerAV();
              double distInAV = lightPath.GetDistInAV();
              double distInWater = lightPath.GetDistInWater();
              const double transitTime = groupVelocity.CalcByDistance( distInInnerAV, distInAV, distInWater ); // Assumes a 400nm photon
              // Time residuals estimate the photon emission time relative to the event start so subtract off the transit time and eventTime
              double timeResidual = pmtCal.GetTime() - transitTime - eventTime;
              count_nhits++;
              if(timeResidual>= -100 && timeResidual<= -20)
              {
                   count++;
                   count_nhits_early++;
                   pmtID.push_back(pmtCal.GetID());
              }
              hHitTimeResiduals->Fill( timeResidual);
              //cout<<timeResidual<<endl;  
            }

            h_nhits_early->Fill(count);
            h_theta_nhit->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()), calibratedPMTs.GetCount())
        
	      }
       catch(exception& e)
       //{std::cout<<e.what()<<"wrong fit"<<std::endl;}
       {}
     //  catch(const RAT::DS::FitCollection::NoResultError&){
       // no fit result by the name of fitName
      //     continue;
      // }
      // catch (const RAT::DS::FitResult::NoVertexError&){
       //    // no fit vertex
       //    continue;
     //  }
      // catch(const RAT::DS::FitVertex::NoValueError&){
       //    // position or time missing
      //     continue;
      // }
       // DataNotFound --> implies no fit results are present, don't catch.
	}
      //cout<<"GTID ="<<GTID.size()<<endl;
    if(pmtID.size()>=1)
    {
    	TVector3 Xdiff=eventPosition_2-eventPosition;
    	TVector3 Xdiff_u = Xdiff.Unit();

       // cout<<"event number = "<<iEntry<<endl;
       // cout<<eventPosition.x()<<"  , "<<eventPosition.y()<<"  , "<<eventPosition.z()<<endl;
        //cout<<eventPosition_2.x()<<"  , "<<eventPosition_2.y()<<"  , "<<eventPosition_2.z()<<endl;
        //const RAT::DS::EV& rEV = rDS.GetEV( iEV );
        //cout<<"GTID ="<<GTID.at(0)<<endl;
       // cout<<"cos_theta="<<cos(eventPosition.Theta())<<" ,phi = "<<eventPosition.Phi()<<endl;
       h_theta_phi_mc->Fill(cos(eventPosition.Theta()),eventPosition.Phi());
        // cout<<"cos_theta="<<cos(eventPosition_2.Theta())<<" ,phi = "<<eventPosition_2.Phi()<<endl;
       h_theta_phi_int->Fill(cos(eventPosition_2.Theta()),eventPosition_2.Phi());
        for (int i=0; i<pmtID.size(); i++)
        {

        	//cout<<"pmt ID: "<<pmtID.at(i)<<endl;

        	TVector3 pmtCoord = pmtInfo.GetPosition( pmtID.at(i) );
        	TVector3 XdiffPMT = pmtCoord-eventPosition_2;
        	TVector3 XdiffPMT_u= XdiffPMT.Unit();
        	TVector3 XdiffPMT_cos= pmtCoord-eventPosition;
        	TVector3 XdiffPMT_cos_u= XdiffPMT_cos.Unit();




        	

            h_costheta->Fill(Xdiff_u.Dot(XdiffPMT_u));
              h_dotvec->Fill(XdiffPMT_cos_u.Dot(XdiffPMT_u));

              h_costheta_new->Fill(pmtCoord.Unit().Dot(eventPosition_2.Unit()));
       

           double Ang_diif_1= fabs((pmtCoord.Theta())- (eventPosition.Theta()));
            double Ang_diif_2= cos(pmtCoord.Theta())- cos(eventPosition_2.Theta());
            //double Ang_diif_1= (pmtCoord.CosTheta())- (eventPosition.CosTheta());

          //cout<<"cos_theta="<<cos(pmtCoord.Theta())<<" ,phi = "<<pmtCoord.Phi()<<endl;
         //cout<<"x="<<pmtCoord.x()<<" ,y= "<<pmtCoord.y()<<" ,z= "<<pmtCoord.z()<<endl;
        	h_theta_phi->Fill(cos(pmtCoord.Theta()),pmtCoord.Phi());
        	h_theta_Ang_diif->Fill(Ang_diif_1);
        	h_theta_Ang_diif_2->Fill(Ang_diif_2);


        	

        	
        }
		//break;
	}

            	
            }


    
  //hHitTimeResiduals->GetYaxis()->SetTitle( "Count per 1 ns bin" );
  //hHitTimeResiduals->GetXaxis()->SetTitle( "Hit time residuals [ns]" );
  hHitTimeResiduals->Draw();

   
  //std::cout<<count_nhits<<std::endl;
  //std::cout<<count_nhits_early<<std::endl;
   file->cd();//
   hHitTimeResiduals->Write();
   //TLegend leg(0.2,0.8,0.4,0.9);
   //h_nhits->Draw();
  h_nhits->Draw("hist");
  h_nhitscleaned->Draw("hist sames");
  h_nhits->GetXaxis()->SetTitle("Nhits cleaned");
  h_nhits->SetLineColor(kBlue);
    h_nhitscleaned->SetLineColor(kOrange);
    TLegend* leg = new TLegend(0.2, 0.2, .8, .8);
   //TLegend leg(0.3,0.8,0.7,0.9);
  // TLegend leg;
   leg->AddEntry("nhits","nhits");
   leg->AddEntry("nhits_cleaned","nhitscleaned");
   leg->Draw();

   C->SetLogy();
   C->SetGrid();
   C->SaveAs("nhits_pmt.pdf");
   h_theta_phi->SetMarkerStyle(21);
  // leg.AddEntry(h_theta_phi,"Early light");
   h_theta_phi->SetMarkerSize(2);
   h_theta_phi->SetMarkerColor(7);

   h_theta_phi_mc->SetMarkerStyle(23);
   h_theta_phi_mc->SetMarkerSize(3);
   h_theta_phi_mc->SetMarkerColor(9);
   //leg.AddEntry(h_theta_phi_mc,"MC Injection");
   h_theta_phi_int->SetMarkerStyle(25);
   h_theta_phi_int->SetMarkerSize(4);
   h_theta_phi_int->SetMarkerColor(3);
  // leg.AddEntry(h_theta_phi_int,"Reconstructed");
   h_theta_phi->GetXaxis()->SetTitle("Cos_theta");
  h_theta_phi->GetYaxis()->SetTitle("phi");


   h_theta_phi->Write();
    file->Write();
  }

