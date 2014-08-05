package aquarium.util;

import aquarium.items.RedFish;

public class DestinationThread extends Thread{
	
	private RedFish rf;
   
	
    public DestinationThread(RedFish redfish)
    {
    	rf = redfish;
    }
    
    public void run(){
    	
    	while(true) {
			rf.setCurrentDestination(rf.target(null));
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {};
		}
    }

}
