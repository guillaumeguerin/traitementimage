package reseau;

import java.net.*;
import java.util.Collection;
import java.io.*;

import aquarium.gui.Aquarium;
import aquarium.items.AquariumItem;
import aquarium.items.MobileItem;
import aquarium.items.RedFish;
import aquarium.items.Stone;
import aquarium.items.Fish;
import aquarium.move.Mobile;
import aquarium.util.Protocole;

import java.awt.Point;

public class ClientTCP extends Thread{

	private String addresse;
	private int port;
	private static int idclient;
	private Collection<AquariumItem> itemsCrees;
	
	public ClientTCP(int portServeur, String adresseServeur)
	{
		addresse = adresseServeur;
		port = portServeur;
		itemsCrees =  Aquarium.getItems(); //liste contenant uniquement les objets crées par ce client.
	}
	
	public static void setIdClient(int id){
		idclient = id;
	}
	
	public int getIdClient(){
		return idclient;
	}
	
	public Collection<AquariumItem> getItemsCrees(){
		return itemsCrees;
	}
	
	public void run()
	{
		Socket socket;
		try {

			socket = new Socket("localhost", port);
			InputStream in = socket.getInputStream();
			OutputStream out = socket.getOutputStream();
			BufferedReader br = new BufferedReader(new InputStreamReader(in, "utf-8"));
			PrintStream ps = new PrintStream(out, false, "utf-8");
			String reponse;
						
			ps.println("READY");
			reponse = br.readLine();
			
			//demande la taille de la collection d'objets du server
			ps.println("nombre d'objets server");
			String nbobjetsserver = br.readLine();
	
			Collection<AquariumItem> items = Aquarium.getItems();
			int nbobjetsclient = items.size(); //on récupère le nombre d'objets créés par le client uniquement
			
			ps.println("envoie moi la carte");
			for(int i = 0; i < Integer.parseInt(nbobjetsserver); i++){
				reponse = br.readLine();
				Protocole.ajouterItem(reponse);
			}
			
			
			//envoie ses poissons au server
			ps.println(nbobjetsclient);
			Point p;
			for(AquariumItem item : items){
				if ( item instanceof Fish){
					p = ((AquariumItem) item).getPosition();
					ps.println("poisson " + item.getWidth() + " " + p.getX() + " " + p.getY() /* + " " + item.getCurrentDestination()*/);
				}
				if ( item instanceof RedFish){
					p = ((AquariumItem) item).getPosition();
					ps.println("redfish " + item.getWidth() + " " + p.getX() + " " + p.getY() /* + " " + item.getCurrentDestination()*/);
				}
			}
			
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}		
}