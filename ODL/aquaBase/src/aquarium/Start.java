package aquarium;

import aquarium.gui.Animation;
import aquarium.gui.Aquarium;
import reseau.*;

import java.net.*;
import java.io.*;


public class Start  {
	
	public static void main(String[] args) {		
		
		boolean server = true;
		
		if (args.length < 1){
			System.out.println("aucun argument, mode serveur");		
			try {
				ServerSocket s = new ServerSocket(7777);
				s.setReuseAddress(true);
				ServeurTCP connexionThread = new ServeurTCP(s);
				connexionThread.start();
				
				final Aquarium aquarium = new Aquarium(server);
				Animation animation = new Animation(aquarium);
				animation.afficher(server);
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		else {
			server = false;
			ClientTCP clientThread = new ClientTCP(7777, args[1]);
			
			final Aquarium aquarium = new Aquarium(server);
			Animation animation = new Animation(aquarium);	
			clientThread.start();
			animation.afficher(server);
		}
	}
}
