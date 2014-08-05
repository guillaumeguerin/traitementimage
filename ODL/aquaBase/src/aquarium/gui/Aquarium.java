package aquarium.gui;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

import javax.swing.JPanel;

import aquarium.factory.SeaweedFactory;
import aquarium.factory.StoneFactory;
import aquarium.factory.FishFactory;
import aquarium.factory.RedFishFactory;
import aquarium.items.AquariumItem;
import aquarium.items.RedFish;
import aquarium.move.Mobile;
import aquarium.util.DestinationThread;

public class Aquarium extends JPanel {

	private static final long serialVersionUID = 1L;

	public static int SIZE_AQUA_X = 1024, SIZE_AQUA_Y = 800;
	private final int NB_STONES = 5, NB_SEAWEED = 5, NB_FISH = 4, NB_REDFISH = 2;

	private Image buffer = null;
	private Graphics gContext = null;

	private static Collection<AquariumItem> items = new ArrayList<AquariumItem>();

	private StoneFactory sf = new StoneFactory();
	private SeaweedFactory swf = new SeaweedFactory();
	private FishFactory f = new FishFactory();
	private RedFishFactory rf = new RedFishFactory();

	public Aquarium(boolean server) {
		//Fill aquarium

		if(server){
			//Le server met en place l'environement
			for(int i=0; i<NB_STONES; i++) {
				AquariumItem ai = sf.newItems();
				sf.sink(items, ai);
				items.add(ai);
			}
			for(int i=0; i<NB_SEAWEED; i++) {
				AquariumItem ai = swf.newItems();
				swf.sink(items, ai);
				items.add(ai);
			}
		}

		else{
			//Le client s'occupe de ses poissons
			for(int i=0; i<NB_FISH; i++) {
				AquariumItem ai = f.newItems();
				f.sink(items, ai);
				items.add(ai);
			}
			for(int i=0; i<NB_REDFISH; i++) {
				AquariumItem ai = rf.newItems();
				rf.sink(items, ai);
				items.add(ai);
			}	
		}
	}

	public Collection<AquariumItem> neighbourhood(AquariumItem item, int r) {
		return null;
	}

	//**************************
	// Accesseurs
	//**************************

	public static Collection<AquariumItem> getItems(){
		return items;
	}

	public static synchronized void addItem(AquariumItem ai){
		//TODO sync
		items.add(ai);
	}


	//**************************
	//  GRAPHIQUE
	//**************************
	public int getSizeX() {
		return SIZE_AQUA_X;
	}
	public int getSizeY() {
		return SIZE_AQUA_Y;
	}

	public synchronized void go() {
		synchronized(items){
			for(AquariumItem item : items)
				if ( item instanceof Mobile)
					//TODO handler
					((Mobile) item).move(items);

			draw();
		}
	}

	public synchronized void paint(Graphics g) {
		g.drawImage(buffer, 0, 0, this); 
	}

	/*
	 * Bufferisation de l'image
	 */
	private synchronized void draw() {
		if (this.gContext == null){
			buffer = createImage (SIZE_AQUA_X, SIZE_AQUA_Y);
			if(buffer!=null)this.gContext = buffer.getGraphics ();
		} else {
			gContext.setColor(Color.blue);
			gContext.fillRect(0, 0, SIZE_AQUA_X, SIZE_AQUA_Y);
			Iterator<AquariumItem> it = items.iterator();
			synchronized(it){
				while (it.hasNext()) {
					AquariumItem ai = it.next();
					ai.draw(gContext);
				}			
			}
		}
		this.repaint();
	}

}
