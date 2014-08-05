package aquarium.factory;

import java.awt.Point;

import aquarium.gui.Aquarium;
import aquarium.items.Fish;
import aquarium.items.RedFish;
import aquarium.util.RandomNumber;

public class RedFishFactory extends AquariumItemFactory<RedFish>{
	
	public RedFish newItems() {
		RedFish rf = new RedFish(RandomNumber.randomValue(RedFish.getMinWidth(), RedFish.getMaxWidth()));
		rf.setPosition(RandomNumber.randomPoint(0, Aquarium.SIZE_AQUA_X-RedFish.getMaxWidth(), 0, Aquarium.SIZE_AQUA_Y-RedFish.getMaxWidth()));
		return rf;
	    }
	
	public RedFish newItems(int taille, int posX, int posY/*, int destX, int destY*/) {
    	RedFish rf = new RedFish(taille);
    	rf.setPosition(new Point(posX, posY));
    	//f.setCurrentDestination(new Point(destX, destY));
    	return rf;
    }
	
}
