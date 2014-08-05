package reseau;

import java.net.*;
import java.io.*;
import reseau.Connexion;

public class ServeurTCP extends Thread{
   
	ServerSocket connexion;
	private static int nbclients;
	
	public ServeurTCP(ServerSocket s){
		connexion = s;
		nbclients = 0;
	}
	
	public static int getNbClients(){
		return nbclients;
	}
	
	public void run()
	{
		while(true)
		{
			try {
				Socket s = connexion.accept();
				System.out.println("un client s'est connecté!" + getNbClients());
				nbclients++;
				Connexion thread = new Connexion(s);
				thread.start();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
