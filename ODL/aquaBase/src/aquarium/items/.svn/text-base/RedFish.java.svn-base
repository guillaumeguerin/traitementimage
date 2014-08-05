package aquarium.items;

import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.net.URL;
import java.util.Collection;
import aquarium.gui.Aquarium;

import aquarium.util.*;

public class RedFish extends MobileItem {
	
	private static final int MIN_WIDTH = 30, MAX_WIDTH = 60;
    private static URL u = ClassLoader.getSystemResource("image/poissonrouge.png");
    private static Image fishImage = Toolkit.getDefaultToolkit().createImage(u);
	
    public RedFish(int width) {
    	super(width, fishImage);
    	DestinationThread thread = new DestinationThread(this);
    	thread.start();
        }
    
    static public int getMaxWidth() {
		return MAX_WIDTH;
	}
	
	static public int getMinWidth() {
		return MIN_WIDTH;
	}
    
	public Point target(Collection<AquariumItem> neighbours){
		return RandomNumber.RandomDest();
	}
}
