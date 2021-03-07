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


void  Test_Modify( const string& fileName, std::string fitName = "" )
//void  PlotHitTimeResidualsFitPosition( const string& fileName, std::string fitName = "")
{
	cout<<"debug 1 " <<endl;
  TFile *file = new TFile("myfiles.root","RECREATE");
  TH1D* hHitTimeResiduals = new TH1D( "hHitTimeResidualsFit", "Hit time residuals using the Fit position", 1000, -500.0, 500.0 );
	cout<<"debug 2 " <<endl;

  TH1D *h_nhits = new TH1D("nhits","nhits",200,0,200);
  TH1D *h_nhits_early = new TH1D("nhits_early","nhits_early",200,0,200);
	cout<<"debug 3 " <<endl;
  int count_nhits=0;
  int count_nhits_early=0;
  // If this is being done on data that does not require remote database connection
  // eg.: a simple simulation with default run number (0)
  // We can disable the remote connections:
  //
  // NOTE: Don't do this if you are using real data!!!
 // RAT::DB::Get()->SetAirplaneModeStatus(true);

	cout<<"debug 4 " <<endl;
  RAT::DU::DSReader dsReader( fileName );

  // RAT::DU::Utility::Get()->GetLightPathCalculator() must be called *after* the RAT::DU::DSReader constructor.
	cout<<"debug 5 " <<endl;
  RAT::DU::LightPathCalculator lightPath = RAT::DU::Utility::Get()->GetLightPathCalculator(); // To calculate the light's path
	cout<<"debug 6 " <<endl;
  const RAT::DU::GroupVelocity& groupVelocity = RAT::DU::Utility::Get()->GetGroupVelocity(); // To get the group velocity
	cout<<"debug 7 " <<endl;
  const RAT::DU::PMTInfo& pmtInfo = RAT::DU::Utility::Get()->GetPMTInfo(); // The PMT positions etc..
  cout<<"dsReader.GetEntryCount() = " << dsReader.GetEntryCount()<<endl;
  for( size_t iEntry = 0; iEntry < dsReader.GetEntryCount(); iEntry++ )
    //for( size_t iEntry = 0; iEntry < 1000; iEntry++ )
    {
    	//cout<<"Ankit"<<endl;
      const RAT::DS::Entry& rDS = dsReader.GetEntry( iEntry );

      for( size_t iEV = 0; iEV < rDS.GetEVCount(); iEV++ )
        {
          
          const RAT::DS::EV& rEV = rDS.GetEV( iEV );
          int count=0;

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
          TVector3 eventPosition;
          double eventTime;

          try{
            //cout<<"Ankit"<<endl;
              const RAT::DS::FitVertex& rVertex = rEV.GetFitResult(fitName).GetVertex(0);
              if(!(rVertex.ValidPosition() && rVertex.ValidTime()))
                  continue; // fit invalid

              eventPosition = rVertex.GetPosition();
              eventTime = rVertex.GetTime();
              //cout<<"Ankit"<<endl;
          // calculate time residuals
          const RAT::DS::CalPMTs& calibratedPMTs = rEV.GetCalPMTs();
          h_nhits->Fill(calibratedPMTs.GetCount());
          for( size_t iPMT = 0; iPMT < calibratedPMTs.GetCount(); iPMT++ )
            {
              const RAT::DS::PMTCal& pmtCal = calibratedPMTs.GetPMT( iPMT );
               
               //cout<<"Ankit"<<endl;
              lightPath.CalcByPosition( eventPosition, pmtInfo.GetPosition( pmtCal.GetID() ) );
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
              }
              hHitTimeResiduals->Fill( timeResidual);
              //cout<<timeResidual<<endl;  
            }

            h_nhits_early->Fill(count);
        
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
    if(count_nhits_early>=3)

            {
            	cout<<"event number = "<<iEntry<<endl;

 for( size_t iEV = 0; iEV < rDS.GetEVCount(); iEV++ )
        {
          
          const RAT::DS::EV& rEV = rDS.GetEV( iEV );
          
             fitName= "partialFitter";
          TVector3 eventPosition;
          double eventTime;

          try{
            //cout<<"Ankit"<<endl;
              const RAT::DS::FitVertex& rVertex = rEV.GetFitResult(fitName).GetVertex(0);
              if(!(rVertex.ValidPosition() && rVertex.ValidTime()))
                  continue; // fit invalid

             
          const RAT::DS::CalPMTs& calibratedPMTs = rEV.GetCalPMTs();
          h_nhits->Fill(calibratedPMTs.GetCount());
          for( size_t iPMT = 0; iPMT < calibratedPMTs.GetCount(); iPMT++ )
            {
              const RAT::DS::PMTCal& pmtCal = calibratedPMTs.GetPMT( iPMT );
               
               cout<<"Pmt id = " << pmtCal.GetID()<<endl;
              
            }
        
	      }
       catch(exception& e)
       //{std::cout<<e.what()<<"wrong fit"<<std::endl;}
       {}
   }
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

            	
            }

    }
  //hHitTimeResiduals->GetYaxis()->SetTitle( "Count per 1 ns bin" );
  //hHitTimeResiduals->GetXaxis()->SetTitle( "Hit time residuals [ns]" );
  hHitTimeResiduals->Draw();
  std::cout<<count_nhits<<std::endl;
  std::cout<<count_nhits_early<<std::endl;
   file->cd();//
   hHitTimeResiduals->Write();
    file->Write();
  }
