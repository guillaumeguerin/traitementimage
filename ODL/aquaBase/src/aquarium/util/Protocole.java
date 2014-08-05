package aquarium.util;

import reseau.ClientTCP;
import reseau.ServeurTCP;
import aquarium.factory.FishFactory;
import aquarium.factory.RedFishFactory;
import aquarium.factory.SeaweedFactory;
import aquarium.factory.StoneFactory;
import aquarium.gui.Aquarium;
import aquarium.items.AquariumItem;

public class Protocole {
	
	public static void ajouterItem(String s){
		if(s != null){
			String [] args;
			args = s.split(" ");
			
			if(args[0].equals("pierre")){
				StoneFactory sf = new StoneFactory();
				AquariumItem ai = sf.newItems(Integer.parseInt(args[1]), parserEntier(args[2]), parserEntier(args[3]));
				sf.sink(Aquarium.getItems(), ai);
				Aquarium.addItem(ai);
			}
			else if(args[0].equals("algue")){
				SeaweedFactory swf = new SeaweedFactory();
				AquariumItem ai = swf.newItems(Integer.parseInt(args[1]), parserEntier(args[2]), parserEntier(args[3]));
				swf.sink(Aquarium.getItems(), ai);
				Aquarium.addItem(ai);
			}
			else if(args[0].equals("poisson")){
				FishFactory f = new FishFactory();
				AquariumItem ai = f.newItems(Integer.parseInt(args[1]), parserEntier(args[2]), parserEntier(args[3])/*, parserEntier(args[4]), parserEntier(args[5])*/);
				f.sink(Aquarium.getItems(), ai);
				Aquarium.addItem(ai);
			}		
			else if(args[0].equals("redfish")){
				RedFishFactory f = new RedFishFactory();
				AquariumItem ai = f.newItems(Integer.parseInt(args[1]), parserEntier(args[2]), parserEntier(args[3])/*, parserEntier(args[4]), parserEntier(args[5])*/);
				f.sink(Aquarium.getItems(), ai);
				Aquarium.addItem(ai);
			}
		}
	}
	
	public static void protocoleTitanPad(String s){
		String [] args;
		args = s.split(" ");
		if(args[0].equals("SET_ID")){
			ClientTCP.setIdClient(ServeurTCP.getNbClients());
		}
	}

	
	//tansforme un double sous forme de chaine de caractère en entier en le tronquant a la virgule
	public static int parserEntier(String s){
		int i = 0;
		while(s.charAt(i) != '.'){
			i++;
		}
		return Integer.parseInt(s.substring(0, i));
	}
}