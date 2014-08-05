package aquarium.factory;

import java.awt.Point;

import aquarium.gui.Aquarium;
import aquarium.items.Fish;
import aquarium.items.Stone;
import aquarium.util.RandomNumber;

public class FishFactory extends AquariumItemFactory<Fish> {

    public Fish newItems() {
	Fish f = new Fish(RandomNumber.randomValue(Fish.getMinWidth(), Fish.getMaxWidth()));
	f.setPosition(RandomNumber.randomPoint(0, Aquarium.SIZE_AQUA_X-Fish.getMaxWidth(), 0, Aquarium.SIZE_AQUA_Y-Fish.getMaxWidth()));
	return f;
    }

    public Fish newItems(int taille, int posX, int posY/*, int destX, int destY*/) {
    	Fish f = new Fish(taille);
    	f.setPosition(new Point(posX, posY));
    	//f.setCurrentDestination(new Point(destX, destY));
    	return f;
    }
    
}