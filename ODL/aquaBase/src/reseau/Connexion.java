package reseau;


import java.io.*;
import java.net.Socket;

import java.util.Collection;
import java.util.Set;

import java.util.ArrayList;

import aquarium.gui.Aquarium;
import aquarium.items.AquariumItem;
import aquarium.items.Stone;
import aquarium.items.Seaweed;
import aquarium.util.Protocole;

import java.awt.Point;

public class Connexion extends Thread{

	private static Set<ClientTCP> set;
	private Socket client_a_ecouter;
	private int id;
	private Collection<AquariumItem> itemsCreesClient;
	
	public Connexion(Socket s) {
		client_a_ecouter = s;
	}
	
	public void setItemsCreesClient(Collection<AquariumItem> items){
		itemsCreesClient = items;
	}
	
	public void run(){
			
		try{
			
			InputStream in = client_a_ecouter.getInputStream();
			OutputStream out = client_a_ecouter.getOutputStream();
			BufferedReader br = new BufferedReader(new InputStreamReader(in, "utf-8"));
			PrintStream ps = new PrintStream(out, false, "utf-8");
			String requete, type;
			
			while(true){
				requete = br.readLine();
				
				if(requete == null)
					break;
				
				if(requete.equals("READY")){
					id = ServeurTCP.getNbClients();
					ps.println("SET_ID " + id);
				}
				
				if(requete != null && requete.equals("nombre d'objets server")){
					Collection<AquariumItem> items = Aquarium.getItems();
					ps.println(items.size());
				}
				
				if(requete.equals("envoie moi la carte"))
				{
					Collection<AquariumItem> items = Aquarium.getItems();
					Point p;
					for(AquariumItem item : items){
						if ( item instanceof Stone){
							p = ((AquariumItem) item).getPosition();
							ps.println("pierre " + item.getWidth() + " " + p.getX() + " " + p.getY());
						}
						if ( item instanceof Seaweed){
							p = ((AquariumItem) item).getPosition();
							ps.println("algue " + item.getWidth() + " " + p.getX() + " " + p.getY());
						}
					}
					requete = br.readLine();
					if(requete != null){
						int nbobjets = Integer.parseInt(requete);
						for(int i = 0; i < nbobjets; i++){
							requete = br.readLine();
							Protocole.ajouterItem(requete);
						}
					}
				}

			}
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
			
	}
}